#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase font;
    font.addApplicationFont(":/font/res/Andy Bold.ttf");
    MainWindow w;
    w.setWindowTitle("Touhou Big Sale");
    w.setWindowIcon(QIcon(":/enemy/res/Gaben_Reimu.png"));
    w.show();

    return a.exec();
}
