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
    void refresh();
private slots:

    void on_c35_currentIndexChanged(int index);

private:
    Ui::TFormConfig1 *ui;
    MainWindow* mainwindow;
    quint8 preModeIndex;
};

#endif // TFORMCONFIG1_H
