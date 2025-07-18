#include "tform3.h"
#include "ui_tform3.h"
#include <QMessageBox>
#include "globalparam.h"
#include "mainwindow.h"

TForm3::TForm3(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TForm3)
{
    ui->setupUi(this);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    init();
}

TForm3::~TForm3()
{
    delete ui;
}

void TForm3::init()
{
    //寄存器地址和LineEdit映射
    addrEditHash[48] = ui->d48;
    addrEditHash[49] = ui->d49;
    addrEditHash[52] = ui->d52;
    addrEditHash[53] = ui->d53;
    addrEditHash[56] = ui->d56;
    addrEditHash[57] = ui->d57;
    addrEditHash[60] = ui->d60;
    addrEditHash[61] = ui->d61;
    //寄存器地址和小数位映射
    addrFormatHash[48] = 0;
    addrFormatHash[49] = 3;
    addrFormatHash[52] = 0;
    addrFormatHash[53] = 3;
    addrFormatHash[56] = 0;
    addrFormatHash[57] = 3;
    addrFormatHash[60] = 0;
    addrFormatHash[61] = 3;
    //寄存器地址和符号映射
    addrSignHash[53] = 0;
    addrSignHash[61] = 0;
    addrSignHash[49] = 0;
    addrSignHash[57] = 0;
    addrSignHash[52] = 1;
    addrSignHash[60] = 1;
    addrSignHash[48] = 1;
    addrSignHash[56] = 1;
    //发送获取校准系数
    sendGetDataCmd();
    //一进页面不能修改KB系数
    ui->d48->blockSignals(true);
    ui->d49->blockSignals(true);
    ui->d52->blockSignals(true);
    ui->d53->blockSignals(true);
    ui->d56->blockSignals(true);
    ui->d57->blockSignals(true);
    ui->d60->blockSignals(true);
    ui->d61->blockSignals(true);
}

void TForm3::sendGetDataCmd()
{
    mainwindow->manualReadCMDBuild(0x00, 0x30, 0x00, 0x0E);
    lastStartAddr = 48;
}

void TForm3::annalyzeData(quint8 *data, quint16 length)
{
    for (int i = 0; i < length; i += 2)
    {
        quint16 addr = lastStartAddr + i / 2;
        quint16 high = data[i];
        quint16 low = data[i + 1];
        quint16 value = ((high << 8) | low);
        annalyzeOneData(addr, value);
    }
}

void TForm3::annalyzeOneData(quint16 addr, quint16 value)
{
    if(addrEditHash.value(addr, nullptr) == nullptr)
    {
        return;
    }
    if(addrSignHash[addr])
    {
        //有符号
        qint16 sValue = value;
        addrEditHash[addr]->setText(QString::number(static_cast<float>(sValue * 1.0/ qPow(10, addrFormatHash[addr])), 'f', addrFormatHash[addr]));
    }else
    {
        addrEditHash[addr]->setText(QString::number(static_cast<float>(value * 1.0/ qPow(10, addrFormatHash[addr])), 'f', addrFormatHash[addr]));
    }
}

void TForm3::refreshRealTimeData()
{
    ui->d21->setText(QString::number(static_cast<float>(reg1027Value * 1.0 / qPow(10, addrFormatHash[1027])), 'f', addrFormatHash[1027]));
    ui->d23->setText(QString::number(static_cast<float>(reg1028Value * 1.0 / qPow(10, addrFormatHash[1028])), 'f', addrFormatHash[1028]));
    ui->d20->setText(QString::number(static_cast<float>(reg1024Value * 1.0 / qPow(10, addrFormatHash[1024])), 'f', addrFormatHash[1024]));
    ui->d22->setText(QString::number(static_cast<float>(reg1024Value * 1.0 / qPow(10, addrFormatHash[1025])), 'f', addrFormatHash[1025]));
}

void TForm3::on_lineEdit_13_returnPressed()
{
    if(ui->lineEdit_13->text() == "fc123")
    {
        QMessageBox::information(this, "提示", "密码正确!");
        ui->d48->blockSignals(false);
        ui->d49->blockSignals(false);
        ui->d52->blockSignals(false);
        ui->d53->blockSignals(false);
        ui->d56->blockSignals(false);
        ui->d57->blockSignals(false);
        ui->d60->blockSignals(false);
        ui->d61->blockSignals(false);
    }else
    {
        QMessageBox::information(this, "提示", "密码错误!");
    }
}

