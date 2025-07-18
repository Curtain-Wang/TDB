#ifndef TFORMCALI_H
#define TFORMCALI_H

#include <QWidget>

namespace Ui {
class TFormCali;
}

class TFormCali : public QWidget
{
    Q_OBJECT

public:
    explicit TFormCali(QWidget *parent = nullptr);
    ~TFormCali();
    void init();
    void annalyzeData(quint8* data, quint16 length);
    void annalyzeOneData(quint16 addr, quint16 value);
    void sendGetDataCmd();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

private:
    Ui::TFormCali *ui;

};

#endif // TFORMCALI_H
