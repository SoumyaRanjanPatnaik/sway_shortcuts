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


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void setApplication(QApplication *a){
        this->a = a;
    };

private:
    // Private Variables
    Ui::Dialog *ui;
    QApplication *a=nullptr;

    // Locations and Files
    QString configDir;
    QString configFilename = "config.json";
    QString configLoc;
    QFile configFile;

    // Json object storing keyboard config
    QJsonDocument kbdConfig;

    // I/O Config
    void configureLocations();
    void readJson();
    void writeJson();
    void writeJson(QByteArray &json);
    void writeConfig(QByteArray &s);

    // Helper Functions
    bool validateConfigObject(QJsonObject &obj);
    void populateCmb();
    void resetDefaults();
    QByteArray generateConfig();
    void clearKsShortcut();
    QString getKbdShortcut();
    void setKbdShortcut(QString combi);


private slots:
    void on_pbReset_clicked();
    void on_pbSave_clicked();
    void on_cmbAction_currentIndexChanged(int index);

    void on_pbRecord_clicked();
//    void saveSequence();
    void on_leShortcut_textChanged(const QString &arg1);

signals:
    void save();
};
#endif // DIALOG_H
