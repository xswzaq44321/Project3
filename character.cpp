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

void character::move(qreal vx, qreal vy){
    vx *= timer->interval() / 10.0;
    vy *= timer->interval() / 10.0;
    qreal x = this->x() + vx;
    qreal y = this->y() - vy;
    if(x > border.right()) x = border.right();
    else if(x < border.x()) x = border.x();
    if(y > border.bottom()) y = border.bottom();
    else if(y < border.y()) y = border.y();
    this->setPosition(x, y);
}

void character::moveTo(qreal x, qreal y, qreal duration){
    static qreal vx = 0;
    static qreal vy = 0;
    static qreal dur = 0;
    if(moveTimer.isActive()){
        disconnect(&moveTimer, 0, 0, 0);
    }
    moveTimer.start(timer->interval());
    moveClock.start();
    vx = (x - this->x()) / duration * 10.0;
    vy = (this->y() - y) / duration * 10.0;
    dur = duration;
    connect(&moveTimer, &QTimer::timeout, [this](void){
        if(moveClock.elapsed() >= dur){
            disconnect(&moveTimer, 0, 0, 0);
            moveTimer.stop();
//            qDebug() << "disconnect";
//            qDebug() << this->x() << this->y();
            return;
        }
        this->move(vx, vy);
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
    bullets.insert(bullets.end(), bullet(":/bullets/res/75%.png", QPointF(0, 0), QPointF(64, 32)));
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
    case 2: case 3:
        if(attackCooldown.elapsed() <= timer->interval() + 2){
            this->moveTo((borderOfCharacter.width() - this->boundingRect().width()) / 2, 40, 1000);
        }
        if(attackCooldown.elapsed() >= 3000){
            if((attackCounter += timer->interval()) % 600 < timer->interval()){
                bullet *b = new bounceBullet(bullets.at(2));
                enemyBulletList.insert(enemyBulletList.end(), b);
                b->setZValue(10);
                b->setPos(this->x() + this->boundingRect().width()/2 - b->boundingRect().width()/2,
                          this->y() + this->boundingRect().height()/2 - b->boundingRect().height()/2 + 50);
                b->setDirection(1.5, -(qrand() % 121 + 30)*(M_PI/180));
                this->scene()->addItem(b);
                connect(timer, &QTimer::timeout, b, &bullet::fly);
            }
            if((attackCounter + 300) % 600 < timer->interval()){
                bullet *b = new bounceBullet(bullets.at(3));
                enemyBulletList.insert(enemyBulletList.end(), b);
                b->setZValue(10);
                b->setPos(this->x() + this->boundingRect().width()/2 - b->boundingRect().width()/2,
                          this->y() + this->boundingRect().height()/2 - b->boundingRect().height()/2 + 50);
                b->setDirection(1.5, -(qrand() % 121 + 30)*(M_PI/180));
                this->scene()->addItem(b);
                connect(timer, &QTimer::timeout, b, &bullet::fly);
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
        attackCooldown.start();
    }

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
                    dynamic_cast<traceBullet*>(b)->setTarget(enemyList.front());
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
