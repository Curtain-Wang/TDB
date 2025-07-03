#ifndef GLOBALPARAM_H
#define GLOBALPARAM_H
#include <QHash>
#define NO_WARN_PROT_STR    "无告警、保护"
#define TITLE "DC48DC48I100-V1.0.0"
class QByteArray;
class QString;
class MainWindow;
class QLineEdit;
extern int waitMessageRemaingTime;
extern int dataRefreshRemaingTime;
//0未连接 1已连接
extern int connFlag;
//手动标记，0自动 1手动 2双手动
extern int manualFlag;
extern QByteArray manualSendDataBuf;
extern const char READ_CMD;
extern const char WRITE_ONE_CMD;
extern const char WRITE_MULTIPLE_CMD;
extern const int MODULE;
extern const int DATA_REFRESH_CYCLE;
extern QByteArray receiveDataBuf;
extern int receiveStartIndex;
extern int receiveEndIndex;
extern int lastStartAddr;
extern int mainAddrStart;
extern int lastEditAddr;
extern int config1AddrStart;
extern int config2AddrStart1;
extern int config2AddrStart2;
extern int kbAddrStart;
extern const int BR;
extern QString connStatus;
extern QString runningStatus;
extern MainWindow* mainwindow;
extern QHash<quint16, QLineEdit*> addrEditHash;
extern QHash<quint16, quint8> addrFormatHash;
extern QHash<quint16, quint8> addrSignHash;
extern quint16 reg1024Value;
extern qint16 reg1025Value;
extern quint16 reg1027Value;
extern qint16 reg1028Value;
#endif // GLOBALPARAM_H
