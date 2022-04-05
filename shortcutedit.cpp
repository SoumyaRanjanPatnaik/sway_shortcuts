#include "shortcutedit.h"
#include "ui_shortcutedit.h"

ShortcutEdit::ShortcutEdit(QSharedPointer<QJsonObject> kbdObject,QWidget *parent, bool isEditWindow, int index) :
    kbdObject(kbdObject),
    QDialog(parent),
    isEditWindow(isEditWindow),
    ui(new Ui::ShortcutEdit),
    index(index)
{
    ui->setupUi(this);
    ui->cbLocked->setChecked((*kbdObject)["locked"].toBool());
    if(isEditWindow) {
        setWindowTitle("Edit Shortcut");
    }
    nonEditable = QSet<QString> {"splith", "splitv", "reload", "kill", "fullscreen"};
    ui->cmbType->setCurrentIndex(0);
    if(isEditWindow){
        ui->cmbType->setCurrentText(kbdObject->value("type").toString());
        ui->leName->setText(kbdObject->value("name").toString());
        ui->leBinding->setText(kbdObject->value("combi").toString());
        if(kbdObject->contains("clause")) {
            ui->leClause->setText(kbdObject->value("clause").toString());
        }
    }
}

ShortcutEdit::~ShortcutEdit()
{
    delete ui;
}

void ShortcutEdit::on_pbSave_clicked()
{
    kbdObject->insert("type", ui->cmbType->currentText());
    kbdObject->insert("name" , ui->leName->text());
    kbdObject->insert("combi" , ui->leBinding->text());
    kbdObject->insert("locked", ui->cbLocked->isChecked());
    if(nonEditable.contains(ui->cmbType->currentText())){
        kbdObject->remove("clause");
    }
    else {
        kbdObject->insert("clause" , ui->leClause->text());
    }
    for(auto c: kbdObject->keys()) {
        qInfo() << (*kbdObject)[c];
    }
    emit save(kbdObject, isEditWindow, index);
    accept();
}


void ShortcutEdit::on_cmbType_currentTextChanged(const QString &arg1)
{
    if(nonEditable.contains(arg1)) {
        ui->leClause->setEnabled(false);
    }
    else {
        ui->leClause->setEnabled(true);
    }
}
