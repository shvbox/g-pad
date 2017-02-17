#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("shv");
    a.setApplicationName("G-Pad");
    
    MainWindow w;
    w.show();
    
    return a.exec();
}
