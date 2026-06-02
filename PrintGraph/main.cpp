#include "MainWindow.h"
#include "IOC.h"
#include "SqliteDataProvider.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    gContainer.RegisterFactory<IDataProvider, SqliteDataProvider>();

    MainWindow w;
    w.show();

    return a.exec();
}
