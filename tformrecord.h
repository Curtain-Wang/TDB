#ifndef TFORMRECORD_H
#define TFORMRECORD_H

#include <QWidget>

namespace Ui {
class TFormRecord;
}

class TFormRecord : public QWidget
{
    Q_OBJECT

public:
    explicit TFormRecord(QWidget *parent = nullptr);
    ~TFormRecord();
    void refresh();

private slots:
    void on_pushButton_13_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::TFormRecord *ui;
};

#endif // TFORMRECORD_H
