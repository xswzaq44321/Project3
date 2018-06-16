#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

QFont mainFont("Andy");
QRectF borderOfBullet = QRectF(-100, -100, 722, 866);
QRectF borderOfCharacter = QRectF(0, 0, 622, 766);
QList<QGraphicsItem*> enemyList;
QList<QGraphicsItem*> myBulletList;
QList<QGraphicsItem*> enemyBulletList;
QList<QGraphicsItem*> missileList;
QTimer *timer;
character *boss = nullptr, *player = nullptr;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene(0, 0, 1022, 766)),
    respawnTime(new QTime),
    shockWaveTime(new QTime)
{
    timer = new QTimer;
    ui->setupUi(this);
    this->setFixedSize(1024, 768);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->installEventFilter(this);
    timer->start(16);
    respawnTime->start();
    qsrand(time(NULL));
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
    disconnect(timer);
    delete bossHealth;
    delete scoreText;
    delete life;
    delete infoItem;
    delete lifePainter;
    delete lifeCanvas;
    delete respawnTime;
    delete scene;
}

void MainWindow::gameStart(){
    connect(timer, SIGNAL(timeout()), this, SLOT(moveHandler()));
    connect(timer, SIGNAL(timeout()), this, SLOT(collidingDetect()));
    connect(timer, SIGNAL(timeout()), this, SLOT(attackHandler()));
    connect(timer, SIGNAL(timeout()), this, SLOT(infoBoardHandler()));

    infoItem = new QGraphicsPixmapItem(QPixmap(":/pics/res/info_board_background.png").scaled(400, 766, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    infoItem->setPos(622, 0);
    infoItem->setZValue(100);
    scene->addItem(infoItem);

    QFont scoreFont = QFont(mainFont);
    scoreFont.setPixelSize(40);
    scoreText = scene->addText("", scoreFont);
    scoreText->setDefaultTextColor(QColor(255, 255, 255));
    scoreText->setPos(672, 50);
    scoreText->setZValue(110);

    lifeCanvas = new QPixmap(300, 100);
    lifeCanvas->fill(Qt::transparent);
    lifePainter = new QPainter(lifeCanvas);
    for(int i = 0; i < 3; ++i){
        lifePainter->drawPixmap(0 + 60 * i, 3, QPixmap(":/player/res/Savings.png").scaled(36, 44, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        lifePainter->drawPixmap(0 + 60 * i, 63, QPixmap(":/bullets/res/card/master.png").scaled(36, 22.5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
    life = new QGraphicsPixmapItem(*lifeCanvas);
    life->setZValue(110);
    life->setPos(672, 100);
    scene->addItem(life);

    sizeOfBackground.setX(borderOfCharacter.width());
    sizeOfBackground.setY((borderOfCharacter.width() / 1344.0) * 6760);
    QPixmap backgroundCanvas(1344, 6760);
    backgroundCanvas.fill(Qt::transparent);
    QPainter backgroundPainter(&backgroundCanvas);
    backgroundPainter.setOpacity(0.5);
    backgroundPainter.drawPixmap(0, 0, QPixmap(":/gameList/res/game_list/game_list.png"));
    backgroundItem[0] = new QGraphicsPixmapItem(backgroundCanvas.scaled(sizeOfBackground.x(), sizeOfBackground.y()));
    backgroundItem[0]->setPos(0, -sizeOfBackground.y() + borderOfCharacter.height());
    backgroundItem[0]->setZValue(-10);
    scene->addItem(backgroundItem[0]);
    backgroundItem[1] = new QGraphicsPixmapItem(backgroundCanvas.scaled(sizeOfBackground.x(), sizeOfBackground.y()));
    backgroundItem[1]->setPos(0, -2*sizeOfBackground.y() + borderOfCharacter.height());
    backgroundItem[1]->setZValue(-10);
    scene->addItem(backgroundItem[1]);

    boss = new gaben_reimu(3000);
    scene->addItem(boss);
    boss->setPosition((borderOfCharacter.width() - boss->boundingRect().width()) / 2, 0 + 40);
    bossHealth = scene->addRect(10, 10, borderOfCharacter.width() - 20, 10, QPen(QColor(0, 0, 0, 0)), QBrush(QColor(200, 0, 0)));
    bossHealth->setZValue(0);
    enemyList.insert(enemyList.end(), boss);

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
            bigOne = true;
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

void MainWindow::collidingDetect(){
//    qDebug() << myBulletList.size() + enemyBulletList.size();
    if(respawnTime->elapsed() > 4000 && player != nullptr){
        for(QGraphicsItem* it:((enemyList) + (enemyBulletList))){ // see if player hits enemy
            if(dynamic_cast<wallet*>(player)->heart->collidesWithItem(it) && !playerIsDead){
                qDebug() << "player died";
                this->scene->removeItem(dynamic_cast<wallet*>(player)->heart);
                this->scene->removeItem(player);
                playerIsDead = true;
                if(player->hit()){ // hit will return whether player is under 6-ft
                    qDebug() << "player is under 6-ft";
                    connect(timer, SIGNAL(timeout()), this, SLOT(gameJudger()));
                }else{
                    qDebug() << "respawn in 2 sec...";
                    respawnTime->start();
                    connect(timer, SIGNAL(timeout()), this, SLOT(respawn()));
                }
                shockWaveTime->start();
                explodeConnection = connect(timer, &QTimer::timeout, [this](void){
                    playerExplode(QPointF(player->x() + player->boundingRect().width() / 2, player->y() + player->boundingRect().height() / 2));
                });
            }
        }
    }
    for(auto it = enemyList.begin(); it != enemyList.end(); ++it){
        for(auto jt = myBulletList.begin(); jt != myBulletList.end(); ++jt){
            if((*it) == nullptr || (*jt) == nullptr) continue;
            if((*it)->collidesWithItem(*jt)){
                score += 10 + qPow(dynamic_cast<wallet*>(player)->spells, 2);
                bool isDead = dynamic_cast<character*>(*it)->hit();
                delete (*jt);
                (*jt) = nullptr;
                if(isDead){
                    if((*it) == dynamic_cast<QGraphicsItem*>(boss)){ // if dead one is boss
                        boss = nullptr;
                        connect(timer, SIGNAL(timeout()), this, SLOT(gameJudger()));
                    }
                    delete (*it);
                    (*it) = nullptr;
                }
            }
        }
    }
    for(QGraphicsItem* it:(missileList)){
        for(auto jt = enemyList.begin(); jt != enemyList.end(); ++jt){
            if((it) == nullptr || (*jt) == nullptr) continue;
            if((it)->collidesWithItem(*jt)){
                score += 1 * timer->interval()/10.0;
                bool isDead = dynamic_cast<character*>(*jt)->hit(timer->interval() / 20.0);
                if(isDead){
                    if(*jt == boss){ // if dead one is boss
                        boss = nullptr;
                        connect(timer, SIGNAL(timeout()), this, SLOT(gameJudger()));
                    }
                    delete (*jt);
                    (*jt) = nullptr;
                }
            }
        }
        for(auto jt = enemyBulletList.begin(); jt != enemyBulletList.end(); ++jt){
            if((it) == nullptr || (*jt) == nullptr) continue;
            if((it)->collidesWithItem(*jt)){
                delete (*jt);
                (*jt) = nullptr;
            }
        }
    }
    if(shockWave != nullptr){
        for(auto it = enemyBulletList.begin(); it != enemyBulletList.end(); ++it){
            if((*it) == nullptr) continue;
            if((*it)->collidesWithItem(shockWave)){
                delete (*it);
                (*it) = nullptr;
            }
        }
    }

    // house keeping
    for(int i = 0; i < enemyList.count(nullptr); ++i){
        enemyList.removeOne(nullptr);
    }
    for(int i = 0; i < myBulletList.count(nullptr); ++i){
        myBulletList.removeOne(nullptr);
    }
    for(int i = 0; i < missileList.count(nullptr); ++i){
        missileList.removeOne(nullptr);
    }
    for(int i = 0; i < enemyBulletList.count(nullptr); ++i){
        enemyBulletList.removeOne(nullptr);
    }
}

void MainWindow::playerExplode(QPointF center){
    if(shockWave == nullptr){
        shockWave = new QGraphicsPixmapItem;
        scene->addItem(shockWave);
    }
    if(shockWaveTime->elapsed() < 1000){
        qreal size = shockWaveTime->elapsed() / 1000.0 * borderOfCharacter.height() * 2;
//        qDebug() << size;
        QPixmap waveCanvas(size, size);
        waveCanvas.fill(Qt::transparent);
        QPainter wavePainter(&waveCanvas);
        wavePainter.setPen(QPen(QColor(255, 207, 64, 128), 4));
        wavePainter.drawEllipse(QPointF(size / 2, size / 2), size / 2 - 2, size / 2 - 2);
        shockWave->setPixmap(waveCanvas);
        shockWave->setPos(center.x() - size / 2, center.y() - size / 2);
    }else{
        disconnect(explodeConnection);
        delete shockWave;
        shockWave = nullptr;
    }
}

void MainWindow::respawn(){
    static bool playerOnScene;
    if(respawnTime->elapsed() >= 1000){
        if(!playerOnScene){
            scene->addItem(player);
            scene->addItem(dynamic_cast<wallet*>(player)->heart);
            playerOnScene = true;
        }
        if(respawnTime->elapsed() <= 2000){
            player->setPosition((borderOfCharacter.width() - player->boundingRect().width()) /  2, scene->height() - player->boundingRect().height()*((respawnTime->elapsed() - 1000.0) / 1000.0));
            return;
        }
        playerIsDead = false;
        disconnect(timer, SIGNAL(timeout()), this, SLOT(respawn()));
    }else{
        playerOnScene = false;
    }
}

void MainWindow::gameJudger(){
    static QTime *clearingTime = nullptr;
    if(clearingTime == nullptr){
        clearingTime = new QTime;
        clearingTime->start();
    }
    if(clearingTime->elapsed() > 2000){
        if(boss == nullptr){
            gameWin();
        }else{
            gameOver();
        }
        disconnect(timer, SIGNAL(timeout()), this, SLOT(gameJudger()));
        delete clearingTime;
        clearingTime = nullptr;
    }
}

void MainWindow::gameWin(){
    static QGraphicsTextItem *totalScore, *totalSpend, *totalCard;
    respawnTime->start();

    winItem = new QGraphicsPixmapItem(QPixmap(":/pics/res/GameWin.png").scaled(borderOfCharacter.width(), borderOfCharacter.height()));
    winItem->setPos(0, 0);
    winItem->setZValue(110);
    scene->addItem(winItem);
    QFont clearingFont("Arial", 36);
    totalScore = scene->addText(QString("你得到了:") + QString::number(score) + QString("分"), clearingFont);
    totalScore->setDefaultTextColor(QColor(255, 255, 255));
    totalScore->setPos(100, 250);
    totalScore->setZValue(110);
    totalSpend = scene->addText(QString("你撒了") + QString::number(spend) + QString("塊錢"), clearingFont);
    totalSpend->setDefaultTextColor(QColor(255, 255, 255));
    totalSpend->setPos(100, 350);
    totalSpend->setZValue(110);
    totalCard = scene->addText(QString("你刷爆了") + QString::number(card) + QString("張卡"), clearingFont);
    totalCard->setDefaultTextColor(QColor(255, 255, 255));
    totalCard->setPos(100, 450);
    totalCard->setZValue(110);

    for(auto it = enemyBulletList.begin(); it != enemyBulletList.end(); ++it){
        delete (*it);
        (*it) = nullptr;
    }
    for(int i = 0; i < enemyBulletList.count(nullptr); ++i){
        enemyBulletList.removeOne(nullptr);
    }
}

void MainWindow::gameOver(){
    QPixmap overPic(borderOfCharacter.width(), borderOfCharacter.height());
    overPic.fill(Qt::transparent);
    QPainter painter(&overPic);
    painter.fillRect(borderOfCharacter, QColor(0, 0, 0, 100));
    painter.setOpacity(0.7);
    painter.drawPixmap(0, 0, borderOfCharacter.width(), borderOfCharacter.height(), QPixmap(":/pics/res/GameOver.png"));
    overItem = new QGraphicsPixmapItem(overPic);
    overItem->setZValue(110);
    overItem->setPos(0, 0);
    scene->addItem(overItem);
}

void MainWindow::attackHandler(){
    if(player != nullptr){
        if(!playerIsDead){
            if(attack){ // player attacks
                spend += player->attack();
            }
            if(bigOne){
                bool success = dynamic_cast<wallet*>(player)->bigOneAttack();
                if(success){
                    respawnTime->start();
                    card += 5;
                }
                bigOne = false;
            }
        }else{
            bigOne = false;
        }
    }
    if(boss != NULL && respawnTime->elapsed() > 2000){
        boss->attack();
    }
}

void MainWindow::infoBoardHandler(){
    static int playerLife = 3;
    static int playerSpell = 3;
    {
        char temp[100];
        sprintf(temp, "Score:%08d", (int)score);
        scoreText->setPlainText(QString::fromLocal8Bit(temp));

        static int backIndex = 0;
        if(backgroundItem[backIndex]->y() >= borderOfCharacter.height()){
            backgroundItem[backIndex]->setPos(0, -2*sizeOfBackground.y() + borderOfCharacter.height());
            backIndex = 1;
        }
        backgroundItem[0]->setPos(0, backgroundItem[0]->y() + timer->interval() / 10.0);
        backgroundItem[1]->setPos(0, backgroundItem[1]->y() + timer->interval() / 10.0);
    }
    if(player != nullptr && (player->hp != playerLife || dynamic_cast<wallet*>(player)->spells != playerSpell)){
        if(player->hp > playerLife){
            for(int i = playerLife; i < player->hp; ++i){
                lifePainter->drawPixmap(i * 60, 3, QPixmap(":/player/res/Savings.png").scaled(36, 44, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            }
        }else{
//            qDebug() << "erase";
            lifePainter->setCompositionMode(QPainter::CompositionMode_Source);
            lifePainter->fillRect(player->hp * 60, 0, 300, 50, Qt::transparent);
            lifePainter->setCompositionMode(QPainter::CompositionMode_SourceOver);
        }
        playerLife = player->hp;

        int spells = dynamic_cast<wallet*>(player)->spells;
        if(spells > playerSpell){
            for(int i = playerSpell; i < spells; ++i){
                lifePainter->drawPixmap(i * 60, 63, QPixmap(":/bullets/res/card/master.png").scaled(36, 22.5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            }
        }else{
            lifePainter->setCompositionMode(QPainter::CompositionMode_Source);
            lifePainter->fillRect(spells * 60, 50, 300, 50, Qt::transparent);
            lifePainter->setCompositionMode(QPainter::CompositionMode_SourceOver);
        }
        playerSpell = spells;

        life->setPixmap(*lifeCanvas);
    }
    if(boss != nullptr){
        bossHealth->setRect(10, 10, (borderOfCharacter.width() - 20)*((float)boss->hp / boss->initialHp), 10);
    }
}
