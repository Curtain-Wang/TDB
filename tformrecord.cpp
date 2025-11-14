#include "tformrecord.h"
#include "ui_tformrecord.h"
#include "QSettings"
#include "QFile"
#include "QDir"
#include "globalparam.h"
#include "mainwindow.h"
#include "QFileDialog"
#include <QMessageBox>
TFormRecord::TFormRecord(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TFormRecord)
{
    ui->setupUi(this);
    //设置窗口标志，确保有边框和标题栏、最小化、关闭，最大化
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    refresh();
}

TFormRecord::~TFormRecord()
{
    delete ui;
}

void TFormRecord::refresh()
{
    if(dataRecordCycle == -1 || dataRecordFilePath == "")
    {
        //加载数据记录文件保存地址和数据记录周期
        if(!QFile::exists(CONFIG_FILE_PATH))
        {
            QString configFilePath = CONFIG_FILE_PATH;
            QSettings settings(configFilePath, QSettings::IniFormat);
            // 设置默认值
            settings.beginGroup(DATA_RECORD_CONFIG);
            settings.setValue(DATA_RECORD_FILE_PATH, QDir::currentPath());
            settings.setValue(DATA_RECORD_CYCLE, DEFAULT_DATA_RECORD_CYCLE);
            settings.endGroup();
        }
        QSettings settings(CONFIG_FILE_PATH, QSettings::IniFormat);
        settings.beginGroup(DATA_RECORD_CONFIG);
        dataRecordCycle = settings.value(DATA_RECORD_CYCLE, DEFAULT_DATA_RECORD_CYCLE).toInt();
        dataRecordFilePath = settings.value(DATA_RECORD_FILE_PATH, QDir::currentPath()).toString();
    }
    ui->lineEdit_5->setText(QString::number(dataRecordCycle));
    ui->lineEdit_6->setText(dataRecordFilePath);
}

void TFormRecord::on_pushButton_13_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, "选择目录", "");
    if (!directory.isEmpty()) {
        ui->lineEdit_6->setText(directory);
    }
}


void TFormRecord::on_pushButton_6_clicked()
{
    dataRecordCycle = ui->lineEdit_5->text().toInt();
    dataRecordFilePath = ui->lineEdit_6->text();
    mainwindow->updateSaveDataInterval(dataRecordCycle);
    //更新配置文件
    QSettings settings(CONFIG_FILE_PATH, QSettings::IniFormat);
    settings.beginGroup(DATA_RECORD_CONFIG);
    settings.setValue(DATA_RECORD_FILE_PATH, dataRecordFilePath);
    settings.setValue(DATA_RECORD_CYCLE, dataRecordCycle);
    settings.endGroup();
    QMessageBox::information(this, tr("提示")
                             , tr("保存成功"));
}

