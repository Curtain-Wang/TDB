#include "tformconfig1.h"
#include "ui_tformconfig1.h"
#include "mainwindow.h"
#include "globalparam.h"

TFormConfig1::TFormConfig1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TFormConfig1)
    , mainwindow(qobject_cast<MainWindow*>(parent))
{
    ui->setupUi(this);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    refresh();
}

TFormConfig1::~TFormConfig1()
{
    delete ui;
}

void TFormConfig1::refresh()
{
    ui->l2->setText(QString::number(static_cast<float>(regs[2] * 1.0/ qPow(10, pows[2])), 'f', pows[2]));
    ui->l3->setText(QString::number(regs[3]));
    ui->l33->setText(QString::number(static_cast<float>(regs[33] * 1.0/ qPow(10, pows[33])), 'f', pows[33]));
    ui->l34->setText(QString::number(static_cast<float>(regs[34] * 1.0/ qPow(10, pows[34])), 'f', pows[34]));
    ui->l35->setText(QString::number(static_cast<float>(regs[35] * 1.0/ qPow(10, pows[35])), 'f', pows[35]));
    ui->l36->setText(QString::number(static_cast<float>(regs[36] * 1.0/ qPow(10, pows[36])), 'f', pows[36]));
    ui->l37->setText(QString::number(static_cast<float>(regs[37] * 1.0/ qPow(10, pows[37])), 'f', pows[37]));
    ui->l38->setText(QString::number(static_cast<float>(regs[38] * 1.0/ qPow(10, pows[38])), 'f', pows[38]));
    ui->l39->setText(QString::number(static_cast<float>(regs[39] * 1.0/ qPow(10, pows[39])), 'f', pows[39]));
    ui->l40->setText(QString::number(static_cast<float>(regs[40] * 1.0/ qPow(10, pows[40])), 'f', pows[40]));
    ui->l41->setText(QString::number(static_cast<float>(regs[41] * 1.0/ qPow(10, pows[41])), 'f', pows[41]));
    ui->l42->setText(QString::number(static_cast<float>(regs[42] * 1.0/ qPow(10, pows[42])), 'f', pows[42]));
    ui->l43->setText(QString::number(static_cast<float>(regs[43] * 1.0/ qPow(10, pows[43])), 'f', pows[43]));
    ui->l44->setText(QString::number(static_cast<float>(regs[44] * 1.0/ qPow(10, pows[44])), 'f', pows[44]));
    ui->l45->setText(QString::number(static_cast<float>(regs[45] * 1.0/ qPow(10, pows[45])), 'f', pows[45]));
    ui->l46->setText(QString::number(static_cast<float>(regs[46] * 1.0/ qPow(10, pows[46])), 'f', pows[46]));
    ui->l47->setText(QString::number(static_cast<float>(regs[47] * 1.0/ qPow(10, pows[47])), 'f', pows[47]));
    ui->l48->setText(QString::number(static_cast<float>(regs[48] * 1.0/ qPow(10, pows[48])), 'f', pows[48]));
    ui->l49->setText(QString::number(static_cast<float>(regs[49] * 1.0/ qPow(10, pows[49])), 'f', pows[49]));
    ui->l50->setText(QString::number(static_cast<float>(regs[50] * 1.0/ qPow(10, pows[50])), 'f', pows[50]));
    ui->l51->setText(QString::number(static_cast<float>(regs[51] * 1.0/ qPow(10, pows[51])), 'f', pows[51]));
    ui->l52->setText(QString::number(static_cast<float>(regs[52] * 1.0/ qPow(10, pows[52])), 'f', pows[52]));
}
