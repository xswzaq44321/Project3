#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

QTimer *timer;
QFont mainFont("Andy");
QRectF borderOfBullet = QRectF(-100, -100, 722, 866);
QRectF borderOfCharacter = QRectF(0, 0, 622, 766);
QList<QGraphicsItem*> *enemyList;
QList<QGraphicsItem*> *myBulletList;
QList<QGraphicsItem*> *enemyBulletList;
MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(0, 0, 1022, 766)),
    respawnTime(new QTime)
{
    timer = new QTimer();
    enemyList = new QList<QGraphicsItem*>();
    myBulletList = new QList<QGraphicsItem*>();
    enemyBulletList = new QList<QGraphicsItem*>();
    ui->setupUi(this);
    this->setFixedSize(1024, 768);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->installEventFilter(this);
    timer->start(16);
    respawnTime->start();
    moveKeys.insert(Qt::Key_Up);
    moveKeys.insert(Qt::Key_Down);
    moveKeys.insert(Qt::Key_Left);
    moveKeys.insert(Qt::Key_Right);
    functionKeys.insert(Qt::Key_Z);
    functionKeys.insert(Qt::Key_X);
    functionKeys.insert(Qt::Key_Shift);

    gameStart();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::gameStart(){
    enemyList->clear();
    enemyBulletList->clear();
    myBulletList->clear();
    connect(timer, SIGNAL(timeout()), this, SLOT(moveHandler()));
    connect(timer, SIGNAL(timeout()), this, SLOT(collidingDetect()));
    connect(timer, SIGNAL(timeout()), this, SLOT(attackHandler()));
    connect(timer, SIGNAL(timeout()), this, SLOT(infoBoardHandler()));

    QPixmap *infoPixmap = new QPixmap(":/pics/res/info_board_background.png");
    infoPixmap->scaled(400, 766);
    QGraphicsPixmapItem *infoItem = new QGraphicsPixmapItem(*infoPixmap);
    infoItem->setPos(622, 0);
    infoItem->setZValue(100);
    scene->addItem(infoItem);

    QFont scoreFont = QFont(mainFont);
    scoreFont.setPixelSize(40);
    scoreText = scene->addText("", scoreFont);
    scoreText->setDefaultTextColor(QColor(255, 255, 255));
    scoreText->setPos(672, 50);
    scoreText->setZValue(110);

    lifeCanvas = new QPixmap(300, 50);
    lifeCanvas->fill(Qt::transparent);
    lifePainter = new QPainter(lifeCanvas);
    for(int i = 0; i < 3; ++i){
        lifePainter->drawPixmap(0 + 60 * i, 3, QPixmap(":/player/res/Savings.png").scaled(36, 44));
    }
    life = new QGraphicsPixmapItem(*lifeCanvas);
    life->setZValue(110);
    life->setPos(672, 100);
    scene->addItem(life);

    boss = new gaben_reimu(3000);
    scene->addItem(boss);
    boss->setPosition((borderOfCharacter.width() - boss->boundingRect().width()) / 2, 0 + 40);
    bossHealth = scene->addRect(10, 10, borderOfCharacter.width() - 20, 10, QPen(QColor(0, 0, 0, 0)), QBrush(QColor(200, 0, 0)));
    bossHealth->setZValue(0);
    enemyList->insert(enemyList->end(), boss);

    player = new wallet;
    scene->addItem(player);
    scene->addItem(dynamic_cast<wallet*>(player)->heart);
    player->setPosition((borderOfCharacter.width() - player->boundingRect().width()) / 2, 766 - player->boundingRect().height());
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
    if(player != NULL){
        if(!playerIsDead){
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
            double r = sqrt(vx*vx + vy*vy);
            vx = (r == 0 ? 0 : vx/r * speed);
            vy = (r == 0 ? 0 : vy/r * speed);
            player->move(vx, vy);
            //        qDebug() << vx << vy;
        }
    }
}

bool MainWindow::collidingDetect(){
//    qDebug() << myBulletList->size() + enemyBulletList->size();
    if(respawnTime->elapsed() > 4000){
        for(QGraphicsItem* it:((*enemyList) + (*enemyBulletList))){ // see if player hits enemy
            if(dynamic_cast<wallet*>(player)->heart->collidesWithItem(it)){
                qDebug() << "player died";
                qDebug() << "respawn in 2 sec...";
                this->scene->removeItem(dynamic_cast<wallet*>(player)->heart);
                this->scene->removeItem(player);
                playerIsDead = true;
                if(player->hit()){ // hit will return whether player is under 6-ft
                    QPixmap *overPic = new QPixmap(borderOfCharacter.width(), borderOfCharacter.height());
                    overPic->fill(Qt::transparent);
                    QPainter painter(overPic);
                    painter.fillRect(borderOfCharacter, QColor(0, 0, 0, 100));
                    painter.setOpacity(0.7);
                    painter.drawPixmap(0, 0, borderOfCharacter.width(), borderOfCharacter.height(), QPixmap(":/pics/res/GameOver.png"));
                    QGraphicsPixmapItem *over = new QGraphicsPixmapItem(*overPic);
                    over->setZValue(110);
                    over->setPos(0, 0);
                    scene->addItem(over);
                }else{
                    respawnTime->start();
                    connect(timer, SIGNAL(timeout()), this, SLOT(respawn()));
                }
                return true;
            }
        }
    }
    for(auto it = enemyList->begin(); it != enemyList->end(); ++it){
        for(auto jt = myBulletList->begin(); jt != myBulletList->end(); ++jt){
            if((*it) == nullptr || (*jt) == nullptr) continue;
            if((*it)->collidesWithItem(*jt)){
                bool isDead = dynamic_cast<character*>(*it)->hit();
                delete (*jt);
                (*jt) = nullptr;
                if(isDead){
                    if(*it == boss){ // if dead one is boss
                        boss = NULL;
                    }
                    delete (*it);
                    (*it) = nullptr;
                }
            }
        }
    }
    for(int i = 0; i < myBulletList->count(nullptr); ++i){
        myBulletList->removeOne(nullptr);
    }
    for(int i = 0; i < enemyBulletList->count(nullptr); ++i){
        enemyBulletList->removeOne(nullptr);
    }
    return false;
}

void MainWindow::respawn(){
    disconnect(timer, SIGNAL(timeout()), this, SLOT(attackHandler()));
    if(respawnTime->elapsed() >= 1000){
        if(playerIsDead){
            scene->addItem(player);
            scene->addItem(dynamic_cast<wallet*>(player)->heart);
            playerIsDead = false;
        }
        if(respawnTime->elapsed() <= 2000){
            player->setPosition((borderOfCharacter.width() - player->boundingRect().width()) /  2, scene->height() - player->boundingRect().height()*((respawnTime->elapsed() - 1000.0) / 1000.0));
            return;
        }
        disconnect(timer, SIGNAL(timeout()), this, SLOT(respawn()));
        connect(timer, SIGNAL(timeout()), this, SLOT(attackHandler()));
    }
}

void MainWindow::attackHandler(){
    if(player != NULL){
        if(!playerIsDead)
            score += timer->interval() / 10.0;
        if(attack && !playerIsDead){ // player attacks
            player->attack(timer);
        }
    }
    if(boss != NULL){
        boss->attack(timer);
        bossHealth->setRect(10, 10, (borderOfCharacter.width() - 20)*((float)boss->hp / boss->initialHp), 10);
    }
}

void MainWindow::infoBoardHandler(){
    static int playerLife = 3;
    {
        char temp[100];
        sprintf(temp, "Score:%08d", score);
        scoreText->setPlainText(QString::fromLocal8Bit(temp));
    }
    if(player != nullptr && player->hp != playerLife){
        if(player->hp > playerLife){
            for(int i = playerLife; i < player->hp; ++i){
                lifePainter->drawPixmap(i * 60, 3, QPixmap(":/player/res/Savings.png").scaled(36, 44));
            }
            playerLife = player->hp;
        }else{
//            qDebug() << "erase";
            lifePainter->setCompositionMode(QPainter::CompositionMode_Source);
            lifePainter->fillRect(player->hp * 60, 0, 300, 50, Qt::transparent);
            lifePainter->setCompositionMode(QPainter::CompositionMode_SourceOver);
            playerLife = player->hp;
        }
        life->setPixmap(*lifeCanvas);
    }
}
