#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globalparam.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QTimer>
#include "tform1.h"
#include <QtMath>
#include "tformconfig1.h"
#include "tform7.h"
#include "tformcali.h"
#include <cmath>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , serialPort(new QSerialPort(this))
    , connectStatusLabel(new QLabel(this))
    , runningStatusLabel(new QLabel(this))
    , txResetTimer(new QTimer(this))
    , rxResetTimer(new QTimer(this))
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::init()
{
    refreshPort();
    //发送数据
    sendTimer = new QTimer(this);
    connect(sendTimer, &QTimer::timeout, this, &MainWindow::onSendTimerTimeout);
    sendTimer->setInterval(100);
    sendTimer->start();
    //接收数据
    receiveTimer = new QTimer(this);
    connect(receiveTimer, &QTimer::timeout, this, &MainWindow::onReceiveTimerTimeout);
    receiveTimer->setInterval(10);
    receiveTimer->start();
    //状态栏
    connectStatusLabel->setMinimumWidth(150);
    ui->statusbar->addWidget(connectStatusLabel);
    connectStatusLabel->setText(connStatus.arg("未连接"));
    connectStatusLabel->setStyleSheet("QLabel { background-color : red; color : white; }");
    ui->statusbar->addWidget(connectStatusLabel);
    runningStatusLabel->setMinimumWidth(150);
    ui->statusbar->addWidget(runningStatusLabel);
    //指示灯定时器相关
    txResetTimer->setSingleShot(true);
    connect(txResetTimer, &QTimer::timeout, this, &MainWindow::on_txResetTimer_timeout);
    rxResetTimer->setSingleShot(true);
    connect(rxResetTimer, &QTimer::timeout, this, &MainWindow::on_rxResetTimer_timeout);

    setWindowTitle(TITLE);
}
void MainWindow::onSendTimerTimeout()
{
    if(connFlag == 0)
    {
        return;
    }
    if(waitMessageRemaingTime > 0)
    {
        waitMessageRemaingTime--;
    }
    if(dataRefreshRemaingTime > 0)
    {
        dataRefreshRemaingTime--;
    }

    //说明串口空闲，看看有没有手动的命令要下发
    if(waitMessageRemaingTime == 0)
    {
        if(manualFlag == 1)
        {
            //手动命令下发
            sendPortData();
            manualFlag = 0;
        }
        //说明没有手动命令要下发，就判断是否到了刷新时间
        else if(dataRefreshRemaingTime <= 0)
        {
            //获取实时数据
            sendGetRealTimeDataCMD();
        }
    }
}

void MainWindow::refreshPort()
{
    //清空combox中已经有的串口名
    ui->comboBox_2->clear();
    // 获取系统中所有可用串口
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();

    // 按串口名升序排序
    std::sort(portList.begin(), portList.end(), [](const QSerialPortInfo &a, const QSerialPortInfo &b) {
        return a.portName() < b.portName();
    });

    // 遍历可用串口，将串口名添加到 comboBox中
    for (const QSerialPortInfo &portInfo : portList) {
        ui->comboBox_2->addItem(portInfo.portName());
    }
}

void MainWindow::sendPortData(QByteArray data)
{
    if(data == nullptr)
    {
        sendSerialData(manualSendDataBuf);
    }
    else
    {
        sendSerialData(data);
    }
    //设置等待时间
    waitMessageRemaingTime = 20;
}

void MainWindow::cacheReceiveData()
{
    if(serialPort->isOpen())
    {
        QByteArray data = serialPort->readAll();
        for (auto byte : data) {
            receiveDataBuf[receiveEndIndex] = byte;
            receiveEndIndex = (receiveEndIndex + 1) % 500;
        }
        if(data.size() > 0)
        {
            //闪一下绿色
            ui->lab_rx->setStyleSheet(
                "QLabel {"
                "    border-radius: 5px;"
                "    background-color: #00F000;"
                "}"
                );
            if(rxResetTimer->isActive())
            {
                rxResetTimer->stop();
            }
            rxResetTimer->start(500);
        }
    }
}

void MainWindow::onReceiveTimerTimeout()
{
    if(connFlag == 0)
    {
        return;
    }
    cacheReceiveData();
    //当缓冲区的消息长度大于messageSize，那说明可能存在一条完整的响应
    while ((receiveEndIndex + 500 - receiveStartIndex) % 500 >= 6) {
        int module = static_cast<uint8_t>(receiveDataBuf[receiveStartIndex]);
        int cmd = static_cast<uint8_t>(receiveDataBuf[(receiveStartIndex + 1) % 500]);
        //没有匹配到开始
        if(module != MODULE || (cmd != 3 && cmd != 6 && cmd != 0x10))
        {
            //更新开始点
            receiveStartIndex = (receiveStartIndex + 1) % 500;
            continue;
        }
        //匹配到开始,再匹配下长度是否符合
        int messageSize = 0;
        if(cmd == 3)
        {
            messageSize = static_cast<uint8_t>(receiveDataBuf[(receiveStartIndex + 2) % 500]) + 5;
        }
        if(cmd == 6)
        {
            messageSize = 8;
        }
        if(cmd == 16)
        {
            messageSize = 16;
        }
        if((receiveEndIndex + 500 - receiveStartIndex) % 500 < messageSize){
            //消息还没接收完整，等下一次定时去接,不更新开始点
            break;
        }
        //构建消息
        QByteArray buf;
        for (int var = 0; var < messageSize; var++) {
            buf.append(receiveDataBuf[(receiveStartIndex + var) % 500]);
        }
        //判断是否是一个完整的消息
        if(receiveDataCRCCheck(buf))
        {
            //首先更新接收缓冲区的开始坐标
            if(tform1 != nullptr)
            {
                tform1->displayInfo("串口上传上来且验证通过的一条消息：" + buf.toHex());
            }
            receiveStartIndex = (receiveStartIndex + messageSize) % 500;
            //清空等待时间
            waitMessageRemaingTime = 0;
            dealMessage(reinterpret_cast<quint8*>(buf.data()));
            break;
        }
        //crc校验失败
        else
        {
            //更新开始点
            receiveStartIndex = (receiveStartIndex + 1) % 500;
            continue;
        }
    }
}

void MainWindow::dealMessage(quint8* data)
{
    //查询命令
    if(data[1] == READ_CMD)
    {
        for(quint16 i = 0; i < data[2] / 2; i++)
        {
            regs[i] =((data[3 + i * 2] << 8) | data[4 + i * 2]);
        }
        refreshAll();
    }
    // else if(data[1] == WRITE_ONE_CMD)
    // {
    //     //写入命令返回，立刻回显
    //     quint16 addr = ((data[2] << 8) | data[3]);
    //     quint16 value = ((data[4] << 8) | data[5]);
    //     if(addr < config2AddrStart1 && addr >= config1AddrStart)
    //     {
    //         if(tformConfig1 != nullptr)
    //         {
    //             tformConfig1->annalyzeOneData(addr, value);
    //         }
    //     }else if(addr >= config2AddrStart1)
    //     {
    //         if(tformConfig2 != nullptr)
    //         {
    //             tformConfig2->annalyzeOneData(addr, value);
    //         }
    //     }else if(addr < config1AddrStart)
    //     {
    //         if(tformCali != nullptr)
    //         {
    //             tformCali->sendGetDataCmd();
    //         }
    //         QMessageBox::information(this, tr("提示"), tr("写入成功!"));
    //     }

    // }
    // else if(data[1] == WRITE_MULTIPLE_CMD)
    // {
    //     //多个写入命令返回，暂时没有处理逻辑、
    // }
}

void MainWindow::refreshAll()
{
    refresh();
    if(tformConfig1 != nullptr)
    {
          tformConfig1->refresh();
    }
}

void MainWindow::refresh()
{
    ui->l6->setText(QString::number(static_cast<float>(regs[6] * 1.0/ qPow(10, pows[6])), 'f', pows[6]));
    ui->l7->setText(QString::number(static_cast<float>(regs[7] * 1.0/ qPow(10, pows[7])), 'f', pows[7]));
    ui->l8->setText(QString::number(static_cast<float>(regs[8] * 1.0/ qPow(10, pows[8])), 'f', pows[8]));
    ui->l9->setText(QString::number(static_cast<float>(regs[9] * 1.0/ qPow(10, pows[9])), 'f', pows[9]));
    ui->l10->setText(QString::number(static_cast<float>(regs[10] * 1.0/ qPow(10, pows[10])), 'f', pows[10]));
    ui->l11->setText(QString::number(static_cast<float>(regs[11] * 1.0/ qPow(10, pows[11])), 'f', pows[11]));
    ui->l12->setText(QString::number(static_cast<float>(regs[12] * 1.0/ qPow(10, pows[12])), 'f', pows[12]));
    ui->l13->setText(QString::number(static_cast<float>(regs[13] * 1.0/ qPow(10, pows[13])), 'f', pows[13]));
    ui->l14->setText(QString::number(static_cast<float>(regs[14] * 1.0/ qPow(10, pows[14])), 'f', pows[14]));
    ui->l23->setText(QString::number(static_cast<float>(regs[23] * 1.0/ qPow(10, pows[23])), 'f', pows[23]));
    ui->l24->setText(QString::number(static_cast<float>(regs[24] * 1.0/ qPow(10, pows[24])), 'f', pows[24]));
    ui->l25->setText(QString::number(static_cast<float>(regs[25] * 1.0/ qPow(10, pows[25])), 'f', pows[25]));
    ui->l26->setText(QString::number(static_cast<float>(regs[26] * 1.0/ qPow(10, pows[26])), 'f', pows[26]));
    ui->l27->setText(QString::number(static_cast<float>(regs[27] * 1.0/ qPow(10, pows[27])), 'f', pows[27]));
    ui->l28->setText(QString::number(static_cast<float>(regs[28] * 1.0/ qPow(10, pows[28])), 'f', pows[28]));
    ui->l29->setText(QString::number(static_cast<float>(regs[29] * 1.0/ qPow(10, pows[29])), 'f', pows[29]));
    ui->l30->setText(QString::number(static_cast<float>(regs[30] * 1.0/ qPow(10, pows[30])), 'f', pows[30]));
    ui->l31->setText(QString::number(static_cast<float>(regs[31] * 1.0/ qPow(10, pows[31])), 'f', pows[31]));
    ui->l32->setText(QString::number(static_cast<float>(regs[32] * 1.0/ qPow(10, pows[32])), 'f', pows[32]));
    QString warnPortStr = getWarnText(regs[5]);
    if(warnPortStr.length() == 0)
    {
        ui->bms_warn_prot->setText(NO_WARN_PROT_STR);
        ui->bms_warn_prot->setProperty("status", "normal");
    }else
    {
        ui->bms_warn_prot->setText(warnPortStr);
        ui->bms_warn_prot->setProperty("status", "warn");
    }
    ui->bms_warn_prot->style()->unpolish(ui->bms_warn_prot);
    ui->bms_warn_prot->style()->polish(ui->bms_warn_prot);
    ui->bms_warn_prot->update();

    ui->label_39->setText(getWorkText(regs[4]));
}


QString MainWindow::getWarnText(quint16 value)
{
    QString text;
    if((value & 1) == 1)
    {
        text.append(" P侧过压保护");
    }
    if(((value >> 1) & 1) == 1)
    {
        text.append(" P侧欠压保护");
    }
    if(((value >> 2) & 1) == 1)
    {
        text.append(" 电池过压保护");
    }
    if(((value >> 3) & 1) == 1)
    {
        text.append(" 电池欠压保护");
    }
    if(((value >> 5) & 1) == 1)
    {
        text.append(" 充放电过流保护");
    }
    if(((value >> 9) & 1) == 1)
    {
        text.append(" 高温保护");
    }
    if(((value >> 12) & 1) == 1)
    {
        text.append(" 高温告警");
    }
    return text;
}

QString MainWindow::getProtText(quint16 value)
{
    QString text;
    if((value & 1) == 1)
    {
        text.append(" 充放电端总电压过压保护");
    }
    if(((value >> 1) & 1) == 1)
    {
        text.append(" 充放电端总电压低压保护");
    }
    if(((value >> 2) & 1) == 1)
    {
        text.append(" 电池总压过压保护");
    }
    if(((value >> 3) & 1) == 1)
    {
        text.append(" 电池总压欠压保护");
    }
    if(((value >> 4) & 1) == 1)
    {
        text.append(" 充电过流保护");
    }
    if(((value >> 5) & 1) == 1)
    {
        text.append(" 放电过流保护");
    }
    if(((value >> 8) & 1) == 1)
    {
        text.append(" 充电MOS管高温保护");
    }
    if(((value >> 9) & 1) == 1)
    {
        text.append(" 放电MOS管高温保护");
    }
    if(((value >> 10) & 1) == 1)
    {
        text.append(" 环境温度高保护");
    }
    if(((value >> 12) & 1) == 1)
    {
        text.append(" 电源反接保护");
    }
    if(((value >> 13) & 1) == 1)
    {
        text.append(" 短路保护");
    }
    if(((value >> 14) & 1) == 1)
    {
        text.append(" 电感电流不均流保护");
    }
    if(((value >> 15) & 1) == 1)
    {
        text.append(" 模块重载保护");
    }
    return text;
}

QString MainWindow::getWorkText(quint16 value)
{
    QString text = "工作模式:";
    if(value == 0)
    {
        text.append(" 待机状态");
    }else if(value == 1)
    {
        text.append(" 恒压限流模式");
    }else if(value == 3)
    {
        text.append(" 恒流限压模式");
    }else
    {
        text.append(" 未知");
    }
    return text;
}

bool MainWindow::receiveDataCRCCheck(const QByteArray &data)
{
    QByteArray crcResultArray = calculateCRCArray(data, data.length() - 2);
    if(tform1 != nullptr)
    {
        tform1->displayInfo("待校验的下位机数据：" + data.toHex());
    }
    return crcResultArray[0] == data[data.size() - 2] && crcResultArray[1] == data[data.size() - 1];
}

void MainWindow::sendSerialData(const QByteArray &data)
{
    //串口未开启
    if(!serialPort->isOpen())
    {
        QMessageBox::critical(this, "错误", "串口未开启!");
        return;
    }
    if(tform1 != nullptr)
    {
        tform1->displayInfo("上位机发送的串口数据：" + data.toHex());
    }
    serialPort->write(data);
    //闪一下绿色
    ui->lab_tx->setStyleSheet(
        "QLabel {"
        "    border-radius: 5px;"
        "    background-color: #00F000;"
        "}"
        );
    if(txResetTimer->isActive())
    {
        txResetTimer->stop();
    }
    txResetTimer->start(500);//500ms后恢复
}

// 计算Modbus-RTU CRC16的方法，返回高低字节的QByteArray
QByteArray MainWindow::calculateCRCArray(const QByteArray &data, int length) {
    uint16_t crc = 0xFFFF; // 初始化CRC为0xFFFF

    for (int i = 0; i < length; i++) {
        crc ^= static_cast<uint8_t>(data[i]); // 将当前字节异或到CRC低位

        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001; // 如果最低位是1，右移后与0xA001异或
            } else {
                crc = crc >> 1; // 否则只右移
            }
        }
    }
    QByteArray crcArray;
    crcArray.append(static_cast<char>(crc & 0xFF));      // CRC低字节
    crcArray.append(static_cast<char>((crc >> 8) & 0xFF)); // CRC高字节
    return crcArray;
}

void MainWindow::sendGetRealTimeDataCMD()
{
    QByteArray buf;
    buf.append(MODULE);
    buf.append(READ_CMD);

    //起始地址
    buf.append(static_cast<char>(0x04));
    buf.append(static_cast<char>(0x00));
    //个数
    buf.append(static_cast<char>(0x00));
    buf.append(static_cast<char>(0x3D));
    QByteArray crcArray = calculateCRCArray(buf, 6);
    //crC
    buf.append(crcArray[0]);
    buf.append(crcArray[1]);
    sendPortData(buf);
    dataRefreshRemaingTime = DATA_REFRESH_CYCLE;
}

void MainWindow::manualReadCMDBuild(quint16 startHigh, quint16 startLow, char numHigh, char numLow)
{
    if(manualFlag == 1)
    {
        QMessageBox::information(this, "冲突", "当前有其他手动命令在发送, 请稍后再试!");
        return;
    }
    manualSendDataBuf.clear();
    manualSendDataBuf.append(MODULE);
    manualSendDataBuf.append(READ_CMD);
    manualSendDataBuf.append(startHigh);
    manualSendDataBuf.append(startLow);
    manualSendDataBuf.append(numHigh);
    manualSendDataBuf.append(numLow);
    QByteArray crcArray = calculateCRCArray(manualSendDataBuf, 6);
    //crC
    manualSendDataBuf.append(crcArray[0]);
    manualSendDataBuf.append(crcArray[1]);
    lastStartAddr = ((startHigh << 8) | startLow);
    manualFlag = 1;
}

void MainWindow::manualWriteOneCMDBuild(quint16 startHigh, quint16 startLow, char valueHigh, char valueLow, quint8 secFlag)
{
    if(manualFlag == 1)
    {
        QMessageBox::information(this, "冲突", "当前有其他手动命令在发送, 请稍后再试!");
        return;
    }
    manualSendDataBuf.clear();
    manualSendDataBuf.append(MODULE);
    manualSendDataBuf.append(WRITE_ONE_CMD);
    manualSendDataBuf.append(startHigh);
    manualSendDataBuf.append(startLow);
    manualSendDataBuf.append(valueHigh);
    manualSendDataBuf.append(valueLow);
    QByteArray crcArray = calculateCRCArray(manualSendDataBuf, 6);
    //crC
    manualSendDataBuf.append(crcArray[0]);
    manualSendDataBuf.append(crcArray[1]);
    lastStartAddr = ((startHigh << 8) | startLow);
    if(secFlag == 0)
    {
        manualFlag = 1;
    }
    else
    {
        manualFlag = 2;
    }
}

void MainWindow::on_connBtn_2_clicked()
{
    refreshPort();
}


void MainWindow::on_connBtn_clicked()
{
    if(ui->connBtn->text() == "建立连接")
    {
        if(ui->comboBox_2->currentIndex() == -1)
        {
            QMessageBox::information(this, tr("提示"),
                                     tr("请选择串口!"));
            return;
        }
        serialPort->setBaudRate(BR);
        serialPort->setPortName(ui->comboBox_2->currentText());
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setParity(QSerialPort::NoParity);
        //连接失败
        if(!serialPort->open(QIODevice::ReadWrite))
        {
            QMessageBox::information(this, tr("错误"),
                                     tr("无法启动串口通讯！！！"));
            connFlag = 0;
            connectStatusLabel->setText(connStatus.arg("未连接"));
            connectStatusLabel->setStyleSheet("QLabel { background-color : red; color : white; }");
            return;
        }
        //连接成功
        else
        {
            connFlag = 1;
            ui->comboBox_2->setEnabled(false);
            connectStatusLabel->setText(connStatus.arg("已连接"));
            connectStatusLabel->setStyleSheet("QLabel { background-color : green; color : white; }");
            ui->connBtn->setText("断开连接");
        }
    }
    else if(ui->connBtn->text() == "断开连接")
    {
        if(!serialPort->isOpen())
        {
            return;
        }
        serialPort->close();
        connFlag = 0;
        ui->comboBox_2->setEnabled(true);
        connectStatusLabel->setText(connStatus.arg("未连接"));
        connectStatusLabel->setStyleSheet("QLabel { background-color : red; color : white; }");
        ui->connBtn->setText("建立连接");

        ui->bms_warn_prot->setText("未连接");
        ui->bms_warn_prot->setProperty("status", "disconnected");
        ui->bms_warn_prot->style()->unpolish(ui->bms_warn_prot);
        ui->bms_warn_prot->style()->polish(ui->bms_warn_prot);
        ui->bms_warn_prot->update();
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if(tformConfig1 == nullptr)
    {
        tformConfig1 = new TFormConfig1(this);
        tformConfig1->setAttribute(Qt::WA_DeleteOnClose);
        connect(tformConfig1, &TFormConfig1::destroyed, this, &MainWindow::onTFormDestroyed);
    }
    tformConfig1->show();
}

void MainWindow::onTFormDestroyed(QObject *obj)
{
    if(obj == tform1)
    {
        tform1 = nullptr;
    }
    if(obj == tform7)
    {
        tform7 = nullptr;
    }
    if(obj == tformConfig1)
    {
        tformConfig1 = nullptr;
    }
    if(obj == tformCali)
    {
        tformCali = nullptr;
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    if(tform1 == nullptr)
    {
        tform1 = new TForm1(this);
        tform1->setAttribute(Qt::WA_DeleteOnClose);
        connect(tform1, &TForm1::destroyed, this, &MainWindow::onTFormDestroyed);
    }
    tform1->show();
}


void MainWindow::on_pushButton_8_clicked()
{
    if(tformCali == nullptr)
    {
        tformCali = new TFormCali(this);
        tformCali->setAttribute(Qt::WA_DeleteOnClose);
        connect(tformCali, &TFormCali::destroyed, this, &MainWindow::onTFormDestroyed);
    }
    tformCali->show();
}

void MainWindow::on_txResetTimer_timeout()
{
    //恢复灰色
    ui->lab_tx->setStyleSheet(
        "QLabel {"
        "    border-radius: 5px;"
        "    background-color: #D3D3D3;"
        "}"
        );
}

void MainWindow::on_rxResetTimer_timeout()
{
    //恢复灰色
    ui->lab_rx->setStyleSheet(
        "QLabel {"
        "    border-radius: 5px;"
        "    background-color: #D3D3D3;"
        "}"
        );
}

