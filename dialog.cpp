#include "dialog.h"
#include "./ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

QJsonDocument Dialog::readJson()
{
    QProcessEnvironment env;
    QString homeDir = env.systemEnvironment().value("HOME") + '/';
    QFile file(homeDir + ".config/swaykbd-settings");
    QJsonDocument jsonDoc;
    if(file.exists()) {
        file.open(QFile::OpenModeFlag::ReadOnly);
        QString jsonStr= file.readAll();
        file.close();
        jsonDoc.fromJson(jsonStr.toLocal8Bit());
    }
    else {
    }
}

