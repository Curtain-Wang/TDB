#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
class QSerialPort;
class TForm1;
class QLineEdit;
class TFormConfig1;
class TFormConfig2;
class QLabel;
class TForm7;
class TForm3;
class TFormCali;
class TFormRecord;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void sendPortData(QByteArray data = nullptr);
    void sendSerialData(const QByteArray &data);
    QByteArray calculateCRCArray(const QByteArray &data, int length);
    void sendGetRealTimeDataCMD();
    void manualReadCMDBuild(quint16 startHigh, quint16 startLow, char numHigh, char numLow);
    void manualWriteOneCMDBuild(quint16 startHigh, quint16 startLow, char valueHigh, char valueLow, quint8 secFlag = 0);
    void cacheReceiveData();
    bool receiveDataCRCCheck(const QByteArray &data);
    void dealMessage(quint8* data);
    void refershData(quint8* data, quint16 length);
    QString getWarnText(quint16 value);
    QString getProtText(quint16 value);
    QString getWorkText(quint16 value);
    void refreshPort();
    void updateSaveDataInterval(int second);
    void initializeCSVFile(QTextStream &out);
    void writeDataToCSV(QTextStream &out, const QDateTime &currentTime);
private slots:
    void onSendTimerTimeout();
    void onReceiveTimerTimeout();
    void on_connBtn_2_clicked();
    void onTFormDestroyed(QObject *obj);
    void on_connBtn_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_saveDataTimer_timeout();

private:
    Ui::MainWindow *ui;
    TForm1* tform1 = nullptr;
    TFormConfig1* tformConfig1 = nullptr;
    TFormConfig2* tformConfig2 = nullptr;
    TForm3* tform3 = nullptr;
    TFormCali* tformCali = nullptr;
    TFormRecord* tformRecord = nullptr;
    QSerialPort* serialPort;
    QTimer* sendTimer = nullptr;
    QTimer* receiveTimer = nullptr;
    QTimer* saveDataTimer = nullptr;
    QLabel* connectStatusLabel;
    QLabel* runningStatusLabel;
    TForm7* tform7 = nullptr;
    //文件写入标签
    QFile csvFile;
};
#endif // MAINWINDOW_H
