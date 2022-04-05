#ifndef SHORTCUTEDIT_H
#define SHORTCUTEDIT_H

#include <QDialog>
#include <QJsonObject>
#include <QSharedPointer>

namespace Ui {
class ShortcutEdit;
}

class ShortcutEdit : public QDialog
{
    Q_OBJECT

public:
    explicit ShortcutEdit(QSharedPointer<QJsonObject> kbdObject, QWidget *parent = nullptr, bool isEditWindow = false, int index = 0);
    ~ShortcutEdit();

private slots:
    void on_pbSave_clicked();
    void on_cmbType_currentTextChanged(const QString &arg1);

private:
    Ui::ShortcutEdit *ui;
    bool isEditWindow;
    bool hasClause;
    int index;
    QSharedPointer<QJsonObject> kbdObject;
    QSet<QString> nonEditable;

signals:
    void save(QSharedPointer<QJsonObject> obj, bool editing, int index);
};

#endif // SHORTCUTEDIT_H
