#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Touhou Big Sale");
    w.setWindowIcon(QIcon(":/enemy/res/Gaben_Reimu.png"));
    w.show();

    return a.exec();
}
