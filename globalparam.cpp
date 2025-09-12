#include "globalparam.h"
#include <QByteArray>
#include <QString>
#include <mainwindow.h>
#include <QLineEdit>
int dischargeLimitI = 0;
int dischargeIAdjV = 0;
int waitMessageRemaingTime = 0;
int curLimitInterval = 10;
int dataRefreshRemaingTime = 0;
//手动标记，0自动 1手动 2双手动
int manualFlag = 0;
int connFlag = 0;
QByteArray manualSendDataBuf;
const char READ_CMD = 0x03;
const char WRITE_ONE_CMD = 0x06;
const char WRITE_MULTIPLE_CMD = 0x10;
const int MODULE = 0xFF;
const int DATA_REFRESH_CYCLE = 20;
QByteArray receiveDataBuf(500, 0);
int receiveStartIndex = 0;
int receiveEndIndex = 0;
int lastStartAddr = 0x00;
int mainAddrStart = 1024;
int config1AddrStart = 1280;
int config2AddrStart1 = 1318;
int config2AddrStart2 = 1531;
int lastEditAddr = 0;
int kbAddrStart = 48;
const int BR = 9600;
QString connStatus = "连接状态：%1";
QString runningStatus = "运行模式：%1";
MainWindow* mainwindow = nullptr;
QHash<quint16, QLineEdit*> addrEditHash;
QHash<quint16, quint8> addrFormatHash;
QHash<quint16, quint8> addrSignHash;
quint16 reg1024Value = 0;
qint16 reg1025Value = 0;
quint16 reg1027Value = 0;
qint16 reg1028Value = 0;
qint16 reg1036Value = -1;
quint16 curLimitInitFlag = 0;
