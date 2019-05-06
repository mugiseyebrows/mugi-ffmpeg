#include <QApplication>
#include "mainwindow.h"

#include "progresstable.h"
#include <QDebug>
#include <QHeaderView>
#include "version.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName(APP_NAME);
    a.setApplicationVersion(APP_VERSION);

    MainWindow w;
    w.show();

    return a.exec();
}
