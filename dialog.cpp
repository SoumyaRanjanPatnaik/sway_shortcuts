#include "dialog.h"
#include "./ui_dialog.h"
#include <QDir>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QProcessEnvironment env;
    QString homeDir = env.systemEnvironment().value("HOME") + '/';
    configDir = homeDir + ".config/swaykbd-settings/";
    configLoc = configDir + configFilename;
    configFile.setFileName(configLoc);
    readJson();
}

Dialog::~Dialog()
{
    delete ui;
}

// Read JSON from config file
void Dialog::readJson()
{
    // Create new file using default config (default.json)
    if(!configFile.exists()) {
        qInfo() << "Couldn't find file: "<<configFile.fileName()
                << "At" << configDir;
        QDir dir(configDir);
        if(!dir.exists())
            dir.mkpath(configDir);
        resetDefaults();
    }

    // Read and parse JSON
    configFile.open(QFile::ReadOnly);
    configFile.seek(0);
    QString jsonStr= configFile.readAll();
    configFile.close();
    kbdConfig.fromJson(jsonStr.toLocal8Bit());
}

// Convert JSON to string and write into config file
void Dialog::writeJson()
{
    writeJson(kbdConfig.toJson());
}

// Write JSON string to config file
void Dialog::writeJson(QByteArray json)
{
    if(configFile.isOpen())
        configFile.close();
    if(!configFile.open(QFile::WriteOnly))
        qCritical()<<"Failed to open file for writing. Changes may be lost.";
    configFile.seek(0);
    configFile.write(json);
    configFile.close();
}

// Reset to default configs
void Dialog::resetDefaults()
{
    QFile def("default.json");
    if(!def.exists()) {
        qCritical() << "Couldn't find/open default config file: default.json"
                << def.errorString();
        throw def.error();
    }
    if(!def.open(QFile::ReadOnly)) {
        qCritical() << "Couldn't Open";
    }
    qInfo() << "Found default config: " << def.fileName()
            << "At" << QDir::currentPath();
    def.seek(0);
    QByteArray defaultJson = def.readAll();
    def.close();
    qInfo() << defaultJson;
    writeJson(defaultJson);
}

void Dialog::on_pbReset_clicked()
{
    qInfo() << "Resetting keybindings to defaults";
    resetDefaults();
}

