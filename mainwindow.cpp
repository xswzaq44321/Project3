#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

QRectF borderOfBullet = QRectF(-100, -100, 1122, 866);
QRectF borderOfCharacter = QRectF(0, 0, 1022, 766);
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(0, 0, 1022, 766, this)),
    timer(new QTimer),
    respawnTime(new QTime),
    attackTime(new QTime)
{
    ui->setupUi(this);
    this->setFixedSize(1024, 768);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->installEventFilter(this);
    timer->start(10);
    respawnTime->start();
    attackTime->start();
    moveKeys.insert(Qt::Key_Up);
    moveKeys.insert(Qt::Key_Down);
    moveKeys.insert(Qt::Key_Left);
    moveKeys.insert(Qt::Key_Right);
    functionKeys.insert(Qt::Key_Z);
    functionKeys.insert(Qt::Key_X);
    functionKeys.insert(Qt::Key_Shift);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(moveHandler()));
    connect(this->timer, SIGNAL(timeout()), this, SLOT(collidingDetect()));
    connect(this->timer, SIGNAL(timeout()), this, SLOT(attackHandler()));

    boss = new gaben_reimu;
    scene->addItem(boss);
    player = new wallet;
    scene->addItem(player);
    boss->setPos((scene->width() - boss->boundingRect().width()) / 2, 0);
    player->setPos((scene->width() - player->boundingRect().width()) / 2, 766 - player->boundingRect().height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event){
    if(event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent*>(event);
        this->keyPressEvent(keyEvent);
        return true;
    }else if(event->type() == QEvent::KeyRelease){
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent*>(event);
        this->keyReleaseEvent(keyEvent);
        return true;
    }else{
        return QObject::eventFilter(watched, event);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e){
    if(moveKeys.count(e->key())){
        switch(e->key()){
        case Qt::Key_Up:
            moving[0] = true;
            break;
        case Qt::Key_Down:
            moving[1] = true;
            break;
        case Qt::Key_Left:
            moving[2] = true;
            break;
        case Qt::Key_Right:
            moving[3] = true;
            break;
        }
    }else if(functionKeys.count(e->key())){
        switch(e->key()){
        case Qt::Key_Shift:
            speed = 1;
            break;
        case Qt::Key_Z:
            attack = true;
            break;
        case Qt::Key_X:
            break;
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e){
    if(moveKeys.count(e->key())){
        switch(e->key()){
        case Qt::Key_Up:
            moving[0] = false;
            break;
        case Qt::Key_Down:
            moving[1] = false;
            break;
        case Qt::Key_Left:
            moving[2] = false;
            break;
        case Qt::Key_Right:
            moving[3] = false;
            break;
        }
    }else if(functionKeys.count(e->key())){
        switch(e->key()){
        case Qt::Key_Shift:
            speed = 2;
            break;
        case Qt::Key_Z:
            attack = false;
            break;
        }
    }
}

void MainWindow::moveHandler(){
    double vx = 0, vy = 0;
    if(moving[0]){
        vy += 1;
    }
    if(moving[1]){
        vy -= 1;
    }
    if(moving[2]){
        vx -= 1;
    }
    if(moving[3]){
        vx += 1;
    }
    if(player != NULL){
        double r = sqrt(vx*vx + vy*vy);
        vx = (r == 0 ? 0 : vx/r * speed);
        vy = (r == 0 ? 0 : vy/r * speed);
        player->move(vx, vy);
//        qDebug() << vx << vy;
    }
}

bool MainWindow::collidingDetect(){
    if(boss == NULL || player == NULL){
        return false;
    }
    if(player->collidesWithItem(boss)){
        qDebug() << "collide with boss";
        qDebug() << "respawn in 2 sec...";
        scene->removeItem(player);
        delete player;
        player = NULL;
        respawnTime->restart();
        connect(this->timer, SIGNAL(timeout()), this, SLOT(respawn()));
        return true;
    }
    return false;
}

void MainWindow::respawn(){
    disconnect(this->timer, SIGNAL(timeout()), this, SLOT(moveHandler()));
    disconnect(this->timer, SIGNAL(timeout()), this, SLOT(attackHandler()));
    if(respawnTime->elapsed() >= 1000){
        if(player == NULL){
            player = new wallet;
            scene->addItem(player);
        }
        if(respawnTime->elapsed() <= 2000){
            player->setPos((scene->width() - player->boundingRect().width()) /  2, scene->height() - player->boundingRect().height()*((respawnTime->elapsed() - 1000.0) / 1000.0));
            return;
        }
        disconnect(this->timer, SIGNAL(timeout()), this, SLOT(respawn()));
        connect(this->timer, SIGNAL(timeout()), this, SLOT(moveHandler()));
        connect(this->timer, SIGNAL(timeout()), this, SLOT(attackHandler()));
    }
}

void MainWindow::attackHandler(){
    if(attack && attackTime->elapsed() >= 40){ // player attacks
        if(player == NULL){
            return;
        }
        player->attack(timer, boss);
        attackTime->start();
    }
}
