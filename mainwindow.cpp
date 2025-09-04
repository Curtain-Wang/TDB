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
#include "tformconfig2.h"
#include "tform7.h"
#include "tform3.h"
#include "tformcali.h"
#include <cmath>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , serialPort(new QSerialPort(this))
    , connectStatusLabel(new QLabel(this))
    , runningStatusLabel(new QLabel(this))
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

    //寄存器地址和LineEdit映射
    addrEditHash[1024] = ui->lineEdit_0;
    addrEditHash[1025] = ui->lineEdit_1;
    addrEditHash[1026] = ui->lineEdit_2;
    addrEditHash[1027] = ui->lineEdit_3;
    addrEditHash[1028] = ui->lineEdit_4;
    addrEditHash[1029] = ui->lineEdit_5;
    addrEditHash[1030] = ui->lineEdit_6;
    addrEditHash[1031] = ui->lineEdit_7;
    addrEditHash[1032] = ui->lineEdit_8;

    addrFormatHash[1024] = 2;
    addrFormatHash[1025] = 2;
    addrFormatHash[1026] = 0;
    addrFormatHash[1027] = 2;
    addrFormatHash[1028] = 2;
    addrFormatHash[1029] = 1;
    addrFormatHash[1030] = 1;
    addrFormatHash[1031] = 1;
    addrFormatHash[1032] = 1;

    addrSignHash[1024] = 0;
    addrSignHash[1025] = 1;
    addrSignHash[1026] = 1;
    addrSignHash[1027] = 0;
    addrSignHash[1028] = 1;
    addrSignHash[1029] = 1;
    addrSignHash[1030] = 1;
    addrSignHash[1031] = 1;
    addrSignHash[1032] = 1;
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
        //主页的数据
        if(lastStartAddr == mainAddrStart)
        {
            refershData(&data[3], data[2]);  // 传递左值
        }else if(lastStartAddr == config1AddrStart)//配置界面1
        {
            if(tformConfig1 != nullptr)
            {
                tformConfig1->annalyzeData(&data[3], data[2]);
            }
        }else if(lastStartAddr == config2AddrStart1)//配置界面2第一段
        {
            if(tformConfig2 != nullptr)
            {
                tformConfig2->annalyzeData(&data[3], data[2]);
            }
            //第一段数据收到之后，发第二段命令
            tformConfig2->sendGetData2Cmd();
        }else if(lastStartAddr == config2AddrStart2)//配置界面2第二段
        {
            if(tformConfig2 != nullptr)
            {
                tformConfig2->annalyzeData(&data[3], data[2]);
            }
        }else if(lastStartAddr == 48)//校准界面
        {
            if(tformCali != nullptr)
            {
                tformCali->annalyzeData(&data[3], data[2]);
            }
        }
    }
    else if(data[1] == WRITE_ONE_CMD)
    {
        //写入命令返回，立刻回显
        quint16 addr = ((data[2] << 8) | data[3]);
        quint16 value = ((data[4] << 8) | data[5]);
        if(addr < config2AddrStart1 && addr >= config1AddrStart)
        {
            if(tformConfig1 != nullptr)
            {
                tformConfig1->annalyzeOneData(addr, value);
            }
        }else if(addr >= config2AddrStart1)
        {
            if(tformConfig2 != nullptr)
            {
                tformConfig2->annalyzeOneData(addr, value);
            }
        }else if(addr < config1AddrStart)
        {
            if(tformCali != nullptr)
            {
                tformCali->sendGetDataCmd();
            }
            QMessageBox::information(this, tr("提示"), tr("写入成功!"));
        }

    }
    else if(data[1] == WRITE_MULTIPLE_CMD)
    {
        //多个写入命令返回，暂时没有处理逻辑、
    }
}

void MainWindow::refershData(quint8* data, quint16 length)
{
    for (int i = 0; i < length - 6; i += 2)
    {
        quint16 addr = lastStartAddr + i / 2;
        if(addr == 1026)
        {
            continue;
        }
        if(addrEditHash.value(addr, nullptr) == nullptr)
        {
            continue;
        }
        quint16 high = data[i];
        quint16 low = data[i + 1];
        if(addrSignHash[addr])
        {
            //有符号
            qint16 value = ((high << 8) | low);
            addrEditHash[addr]->setText(QString::number(static_cast<float>(value * 1.0/ qPow(10, addrFormatHash[addr])), 'f', addrFormatHash[addr]));
        }else
        {
            //无符号
            quint16 value = ((high << 8) | low);
            addrEditHash[addr]->setText(QString::number(static_cast<float>(value * 1.0/ qPow(10, addrFormatHash[addr])), 'f', addrFormatHash[addr]));

        }
    }
    quint16 warnValue = ((data[length - 8] << 8) | data[length - 7]);
    quint16 protValue = ((data[length - 6] << 8) | data[length - 5]);
    QString warnPortStr = getWarnText(warnValue).append(getProtText(protValue));
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


    quint16 workValue = ((data[length - 2] << 8) | data[length - 1]);
    ui->label_39->setText(getWorkText(workValue));
    reg1024Value = ((data[0] << 8) | data[1]);
    reg1025Value = ((data[2] << 8) | data[3]);
    reg1027Value = ((data[6] << 8) | data[7]);
    reg1028Value = ((data[8] << 8) | data[9]);
    if(tform3 != nullptr)
    {
       tform3->refreshRealTimeData();
    }
    //电池端总功率计算
    float val1 = ui->lineEdit_3->text().toFloat();
    float val2 = ui->lineEdit_4->text().toFloat();
    float result = val1 * val2;
    ui->lineEdit_9->setText(QString::number(result, 'f', 0));
    //充电总功率计算
    val1 = ui->lineEdit_0->text().toFloat();
    val2 = ui->lineEdit_1->text().toFloat();
    result = val1 * val2;
    ui->lineEdit_2->setText(QString::number(result, 'f', 0));
    //转换效率计算
    val1 = ui->lineEdit_2->text().toFloat();
    val2 = ui->lineEdit_9->text().toFloat();
    result = std::abs(val1) < std::abs(val2) ? val1 * 100 / val2 : val2 * 100 / val1;
    ui->lineEdit_10->setText(QString::number(result, 'f', 2));
}

QString MainWindow::getWarnText(quint16 value)
{
    QString text;
    if((value & 1) == 1)
    {
        text.append(" 充放电端总电压过压告警");
    }
    if(((value >> 1) & 1) == 1)
    {
        text.append(" 充放电端总电压低压告警");
    }
    if(((value >> 2) & 1) == 1)
    {
        text.append(" 电池总压过压告警");
    }
    if(((value >> 3) & 1) == 1)
    {
        text.append(" 电池总压欠压告警");
    }
    if(((value >> 4) & 1) == 1)
    {
        text.append(" 充电过流告警");
    }
    if(((value >> 5) & 1) == 1)
    {
        text.append(" 放电过流告警");
    }
    if(((value >> 8) & 1) == 1)
    {
        text.append(" 充电MOS管高温告警");
    }
    if(((value >> 9) & 1) == 1)
    {
        text.append(" 放电MOS管高温告警");
    }
    if(((value >> 10) & 1) == 1)
    {
        text.append(" 环境温度高告警");
    }
    if(((value >> 12) & 1) == 1)
    {
        text.append(" 电源反接告警");
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
        text.append(" 初始化");
        return text;
    }
    if((value & 1) == 1)
    {
        text.append(" BUCK限流充电");
    }
    if(((value >> 1) & 1) == 1)
    {
        text.append(" 直通充电");
    }
    if(((value >> 2) & 1) == 1)
    {
        text.append(" 直通放电");
    }
    if(((value >> 3) & 1) == 1)
    {
        text.append(" BUCK限流放电");
    }
    if(((value >> 4) & 1) == 1)
    {
        text.append(" BOOST限流放电");
    }
    if(((value >> 5) & 1) == 1)
    {
        text.append(" 静置待机");
    }
    if(((value >> 6) & 1) == 1)
    {
        text.append(" 保护状态");
    }
    if(((value >> 8) & 1) == 1)
    {
        text.append(" 继电器导通");
    }
    if(((value >> 9) & 1) == 1)
    {
        text.append(" 放电主MOS管开启");
    }
    if(((value >> 10) & 1) == 1)
    {
        text.append(" 充电主MOS管开启");
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
    buf.append(static_cast<char>(0x0D));
    QByteArray crcArray = calculateCRCArray(buf, 6);
    //crC
    buf.append(crcArray[0]);
    buf.append(crcArray[1]);
    lastStartAddr = 1024;
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
    if(obj == tformConfig2)
    {
        tformConfig2 = nullptr;
    }
    if(obj == tform3)
    {
        tform3 = nullptr;
    }
    if(obj == tformCali)
    {
        tformCali = nullptr;
    }
}


void MainWindow::on_pushButton_5_clicked()
{
    if(tformConfig2 == nullptr)
    {
        tformConfig2 = new TFormConfig2(this);
        tformConfig2->setAttribute(Qt::WA_DeleteOnClose);
        connect(tformConfig2, &TFormConfig2::destroyed, this, &MainWindow::onTFormDestroyed);
    }
    tformConfig2->show();
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

