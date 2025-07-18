#include "tformcali.h"
#include "ui_tformcali.h"
#include "globalparam.h"
#include <QMessageBox>
#include "mainwindow.h"

TFormCali::TFormCali(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TFormCali)
{
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    init();
}

TFormCali::~TFormCali()
{
    delete ui;
}

void TFormCali::init()
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
}

void TFormCali::annalyzeData(quint8 *data, quint16 length)
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

void TFormCali::annalyzeOneData(quint16 addr, quint16 value)
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

void TFormCali::on_pushButton_2_clicked()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }

    float fValue = ui->lineEdit_2->text().toFloat();
    quint16 iValue= (quint16)(fValue * 100 + 0.5);
    quint8 startHigh = ((64 >> 8) & 0xFF);
    quint8 startLow = (64 & 0xFF);
    quint8 valueHigh = ((iValue >> 8) & 0xFF);
    quint8 valueLow = (iValue & 0xFF);
    mainwindow->manualWriteOneCMDBuild(startHigh, startLow, valueHigh, valueLow);
}


void TFormCali::on_pushButton_3_clicked()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }

    float fValue = ui->lineEdit_3->text().toFloat();
    quint16 iValue= (quint16)(fValue * 100 + 0.5);
    quint8 startHigh = ((65 >> 8) & 0xFF);
    quint8 startLow = (65 & 0xFF);
    quint8 valueHigh = ((iValue >> 8) & 0xFF);
    quint8 valueLow = (iValue & 0xFF);
    mainwindow->manualWriteOneCMDBuild(startHigh, startLow, valueHigh, valueLow);
}


void TFormCali::on_pushButton_4_clicked()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }

    float fValue = ui->lineEdit_4->text().toFloat();
    quint16 iValue= (quint16)(fValue * 100 + 0.5);
    quint8 startHigh = ((66 >> 8) & 0xFF);
    quint8 startLow = (66 & 0xFF);
    quint8 valueHigh = ((iValue >> 8) & 0xFF);
    quint8 valueLow = (iValue & 0xFF);
    mainwindow->manualWriteOneCMDBuild(startHigh, startLow, valueHigh, valueLow);
}


void TFormCali::on_pushButton_5_clicked()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }

    float fValue = ui->lineEdit_5->text().toFloat();
    quint16 iValue= (quint16)(fValue * 100 + 0.5);
    quint8 startHigh = ((67 >> 8) & 0xFF);
    quint8 startLow = (67 & 0xFF);
    quint8 valueHigh = ((iValue >> 8) & 0xFF);
    quint8 valueLow = (iValue & 0xFF);
    mainwindow->manualWriteOneCMDBuild(startHigh, startLow, valueHigh, valueLow);
}


void TFormCali::on_pushButton_6_clicked()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }

    float fValue = ui->lineEdit_6->text().toFloat();
    quint16 iValue= (quint16)(fValue * 100 + 0.5);
    quint8 startHigh = ((68 >> 8) & 0xFF);
    quint8 startLow = (68 & 0xFF);
    quint8 valueHigh = ((iValue >> 8) & 0xFF);
    quint8 valueLow = (iValue & 0xFF);
    mainwindow->manualWriteOneCMDBuild(startHigh, startLow, valueHigh, valueLow);
}


void TFormCali::on_pushButton_7_clicked()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }

    float fValue = ui->lineEdit_7->text().toFloat();
    quint16 iValue= (quint16)(fValue * 100 + 0.5);
    quint8 startHigh = ((69 >> 8) & 0xFF);
    quint8 startLow = (69 & 0xFF);
    quint8 valueHigh = ((iValue >> 8) & 0xFF);
    quint8 valueLow = (iValue & 0xFF);
    mainwindow->manualWriteOneCMDBuild(startHigh, startLow, valueHigh, valueLow);
}


void TFormCali::on_pushButton_8_clicked()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }

    float fValue = ui->lineEdit_8->text().toFloat();
    quint16 iValue= (quint16)(fValue * 100 + 0.5);
    quint8 startHigh = ((70 >> 8) & 0xFF);
    quint8 startLow = (70 & 0xFF);
    quint8 valueHigh = ((iValue >> 8) & 0xFF);
    quint8 valueLow = (iValue & 0xFF);
    mainwindow->manualWriteOneCMDBuild(startHigh, startLow, valueHigh, valueLow);
}


void TFormCali::on_pushButton_9_clicked()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }

    float fValue = ui->lineEdit_9->text().toFloat();
    quint16 iValue= (quint16)(fValue * 100 + 0.5);
    quint8 startHigh = ((71 >> 8) & 0xFF);
    quint8 startLow = (71 & 0xFF);
    quint8 valueHigh = ((iValue >> 8) & 0xFF);
    quint8 valueLow = (iValue & 0xFF);
    mainwindow->manualWriteOneCMDBuild(startHigh, startLow, valueHigh, valueLow);
}

void TFormCali::sendGetDataCmd()
{
    mainwindow->manualReadCMDBuild(0x00, 0x30, 0x00, 0x0E);
    lastStartAddr = 48;
}

