#ifndef GLOBALPARAM_H
#define GLOBALPARAM_H
#include <QHash>
#define NO_WARN_PROT_STR    "无告警、保护"
#define TITLE "DC100DC500P10K-V1.0.4"
#define RED_BUTTON_STYLE                "QPushButton { background-color: #EF5350; border: 2px solid #E53935; color: white; font-size: 22px; padding: 10px; border-radius: 10px; width: 100px; height: 50px; text-align: center; } QPushButton:hover { background-color: #e14a47; border: 2px solid #D32F2F; } QPushButton:pressed { background-color: #E53935; border: 2px solid #B71C1C; } QPushButton:checked { background-color: #E53935; border: 2px solid #B71C1C; color: #FFEBEE; box-shadow: 0 0 8px rgba(239, 83, 80, 0.6); font-weight: bold; }"
#define GREEN_BUTTON_STYLE              "QPushButton { background-color: #66BB6A; border: 2px solid #43A047; color: white; font-size: 22px; padding: 10px; border-radius: 10px; width: 100px; height: 50px; text-align: center; } QPushButton:hover { background-color: #5AAE5E; border: 2px solid #388E3C; } QPushButton:pressed { background-color: #4CAF50; border: 2px solid #2C6E2E; } QPushButton:checked { background-color: #4CAF50; border: 2px solid #2E7D32; color: #E8F5E9; box-shadow: 0 0 8px rgba(76, 175, 80, 0.6); font-weight: bold; }"
#define REG_ADDR_OFFSET   1024


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
extern qint16 regs[100];
extern quint8 pows[61];
extern quint8 sign[61];
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
extern quint16 reg1024Value;
extern qint16 reg1025Value;
extern quint16 reg1027Value;
extern qint16 reg1028Value;
#endif // GLOBALPARAM_H
