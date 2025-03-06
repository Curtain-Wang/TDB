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
    void annalyzeData(quint8* data, quint16 length);
    void sendGetData2Cmd();
    void annalyzeOneData(quint16 addr, quint16 value);
private:
    Ui::TFormConfig2 *ui;
    MainWindow* mainwindow;
};

#endif // TFORMCONFIG2_H
