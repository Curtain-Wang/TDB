#include "tformconfig1.h"
#include "ui_tformconfig1.h"
#include "mainwindow.h"
#include "globalparam.h"

TFormConfig1::TFormConfig1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TFormConfig1)
    , mainwindow(qobject_cast<MainWindow*>(parent))
{
    ui->setupUi(this);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    init();
}

TFormConfig1::~TFormConfig1()
{
    delete ui;
}

void TFormConfig1::init()
{
    //寄存器地址和LineEdit映射
    addrEditHash[1280] = ui->l1280;
    addrEditHash[1281] = ui->l1281;
    addrEditHash[1284] = ui->l1284;
    addrEditHash[1290] = ui->l1290;
    addrEditHash[1291] = ui->l1291;
    addrEditHash[1292] = ui->l1292;
    addrEditHash[1293] = ui->l1293;
    addrEditHash[1294] = ui->l1294;
    addrEditHash[1295] = ui->l1295;
    addrEditHash[1296] = ui->l1296;
    addrEditHash[1301] = ui->l1301;
    addrEditHash[1305] = ui->l1305;
    addrEditHash[1306] = ui->l1306;
    addrEditHash[1309] = ui->l1309;
    addrEditHash[1310] = ui->l1310;
    addrEditHash[1313] = ui->l1313;
    addrEditHash[1314] = ui->l1314;
    addrEditHash[1317] = ui->l1317;
    //寄存器地址和小数位映射
    addrFormatHash[1280] = 0;
    addrFormatHash[1281] = 2;
    addrFormatHash[1284] = 0;
    addrFormatHash[1290] = 2;
    addrFormatHash[1291] = 2;
    addrFormatHash[1292] = 0;
    addrFormatHash[1293] = 2;
    addrFormatHash[1294] = 2;
    addrFormatHash[1295] = 2;
    addrFormatHash[1296] = 2;
    addrFormatHash[1301] = 2;
    addrFormatHash[1305] = 2;
    addrFormatHash[1306] = 2;
    addrFormatHash[1309] = 2;
    addrFormatHash[1310] = 2;
    addrFormatHash[1313] = 2;
    addrFormatHash[1314] = 2;
    addrFormatHash[1317] = 2;
    //寄存器地址和符号映射
    addrSignHash[1280] = 0;
    addrSignHash[1281] = 1;
    addrSignHash[1284] = 1;
    addrSignHash[1290] = 0;
    addrSignHash[1291] = 1;
    addrSignHash[1292] = 0;
    addrSignHash[1293] = 0;
    addrSignHash[1294] = 1;
    addrSignHash[1295] = 0;
    addrSignHash[1296] = 0;
    addrSignHash[1301] = 0;
    addrSignHash[1305] = 0;
    addrSignHash[1306] = 0;
    addrSignHash[1309] = 0;
    addrSignHash[1310] = 0;
    addrSignHash[1313] = 0;
    addrSignHash[1314] = 0;
    addrSignHash[1317] = 0;
    sendGetDataCmd();
}

void TFormConfig1::sendGetDataCmd()
{
    mainwindow->manualReadCMDBuild(0x05, 0x00, 0x00, 0x26);
    lastStartAddr = 1280;
}

void TFormConfig1::annalyzeData(quint8* data, quint16 length)
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

void TFormConfig1::annalyzeOneData(quint16 addr, quint16 value)
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
