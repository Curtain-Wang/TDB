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
    void annalyzeData(quint8* data, quint16 length);
    void annalyzeOneData(quint16 addr, quint16 value);
private:
    Ui::TFormConfig1 *ui;
    MainWindow* mainwindow;
};

#endif // TFORMCONFIG1_H
