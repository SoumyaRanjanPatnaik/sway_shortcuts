#include "dialog.h"
#include "./ui_dialog.h"
#include <QDir>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    configureLocations();
    readJson();
    cmbInsertActions();
}

Dialog::~Dialog()
{
    delete ui;
}

// Configure locations for config files
void Dialog::configureLocations()
{
    QProcessEnvironment env;
    QString homeDir = env.systemEnvironment().value("HOME") + '/';
    configDir = homeDir + ".config/swaykbd-settings/";
    configLoc = configDir + configFilename;
    configFile.setFileName(configLoc);
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
    kbdConfig = QJsonDocument::fromJson(jsonStr.toLocal8Bit());
    if(kbdConfig.isNull())
        qCritical() << "Config File is Invalid";
    else
        qInfo() << "Successfully Parsed Config File";
    kbdArray = kbdConfig.array();
}

// Convert JSON to string and write into config file
void Dialog::writeJson()
{
    kbdConfig.setArray(kbdArray);
    QByteArray json = kbdConfig.toJson();
    writeJson(json);
}

// Write JSON string to config file
void Dialog::writeJson(QByteArray &json)
{
    if(configFile.isOpen())
        configFile.close();
    if(!configFile.open(QFile::WriteOnly))
        qCritical()<<"Failed to open file for writing. Changes may be lost.";
    configFile.seek(0);
    if(configFile.write(json) < 0) {
        qCritical() << "Failed to write into config file: "<<configFile.errorString();
    }
    configFile.close();
}

// Write generated configurations to file
void Dialog::writeConfig(QByteArray &s)
{
    QFile gen(configDir+"kbd_config");
    gen.open(QFile::WriteOnly);
    gen.write(s);
    gen.close();
}

// Reset to default configs
void Dialog::resetDefaults()
{
    QFile def("/etc/sway_shortcuts/default.json");
    if(!def.exists()) {
        def.setFileName("default.json");
    }
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
    writeJson(defaultJson);
    readJson();
    cmbInsertActions();
}

// Generate configurations
QByteArray Dialog::generateConfig()
{
    QByteArray configStr = "**** Generated By Sway Shortcuts Manager ****\n\n";
    foreach(const QJsonValue & value, kbdArray) {
        QJsonObject curr = value.toObject();
        if(!curr.contains("combi"))
            continue;
        QString tmpStr = "bindsym ";
        if(curr.contains("locked") && curr["locked"].toBool()) {
            tmpStr += "--locked ";
        }
        tmpStr += curr["combi"].toString() + " ";
        tmpStr += curr["type"].toString() + " ";
        tmpStr += curr["clause"].toString() + " \n";
        qInfo() << tmpStr;
        configStr.push_back(tmpStr.toLocal8Bit());
    }
    return configStr;
}

// Get keybindings for currently selected action
QString Dialog::getKbdShortcut()
{
    qInfo() << getKbdObject(currIndex)["combi"].toString();
    return getKbdObject(currIndex)["combi"].toString();
}

// Get QJsonObject present at the index specified in the array
QJsonObject Dialog::getKbdObject(quint32 index)
{
    return kbdArray[index].toObject();
}

// set keybinding for the currently selected item.
void Dialog::setKbdShortcut(QString combi)
{
    QJsonObject currObj = kbdArray[currIndex].toObject();
    currObj["combi"] = combi;
    kbdArray[currIndex] = currObj;
    kbdConfig.setArray(kbdArray);
}

// Open edit/add window
int Dialog::spawnEditWindow(QSharedPointer<QJsonObject> obj, bool isEditWindow = false)
{
    const quint32 tempIndex = currIndex;
    ShortcutEdit *editWindow;
    if(isEditWindow) {
         editWindow = new ShortcutEdit(obj, this, isEditWindow, currIndex);
    }
    else {
        editWindow = new ShortcutEdit(obj, this, isEditWindow);
    }
    QObject::connect(editWindow, &ShortcutEdit::save,this, &Dialog::saveEditAction);
    editWindow->open();
    return tempIndex;
}

// Add Actions List
void Dialog::addAction()
{
    int index = kbdArray.count()-1;
    ui->cmbAction->insertItem(index, kbdArray[index].toObject()["name"].toString());
    ui->cmbAction->setCurrentIndex(index);
}

void Dialog::updateAction(int index)
{
    qInfo()<<index;
    ui->cmbAction->setItemText(index, kbdArray[index].toObject()["name"].toString());
    ui->leShortcut->setText(getKbdShortcut());
    ui->cbLocked->setChecked(getKbdObject(currIndex)["locked"].toBool());
}

void Dialog::cmbInsertActions()
{
    QStringList cmbItems;
    for(auto curr: qAsConst(kbdArray)) {
        QJsonObject binding = curr.toObject();
        cmbItems.push_back(binding["name"].toString());
    }
    ui->cmbAction->clear();
    ui->cmbAction->insertItems(0,cmbItems);
    ui->cmbAction->setCurrentIndex(0);
    currIndex = 0;
}


void Dialog::on_pbReset_clicked()
{
    qInfo() << "Resetting keybindings to defaults";
    resetDefaults();
}

void Dialog::on_pbSave_clicked()
{
    on_pbApply_clicked();
    emit save();
}

void Dialog::on_cmbAction_currentIndexChanged(int index)
{
    isEditingShortcut = false;
    currIndex = index;
    ui->leShortcut->setText(getKbdShortcut());
    ui->cbLocked->setChecked(getKbdObject(currIndex)["locked"].toBool());
}

void Dialog::on_pbEdit_clicked()
{
    if(isEditingShortcut) {
        ui->leShortcut->setEnabled(false);
        ui->pbEdit->setText("Edit");
        QString combi = getKbdShortcut();
        setKbdShortcut(ui->leShortcut->text());
        isEditingShortcut = false;
    }
    else {
        ui->leShortcut->setEnabled(true);
        ui->leShortcut->setFocus();
        ui->pbEdit->setText("Save");
        isEditingShortcut = true;
    }
}


void Dialog::on_pbApply_clicked()
{
    if(isEditingShortcut) {
        on_pbEdit_clicked();
    }
    writeJson();
    QByteArray configGenStr = generateConfig();
    writeConfig(configGenStr);
}


void Dialog::on_pbEditAction_clicked()
{
    QSharedPointer<QJsonObject> curr(new QJsonObject(getKbdObject(currIndex)));
    spawnEditWindow(curr, true);
}

void Dialog::on_pbAddAction_clicked()
{
    QSharedPointer<QJsonObject> curr(new QJsonObject);
    spawnEditWindow(curr);
}

void Dialog::saveEditAction(QSharedPointer<QJsonObject> obj, bool edit, int index)
{
    if(edit) {
        kbdArray[index] = *obj;
        updateAction(index);
    }
    else {
        kbdArray.append(*obj);
        addAction();
    }
}

void Dialog::on_cbLocked_toggled(bool checked)
{
    QJsonObject curr = getKbdObject(currIndex);
    curr["locked"] = checked;
    if(currIndex < kbdArray.size()){
        kbdArray[currIndex] = curr;
    }
}

