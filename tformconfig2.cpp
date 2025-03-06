#include "tformconfig2.h"
#include "ui_tformconfig2.h"
#include "mainwindow.h"
#include "globalparam.h"
TFormConfig2::TFormConfig2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TFormConfig2)
    , mainwindow(qobject_cast<MainWindow*>(parent))
{
    ui->setupUi(this);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    init();
}

TFormConfig2::~TFormConfig2()
{
    delete ui;
}

void TFormConfig2::init()
{
    //寄存器地址和LineEdit映射
    addrEditHash[1318] = ui->l1318;
    addrEditHash[1321] = ui->l1321;
    addrEditHash[1322] = ui->l1322;
    addrEditHash[1323] = ui->l1323;
    addrEditHash[1326] = ui->l1326;
    addrEditHash[1327] = ui->l1327;
    addrEditHash[1328] = ui->l1328;
    addrEditHash[1329] = ui->l1329;
    addrEditHash[1330] = ui->l1330;
    addrEditHash[1336] = ui->l1336;
    addrEditHash[1337] = ui->l1337;
    addrEditHash[1531] = ui->l1531;
    addrEditHash[1532] = ui->l1532;
    //寄存器地址和小数位映射
    addrFormatHash[1318] = 2;
    addrFormatHash[1321] = 2;
    addrFormatHash[1322] = 0;
    addrFormatHash[1323] = 0;
    addrFormatHash[1326] = 2;
    addrFormatHash[1327] = 0;
    addrFormatHash[1328] = 2;
    addrFormatHash[1329] = 0;
    addrFormatHash[1330] = 0;
    addrFormatHash[1336] = 2;
    addrFormatHash[1337] = 2;
    addrFormatHash[1531] = 0;
    addrFormatHash[1532] = 0;
    //寄存器地址和符号映射
    addrSignHash[1318] = 0;
    addrSignHash[1321] = 1;
    addrSignHash[1322] = 0;
    addrSignHash[1323] = 0;
    addrSignHash[1326] = 1;
    addrSignHash[1327] = 0;
    addrSignHash[1328] = 1;
    addrSignHash[1329] = 0;
    addrSignHash[1330] = 0;
    addrSignHash[1336] = 1;
    addrSignHash[1337] = 1;
    addrSignHash[1531] = 0;
    addrSignHash[1532] = 0;
    sendGetData1Cmd();
}

void TFormConfig2::sendGetData1Cmd()
{
    mainwindow->manualReadCMDBuild(0x05, 0x26, 0x00, 0x14);
    lastStartAddr = 1318;
}

void TFormConfig2::annalyzeData(quint8* data, quint16 length)
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

void TFormConfig2::sendGetData2Cmd()
{
    mainwindow->manualReadCMDBuild(0x05, 0xFB, 0x00, 0x02);
    lastStartAddr = 1531;
}

void TFormConfig2::annalyzeOneData(quint16 addr, quint16 value)
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

