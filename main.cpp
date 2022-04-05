#include "dialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    QObject::connect(&w, &Dialog::save, &a, &QApplication::quit, Qt::QueuedConnection);
    w.show();
    return a.exec();
}
