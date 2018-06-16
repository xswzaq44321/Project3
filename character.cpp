#include "character.h"

character::character():
    QGraphicsPixmapItem(NULL),
    hp(0)
{
    attackCooldown.start();
    spellCooldown.start();
}

character::character(const QString &filename, int hp_init):
    QGraphicsPixmapItem(QPixmap(filename).scaled(64, 88, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)),
    hp(hp_init),
    initialHp(hp_init)
{
    attackCooldown.start();
    spellCooldown.start();
    border.setCoords(0, 0, borderOfCharacter.width() - this->boundingRect().width(), borderOfCharacter.height() - this->boundingRect().height());
}

character::~character(){
    if(moveTimer.isActive()){
        disconnect(&moveTimer);
    }
}

void character::move(qreal vx, qreal vy, bool ignore){
    vx *= timer->interval() / 10.0;
    vy *= timer->interval() / 10.0;
    qreal x = this->x() + vx;
    qreal y = this->y() - vy;
    if(!ignore){
        if(x > border.right()) x = border.right();
        else if(x < border.x()) x = border.x();
        if(y > border.bottom()) y = border.bottom();
        else if(y < border.y()) y = border.y();
    }
    this->setPosition(x, y);
}

void character::moveTo(qreal x, qreal y, qreal duration, bool ignore){
    if(moveTimer.isActive()){
        disconnect(&moveTimer, 0, 0, 0);
    }
    moveTimer.start(timer->interval());
    moveClock.start();
    qreal vx = (x - this->x()) / duration * 10.0;
    qreal vy = (this->y() - y) / duration * 10.0;
    qreal dur = duration;
    connect(&moveTimer, &QTimer::timeout, [this, vx, vy, dur, ignore](void){
        if(moveClock.elapsed() >= dur){
            disconnect(&moveTimer, 0, 0, 0);
            moveTimer.stop();
//            qDebug() << "disconnect";
//            qDebug() << this->x() << this->y();
            return;
        }
        this->move(vx, vy, ignore);
    });
}

void character::setPosition(qreal x, qreal y){
    this->setPos(x, y);
}

gaben_reimu::gaben_reimu(int health):
    character(":/enemy/res/Gaben_Reimu.png", health)
{
    this->setZValue(20);
    bullets.insert(bullets.end(), bullet(":/bullets/res/steam_logo.png", QPointF(0, 0), QPointF(15, 15)));
    bullets.insert(bullets.end(), bullet(":/bullets/res/50%.png", QPointF(0, 0), QPointF(40, 20)));
    bullets.insert(bullets.end(), bullet(":/bullets/res/66%.png", QPointF(0, 0), QPointF(48, 24)));
    bullets.insert(bullets.end(), bullet(":/bullets/res/75%.png", QPointF(0, 0), QPointF(56, 28)));
    bullets.insert(bullets.end(), bullet(":/bullets/res/90%.png", QPointF(0, 0), QPointF(64, 32)));
}

gaben_reimu::~gaben_reimu(){
    qDebug() << "gaben dtor";
    this->scene()->removeItem(this);
}

int gaben_reimu::attack(){
    switch(phase){
    case 1:
        if(attackCooldown.elapsed() >= 3000){
            if(attackCounter == 0){
                this->moveTo((borderOfCharacter.width() - this->boundingRect().width()) / 2 + (qrand() % 3 - 1)*150, 40, 1000);
            }
            if((attackCounter += timer->interval()) % 1000 < timer->interval()){
                for(int i = 1; i >= -1; i -= 2){
                    for(double theta = 0; theta <= 2*M_PI; theta += 2*M_PI/50){
                        bullet *b = new bullet(bullets.at(0));
                        enemyBulletList.insert(enemyBulletList.end(), b);
                        b->setZValue(10);
                        b->setPos(this->x() + this->boundingRect().width()/2 - b->boundingRect().width()/2 + 100 * cos(theta),
                                  this->y() + this->boundingRect().height()/2 - b->boundingRect().height()/2  - 100 * sin(theta));
                        b->setDirection(1, theta + i*M_PI_2);
                        this->scene()->addItem(b);
                        connect(timer, &QTimer::timeout, b, &bullet::fly);
                    }
                }
                {
                    double dx =(player->x() + player->boundingRect().width() / 2) - (this->x() + this->boundingRect().width() / 2);
                    double dy =(this->y() + this->boundingRect().height() / 2) - (player->y() + player->boundingRect().height() / 2);
                    double direction = qAtan2(dy, dx);
//                    qDebug() << direction;
                    bullet *b = new bounceBullet(bullets.at(1));
                    enemyBulletList.insert(enemyBulletList.end(), b);
                    b->setZValue(10);
                    b->setPos(this->x() + this->boundingRect().width()/2 - b->boundingRect().width()/2,
                              this->y() + this->boundingRect().height()/2 - b->boundingRect().height()/2 + 50);
                    b->setDirection(1.5, direction + qDegreesToRadians((double)(qrand() % 121 - 60)));
                    this->scene()->addItem(b);
                    connect(timer, &QTimer::timeout, b, &bullet::fly);
                }
            }
            if(attackCounter > 3000){
                attackCooldown.start();
                attackCounter = 0;
            }
        }
        break;
    case 2:
        if(attackCounter % 1000 < timer->interval()){
            this->moveTo((borderOfCharacter.width() - this->boundingRect().width()) / 2 + (qrand() % 3 - 1)*150, 40 + (qrand()%3) * 30, 1000);
        }
        if(attackCooldown.elapsed() >= 3000){
            if((attackCounter += timer->interval()) % 600 < timer->interval()){
                double dx =(player->x() + player->boundingRect().width() / 2) - (this->x() + this->boundingRect().width() / 2);
                double dy =(this->y() + this->boundingRect().height() / 2) - (player->y() + player->boundingRect().height() / 2);
                double direction = qAtan2(dy, dx);
                bullet *b = new bounceBullet(bullets.at(2));
                enemyBulletList.insert(enemyBulletList.end(), b);
                b->setZValue(10);
                b->setPos(this->x() + this->boundingRect().width()/2 - b->boundingRect().width()/2,
                          this->y() + this->boundingRect().height()/2 - b->boundingRect().height()/2 + 50);
                b->setDirection(1.5, direction + qDegreesToRadians((double)(qrand() % 121 - 60)));
                this->scene()->addItem(b);
                connect(timer, &QTimer::timeout, b, &bullet::fly);
            }
            if((attackCounter + 300) % 600 < timer->interval()){
                double dx =(player->x() + player->boundingRect().width() / 2) - (this->x() + this->boundingRect().width() / 2);
                double dy =(this->y() + this->boundingRect().height() / 2) - (player->y() + player->boundingRect().height() / 2);
                double direction = qAtan2(dy, dx);
                bullet *b = new bounceBullet(bullets.at(3));
                enemyBulletList.insert(enemyBulletList.end(), b);
                b->setZValue(10);
                b->setPos(this->x() + this->boundingRect().width()/2 - b->boundingRect().width()/2,
                          this->y() + this->boundingRect().height()/2 - b->boundingRect().height()/2 + 50);
                b->setDirection(1.5, direction + qDegreesToRadians((double)(qrand() % 121 - 60)));
                this->scene()->addItem(b);
                connect(timer, &QTimer::timeout, b, &bullet::fly);
            }
        }
        break;
    case 3:
        if(minion != nullptr && !enemyList.contains(minion)){
            enemyList.insert(enemyList.end(), minion);
            minion = nullptr;
        }
        if(attackCooldown.elapsed() >= 3000){
            if(attackCounter < timer->interval()){
                if(enemyList.size() == 1){
                    this->moveTo((borderOfCharacter.width() - this->boundingRect().width()) / 2, 40, 1000);
                }else{
                    this->moveTo((borderOfCharacter.width() - this->boundingRect().width()) / 4 + (qrand() % 3 - 1)*100, 40 + (qrand()%3)*20, 1000);
                }
            }
            if((attackCounter += timer->interval()) % 600 < timer->interval()){
                qreal theta = qDegreesToRadians((double)(qrand() % 359 - 179));
                bullet *b = new aimBullet(bullets.at(qrand() % 4 + 1));
                enemyBulletList.insert(enemyBulletList.end(), b);
                b->setZValue(10);
                b->setPos(this->x() + this->boundingRect().width()/2 - b->boundingRect().width()/2 + 100*cos(theta),
                          this->y() + this->boundingRect().height()/2 - b->boundingRect().height()/2 + 100*sin(theta));
                b->setDirection(1.5, 0);
                dynamic_cast<aimBullet*>(b)->setTarget(player);
                this->scene()->addItem(b);
                connect(timer, &QTimer::timeout, b, &bullet::fly);
            }
            if(enemyList.size() == 1 && attackCounter % 16 < timer->interval()){
                qreal s, a, b, m, n, r_1, r_2;
                r_1 = 5;
                r_2 = 2;
                n = 1;
                s = r_1 - r_2;
                m = r_1/r_2 - 1;
                a = qDegreesToRadians((double)((attackCounter/2) % 1440));
                b = m * a;
                bullet *bul = new bullet(bullets.at(0));
                enemyBulletList.insert(enemyBulletList.end(), bul);
                bul->setZValue(10);
                bul->setPos(this->x() + this->boundingRect().width()/2 - bul->boundingRect().width()/2 + 25*(s*cos(a) + n*r_2*cos(b)),
                          this->y() + this->boundingRect().height()/2 - bul->boundingRect().height()/2 - 25*(s*sin(a) - n*r_2*sin(b)));
                bul->setDirection(1, -b);
                this->scene()->addItem(bul);
                connect(timer, &QTimer::timeout, bul, &bullet::fly);
            }
            if(attackCounter >= 2880){
                attackCounter = 0;
            }
        }
        break;
    }
    return 0;
}

bool gaben_reimu::hit(qreal damage){
    hp -= damage;
//    qDebug() << "ouch! " << hp;

    if(phase == 1 && hp <= initialHp * 2 / 3){
        phase = 2;
        this->moveTo((borderOfCharacter.width() - this->boundingRect().width()) / 2, 0 + 40, 500);
        attackCooldown.start();
    }else if(phase == 2 && hp <= initialHp / 3){
        phase = 3;
        this->moveTo((borderOfCharacter.width() - this->boundingRect().width()) / 3, 0 + 40, 500);
        attackCooldown.start();

        minion = new gaben(500);
        this->scene()->addItem(minion);
        minion->setPosition((borderOfCharacter.width() - this->boundingRect().width()) / 3 * 2, 0 - minion->boundingRect().height());
        minion->moveTo(minion->x(), 40, 2000, true);
    }

    if(hp <= 0){
        return true;
    }
    return false;
}

gaben::gaben(int health):
    character(":/enemy/res/gabe_newell.png", health)
{
    this->setZValue(20);
    bullets.insert(bullets.end(), bullet(":/bullets/res/steam_logo.png", QPointF(0, 0), QPointF(15, 15)));
    bullets.insert(bullets.end(), bullet(":/bullets/res/90%.png", QPointF(0, 0), QPointF(64, 32)));
}

gaben::~gaben()
{
    qDebug() << "minion dtor";
    this->scene()->removeItem(this);
}

int gaben::attack(){
    if(attackCooldown.elapsed() >= 3000){
        if(attackCounter >= 2000){
            this->moveTo((borderOfCharacter.width() - this->boundingRect().width()) / 4 * 3 + (qrand() % 3 - 1)*100, 40 + (qrand()%3)*20, 1000);
            attackCounter = 0;
        }
        if((attackCounter += timer->interval()) % 1000 < timer->interval()){
            for(double theta = 0; theta <= 2*M_PI; theta += 2*M_PI/15){
                bullet *b = new bullet(bullets.at(0));
                enemyBulletList.insert(enemyBulletList.end(), b);
                b->setZValue(10);
                b->setPos(this->x() + this->boundingRect().width()/2 - b->boundingRect().width()/2 + 100 * cos(theta),
                          this->y() + this->boundingRect().height()/2 - b->boundingRect().height()/2  - 100 * sin(theta));
                b->setDirection(1, theta);
                this->scene()->addItem(b);
                connect(timer, &QTimer::timeout, b, &bullet::fly);
            }
            for(double theta = 2*M_PI/30; theta <= 2*M_PI; theta += 2*M_PI/15){
                bullet *b = new bullet(bullets.at(1));
                enemyBulletList.insert(enemyBulletList.end(), b);
                b->setZValue(10);
                b->setPos(this->x() + this->boundingRect().width()/2 - b->boundingRect().width()/2 + 100 * cos(theta),
                          this->y() + this->boundingRect().height()/2 - b->boundingRect().height()/2  - 100 * sin(theta));
                b->setDirection(1, theta);
                this->scene()->addItem(b);
                connect(timer, &QTimer::timeout, b, &bullet::fly);
            }
        }
    }
    return 0;
}

bool gaben::hit(qreal damage){
//    qDebug() << "minion gaben hit" << hp;
    hp -= damage;
    if(hp <= 0){
        return true;
    }
    return false;
}

wallet::wallet():
    character(":/player/res/Savings.png", 3),
    heart(new QGraphicsPixmapItem(QPixmap(":/player/res/heart.png").scaled(8, 8, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)))
{
    this->border.setCoords(-(this->boundingRect().width() - this->heart->boundingRect().width()) / 2,
                           -(this->boundingRect().height() - this->heart->boundingRect().height()) / 2,
                           borderOfCharacter.width() - (this->boundingRect().width() + this->heart->boundingRect().width()) / 2,
                           borderOfCharacter.height() - (this->boundingRect().height() + this->heart->boundingRect().height()) / 2);
    this->setZValue(0);
    this->heart->setZValue(30);
    bullets.insert(bullets.end(), bullet(":/bullets/res/NTD/10_dollor.png", QPointF(10, M_PI_2), QPointF(30, 30), this));
    bullets.insert(bullets.end(), bullet(":/bullets/res/NTD/10_dollor.png", QPointF(10, M_PI_2 + 0.09), QPointF(30, 30), this));
    bullets.insert(bullets.end(), bullet(":/bullets/res/NTD/10_dollor.png", QPointF(10, M_PI_2 - 0.09), QPointF(30, 30), this));
    bullets.insert(bullets.end(), bullet(":/bullets/res/NTD/50_dollor.png", QPointF(10, M_PI_2 - 0.18), QPointF(30, 30), this));
    bullets.insert(bullets.end(), bullet(":/bullets/res/NTD/50_dollor.png", QPointF(10, M_PI_2 + 0.18), QPointF(30, 30), this));
    missiles.insert(missiles.end(), bullet(":/bullets/res/card/citi.png", QPointF(7.5, 0), QPointF(80, 50), this));
    missiles.insert(missiles.end(), bullet(":/bullets/res/card/master.png", QPointF(7.5, 0), QPointF(80, 50), this));
    missiles.insert(missiles.end(), bullet(":/bullets/res/card/standard.png", QPointF(7.5, 0), QPointF(80, 50), this));
    missiles.insert(missiles.end(), bullet(":/bullets/res/card/visa.png", QPointF(7.5, 0), QPointF(80, 50), this));
}

wallet::~wallet(){
    qDebug() << "wallet dtor";
    this->scene()->removeItem(this->heart);
    this->scene()->removeItem(this);
    delete heart;
}

int wallet::attack(){
    int spend = 0;
    if(attackCooldown.elapsed() >= 40){
        for(int i = 0; i <= 2; ++i){
            bullet *b = new bullet(bullets.at(i));
            myBulletList.insert(myBulletList.end(), b);
            b->setPos(this->x() + this->boundingRect().width() / 2 - b->boundingRect().width() / 2, this->y());
            this->scene()->addItem(b);
            connect(timer, &QTimer::timeout, b, &bullet::fly);
            spend += 10;
        }
        if(++attackCounter > 3){
            for(int i = 3; i <= 4; ++i){
                bullet *b = new traceBullet(bullets.at(i));
                myBulletList.insert(myBulletList.end(), b);
                if(i == 3){
                    b->setPos(this->x() - 10 - b->boundingRect().width(), this->y() + (this->boundingRect().height() - b->boundingRect().height()) / 2);
                }else{
                    b->setPos(this->x() + this->boundingRect().width() + 10, this->y() + (this->boundingRect().height() - b->boundingRect().height()) / 2);
                }
                this->scene()->addItem(b);
                if(enemyList.size() > 0){
                    dynamic_cast<traceBullet*>(b)->setTarget(enemyList.back());
                }
                connect(timer, &QTimer::timeout, b, &bullet::fly);
                spend += 50;
            }
            attackCounter = 0;
        }
        attackCooldown.start();
    }
    return spend;
}

bool wallet::bigOneAttack(){
    if(spells > 0 && spellCooldown.elapsed() >= 4000){
        for(int i = 0; i < 5; ++i){
            bullet *b = new missile(missiles.at(qrand() % missiles.size()));
            missileList.insert(missileList.end(), b);
            b->setPos(this->x() + this->boundingRect().width() / 2 - b->boundingRect().width() / 2 + 50*cos(2*M_PI * (i / 5.0)),
                      this->y() + this->boundingRect().height() / 2 - b->boundingRect().height() / 2 + 50*sin(2*M_PI * (i / 5.0)));
            b->setDirection(7.5, qDegreesToRadians((double)(qrand() % 361 - 180)));
            this->scene()->addItem(b);
            connect(timer, &QTimer::timeout, b, &bullet::fly);
        }
        --spells;
        spellCooldown.start();
        return true;
    }
    return false;
}

void wallet::setPosition(qreal x, qreal y){
    this->setPos(x, y);
    this->heart->setPos(x + (this->boundingRect().width() - this->heart->boundingRect().width()) / 2,
                        y + (this->boundingRect().height() - this->heart->boundingRect().height()) / 2);
}

bool wallet::hit(qreal damage){
    hp -= damage;
    qDebug() << "player hp = " << hp;
    if(hp == 0){
        return true;
    }
    return false;
}
