#include "tform7.h"
#include "ui_tform7.h"
#include "globalparam.h"
#include <QMessageBox>
#include "mainwindow.h"
#include <QDir>
#include <QSettings>
TForm7::TForm7(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TForm7)
{
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    ui->setupUi(this);
}

TForm7::~TForm7()
{
    delete ui;
}


void TForm7::on_lineEdit_returnPressed()
{
    if(connFlag == 0)
    {
        QMessageBox::information(this, tr("提示"), tr("请先建立连接!"));
        return;
    }

    float fValue = ui->lineEdit->text().toFloat();
    quint16 iValue= (quint16)(fValue * qPow(10, pows[lastEditAddr]) + 0.5);
    quint8 startHigh = (((lastEditAddr + 1024) >> 8) & 0xFF);
    quint8 startLow = ((lastEditAddr + 1024) & 0xFF);
    quint8 valueHigh = ((iValue >> 8) & 0xFF);
    quint8 valueLow = (iValue & 0xFF);
    mainwindow->manualWriteOneCMDBuild(startHigh, startLow, valueHigh, valueLow);
    this->close();
}

