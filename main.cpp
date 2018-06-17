#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase font;
    font.addApplicationFont(":/font/Andy");
    font.addApplicationFont(":/font/Zelda");
    font.addApplicationFont(":/font/Arial");
    font.addApplicationFont(":/font/Arial Bold");
    MainWindow w;
    w.setWindowTitle("Touhou Big Sale");
    w.setWindowIcon(QIcon(":/enemy/gaben_reimu"));
    w.show();

    return a.exec();
}
