#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFile>
#include <QProcessEnvironment>
#include <QJsonDocument>
#include <QJsonObject>


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
    QString configDir;
    QString configFilename = "config.json";
    QString configLoc;
    QFile configFile;
    QJsonDocument kbdConfig;

    // Private Functions
    void readJson();
    void writeJson();
    void writeJson(QByteArray json);
    void resetDefaults();
private slots:
    void on_pbReset_clicked();
};
#endif // DIALOG_H
