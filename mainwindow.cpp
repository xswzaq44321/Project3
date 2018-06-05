#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(0, 0, 1022, 766, this)),
    timer(new QTimer)
{
    ui->setupUi(this);
    this->setFixedSize(1024, 768);
    ui->graphicsView->setScene(scene);
    item = new QGraphicsPixmapItem(QPixmap(":/enemy/res/Gaben_Reimu.png").scaled(64, 88));
    scene->addItem(item);
    item->setPos(0, 0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
