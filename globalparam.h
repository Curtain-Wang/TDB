#ifndef GLOBALPARAM_H
#define GLOBALPARAM_H
class QByteArray;
class QString;
class MainWindow;
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
extern int config1AddrStart;
extern int config2AddrStart1;
extern int config2AddrStart2;
extern const int BR;
extern QString connStatus;
extern QString runningStatus;
extern MainWindow* mainwindow;
#endif // GLOBALPARAM_H
