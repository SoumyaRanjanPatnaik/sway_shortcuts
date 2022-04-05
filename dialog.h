#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFile>
#include <QVector>
#include <QProcessEnvironment>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QKeyEvent>
#include <QKeySequenceEdit>
#include <shortcutedit.h>
#include <QMutex>


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    // Private Variables
    Ui::Dialog *ui;
    bool isEditingShortcut = false;
    quint32 currIndex = 0;

    // Locations and Files
    QString configDir;
    QString configFilename = "config.json";
    QString configLoc;
    QFile configFile;

    // Json object storing keyboard config
    QJsonDocument kbdConfig;
    QJsonArray kbdArray;

    // I/O Config
    void configureLocations();
    void readJson();
    void writeJson();
    void writeJson(QByteArray &json);
    void writeConfig(QByteArray &s);

    // Helper Functions
    void populateCmb();
    void resetDefaults();
    QByteArray generateConfig();
    QString getKbdShortcut();
    QJsonObject getKbdObject(quint32 index);
    void setKbdShortcut(QString combi);
    int spawnEditWindow(QSharedPointer<QJsonObject> obj, bool isEditWindow);
    void addAction();
    void updateAction(int index);
    void cmbInsertActions();
    void cmbUpdateAllActions();

    // Mutex
    QMutex mutex;

private slots:
    void on_pbReset_clicked();
    void on_pbSave_clicked();
    void on_cmbAction_currentIndexChanged(int index);
    void on_pbEdit_clicked();
    void on_pbApply_clicked();
    void on_pbEditAction_clicked();
    void on_pbAddAction_clicked();
    void saveEditAction(QSharedPointer<QJsonObject> obj,bool edit = false, int index = 0);


    void on_cbLocked_toggled(bool checked);

signals:
    void save();
};
#endif // DIALOG_H
