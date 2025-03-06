#ifndef TFORM3_H
#define TFORM3_H

#include <QWidget>
class QLineEdit;
class MainWindow;
namespace Ui {
class TForm3;
}

class TForm3 : public QWidget
{
    Q_OBJECT

public:
    explicit TForm3(QWidget *parent = nullptr);
    ~TForm3();
    void init();
    void sendGetDataCmd();
    void annalyzeData(quint8* data, quint16 length);
    void annalyzeOneData(quint16 addr, quint16 value);
    void refreshRealTimeData();
private slots:
    void on_lineEdit_13_returnPressed();

private:
    Ui::TForm3 *ui;
};

#endif // TFORM3_H
