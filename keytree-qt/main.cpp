#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "keynode/logger.h"
using namespace  std;

int main(int argc, char *argv[])
{
    try
    {
        Logger::setLogLevelError();
        //Logger::setLogLevelWarning();
        //Logger::setLogLevelDebug();
        Logger::setLogLevelInfo();

        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
    }
    catch (int e)
    {
        cout << "An exception occurred. Exception Nr. " << e << '\n';
    }

    //return 0;

}
