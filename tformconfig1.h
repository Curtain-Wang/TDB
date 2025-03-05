#ifndef TFORMCONFIG1_H
#define TFORMCONFIG1_H

#include <QWidget>
class MainWindow;
class MyLineEdit;
namespace Ui {
class TFormConfig1;
}

class TFormConfig1 : public QWidget
{
    Q_OBJECT

public:
    explicit TFormConfig1(QWidget *parent = nullptr);
    ~TFormConfig1();
    void init();
    void sendGetDataCmd();
    void annalyzeData(QByteArray data);
private:
    Ui::TFormConfig1 *ui;
    MainWindow* mainwindow;
    QHash<quint16, MyLineEdit*> addrEditHash;
    QHash<quint16, quint8> addrFormatHash;
};

#endif // TFORMCONFIG1_H
