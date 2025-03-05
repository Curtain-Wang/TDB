#ifndef TFORMCONFIG2_H
#define TFORMCONFIG2_H

#include <QWidget>
class MainWindow;
class MyLineEdit;
namespace Ui {
class TFormConfig2;
}

class TFormConfig2 : public QWidget
{
    Q_OBJECT

public:
    explicit TFormConfig2(QWidget *parent = nullptr);
    ~TFormConfig2();
    void init();
    void sendGetData1Cmd();
    void annalyzeData(QByteArray data);
    void sendGetData2Cmd();
private:
    Ui::TFormConfig2 *ui;
    MainWindow* mainwindow;
    QHash<quint16, MyLineEdit*> addrEditHash;
    QHash<quint16, quint8> addrFormatHash;
};

#endif // TFORMCONFIG2_H
