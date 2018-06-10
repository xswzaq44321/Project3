#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

QFont mainFont("Hylia Serif Beta");
QRectF borderOfBullet = QRectF(-100, -100, 722, 866);
QRectF borderOfCharacter = QRectF(0, 0, 622, 766);
QList<QGraphicsItem*> *enemyList;
QList<QGraphicsItem*> *myBullitList;
QList<QGraphicsItem*> *enemyBullitList;
MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(0, 0, 1022, 766)),
    timer(new QTimer),
    respawnTime(new QTime)
{
    ui->setupUi(this);
    this->setFixedSize(1024, 768);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->installEventFilter(this);
    timer->start(10);
    respawnTime->start();
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

    enemyList = new QList<QGraphicsItem*>();
    myBullitList = new QList<QGraphicsItem*>();
    enemyBullitList = new QList<QGraphicsItem*>();
    boss = new gaben_reimu;
    player = new wallet;
    scene->addItem(boss);
    scene->addItem(player);
    scene->addItem(dynamic_cast<wallet*>(player)->heart);
    boss->setPosition((borderOfCharacter.width() - boss->boundingRect().width()) / 2, 0 + 40);
    player->setPosition((borderOfCharacter.width() - player->boundingRect().width()) / 2, 766 - player->boundingRect().height());
    bossHealth = scene->addRect(10, 10, borderOfCharacter.width() - 20, 10, QPen(QColor(0, 0, 0, 0)), QBrush(QColor(200, 0, 0)));
    bossHealth->setZValue(90);
    scene->addRect(622, 0, 400, 766, QPen(QColor(0, 0, 0, 0)), QBrush(QColor(47, 63, 86)))->setZValue(100);
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
    QList<QGraphicsItem*> all = scene->items();
    enemyList->clear();
    myBullitList->clear();
    enemyBullitList->clear();
    for(QGraphicsItem* it:all){
        character *who;
        bullet *b;
        if((who = dynamic_cast<character*>(it)) != 0){
            if(who != player){
                enemyList->insert(enemyList->end(), it);
            }
            continue;
        }
        if((b = dynamic_cast<bullet*>(it)) != 0){
            if(b->origin == player){
                myBullitList->insert(myBullitList->end(), it);
            }else{
                enemyBullitList->insert(enemyBullitList->end(), it);
            }
            continue;
        }
    }
    //    qDebug() << myBullitList->size();
    if(!playerIsDead){
        for(QGraphicsItem* it:((*enemyList) + (*enemyBullitList))){ // see if player hits enemy
            if(dynamic_cast<wallet*>(player)->heart->collidesWithItem(it)){
                qDebug() << "player died";
                qDebug() << "respawn in 2 sec...";
                this->scene->removeItem(dynamic_cast<wallet*>(player)->heart);
                this->scene->removeItem(player);
                playerIsDead = true;
                if(player->hit()){ // hit will return whether player is under 6-ft
                    QGraphicsPixmapItem *over = new QGraphicsPixmapItem(QPixmap(":/texts/res/GameOver.png"));
                    over->setZValue(100);
                    over->setPos(0, 0);
                    scene->addItem(over);
                }else{
                    respawnTime->restart();
                    connect(this->timer, SIGNAL(timeout()), this, SLOT(respawn()));
                }
                return true;
            }
        }
    }
    for(auto it = enemyList->begin(); it != enemyList->end(); ++it){
        for(auto jt = myBullitList->begin(); jt != myBullitList->end(); ++jt){
            if((*it) == nullptr || (*jt) == nullptr) continue;
            if((*it)->collidesWithItem(*jt)){
                bool isDead = dynamic_cast<character*>(*it)->hit();
                delete (*jt);
                (*jt) = nullptr;
                if(isDead){
                    if(dynamic_cast<gaben_reimu*>(*it) != 0){ // if dead one is boss
                        boss = NULL;
                    }
                    enemyList->removeOne(*it);
                    delete (*it);
                    (*it) = nullptr;
                }
            }
        }
    }
    return false;
}

void MainWindow::respawn(){
    disconnect(this->timer, SIGNAL(timeout()), this, SLOT(moveHandler()));
    disconnect(this->timer, SIGNAL(timeout()), this, SLOT(attackHandler()));
    if(respawnTime->elapsed() >= 1000){
        if(playerIsDead){
            scene->addItem(player);
            scene->addItem(dynamic_cast<wallet*>(player)->heart);
        }
        if(respawnTime->elapsed() <= 2000){
            player->setPosition((borderOfCharacter.width() - player->boundingRect().width()) /  2, scene->height() - player->boundingRect().height()*((respawnTime->elapsed() - 1000.0) / 1000.0));
            return;
        }
        playerIsDead = false;
        disconnect(this->timer, SIGNAL(timeout()), this, SLOT(respawn()));
        connect(this->timer, SIGNAL(timeout()), this, SLOT(moveHandler()));
        connect(this->timer, SIGNAL(timeout()), this, SLOT(attackHandler()));
    }
}

void MainWindow::attackHandler(){
    if(attack && !playerIsDead){ // player attacks
        player->attack(timer);
    }
    if(boss != NULL){
        boss->attack(timer);
        bossHealth->setRect(10, 10, (borderOfCharacter.width() - 20)*((float)boss->hp / boss->initialHp), 10);
    }
}
