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
    refresh();
}

TFormCali::~TFormCali()
{
    delete ui;
}

void TFormCali::refresh()
{
    ui->l15->setText(QString::number(static_cast<float>(regs[15] * 1.0/ qPow(10, pows[15])), 'f', pows[15]));
    ui->l16->setText(QString::number(static_cast<float>(regs[16] * 1.0/ qPow(10, pows[16])), 'f', pows[16]));
    ui->l17->setText(QString::number(static_cast<float>(regs[17] * 1.0/ qPow(10, pows[17])), 'f', pows[17]));
    ui->l18->setText(QString::number(static_cast<float>(regs[18] * 1.0/ qPow(10, pows[18])), 'f', pows[18]));
    ui->l19->setText(QString::number(static_cast<float>(regs[19] * 1.0/ qPow(10, pows[19])), 'f', pows[19]));
    ui->l20->setText(QString::number(static_cast<float>(regs[20] * 1.0/ qPow(10, pows[20])), 'f', pows[20]));
    ui->l21->setText(QString::number(static_cast<float>(regs[21] * 1.0/ qPow(10, pows[21])), 'f', pows[21]));
    ui->l22->setText(QString::number(static_cast<float>(regs[22] * 1.0/ qPow(10, pows[22])), 'f', pows[22]));
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
    quint16 addr = 53 + 1024;
    quint8 startHigh = ((addr >> 8) & 0xFF);
    quint8 startLow = (addr & 0xFF);
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
    quint16 addr = 54 + 1024;
    quint8 startHigh = ((addr >> 8) & 0xFF);
    quint8 startLow = (addr & 0xFF);
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
    quint16 addr = 55 + 1024;
    quint8 startHigh = ((addr >> 8) & 0xFF);
    quint8 startLow = (addr & 0xFF);
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
    quint16 addr =56 + 1024;
    quint8 startHigh = ((addr >> 8) & 0xFF);
    quint8 startLow = (addr & 0xFF);
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
    quint16 addr = 57 + 1024;
    quint8 startHigh = ((addr >> 8) & 0xFF);
    quint8 startLow = (addr & 0xFF);
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
    quint16 addr = 58 + 1024;
    quint8 startHigh = ((addr >> 8) & 0xFF);
    quint8 startLow = (addr & 0xFF);
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
    quint16 addr = 59 + 1024;
    quint8 startHigh = ((addr >> 8) & 0xFF);
    quint8 startLow = (addr & 0xFF);
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
    quint16 addr = 60 + 1024;
    quint8 startHigh = ((addr >> 8) & 0xFF);
    quint8 startLow = (addr & 0xFF);
    quint8 valueHigh = ((iValue >> 8) & 0xFF);
    quint8 valueLow = (iValue & 0xFF);
    mainwindow->manualWriteOneCMDBuild(startHigh, startLow, valueHigh, valueLow);
}


