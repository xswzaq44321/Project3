#include "character.h"

character::character():
    QGraphicsPixmapItem(NULL),
    hp(0)
{
    attackCooldown.start();
}

character::character(const QString &filename, int hp_init):
    QGraphicsPixmapItem(QPixmap(filename).scaled(64, 88)),
    hp(hp_init),
    initialHp(hp_init)
{
    attackCooldown.start();
    border.setCoords(0, 0, borderOfCharacter.width() - this->boundingRect().width(), borderOfCharacter.height() - this->boundingRect().height());
}

character::~character(){
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

void character::setPosition(qreal x, qreal y){
    this->setPos(x, y);
}

gaben_reimu::gaben_reimu(int health):
    character(":/enemy/res/Gaben_Reimu.png", health)
{
    this->setZValue(20);
    normalBullets.insert(normalBullets.end(), bullet(":/bullets/res/steam_logo.png", QPointF(0, 0), QPointF(10, 10)));
    normalBullets.insert(normalBullets.end(), bullet(":/bullets/res/50%.png", QPointF(0, 0), QPointF(40, 20)));
}

gaben_reimu::~gaben_reimu(){
    qDebug() << "gaben dtor";
    this->scene()->removeItem(this);
}

void gaben_reimu::attack(QTimer *timer){
    if(attackCooldown.elapsed() >= 3000){
        if((attackCounter += timer->interval()) % 500 < timer->interval()){
            for(int i = 1; i >= -1; i -= 2){
                for(double theta = 0; theta <= 2*M_PI; theta += 2*M_PI/50){
                    bullet *b = new bullet(normalBullets.at(0));
                    enemyBulletList->insert(enemyBulletList->end(), b);
                    b->setZValue(10);
                    b->setPos(this->x() + this->boundingRect().width()/2 - b->boundingRect().width()/2 + 100 * cos(theta),
                              this->y() + this->boundingRect().height()/2 - b->boundingRect().height()/2  - 100 * sin(theta));
                    b->setDirection(1, theta + i*M_PI_2);
                    this->scene()->addItem(b);
                    connect(timer, &QTimer::timeout, b, &bullet::fly);
                }
            }
            {
                bullet *b = new bounceBullet(normalBullets.at(1));
                enemyBulletList->insert(enemyBulletList->end(), b);
                b->setZValue(10);
                b->setPos(this->x() + this->boundingRect().width()/2 - b->boundingRect().width()/2,
                          this->y() + this->boundingRect().height()/2 - b->boundingRect().height()/2 + 50);
                b->setDirection(2, -(qrand() % 121 + 30)*(M_PI/180));
                this->scene()->addItem(b);
                connect(timer, &QTimer::timeout, b, &bullet::fly);
            }
        }
        if(attackCounter > 2000){
            attackCooldown.start();
            attackCounter = 0;
        }
    }
}

bool gaben_reimu::hit(){
    --hp;
//    qDebug() << "ouch! " << hp;
    if(hp == 0){
        return true;
    }
    return false;
}

wallet::wallet():
    character(":/player/res/Savings.png", 3),
    heart(new QGraphicsPixmapItem(QPixmap(":/player/res/heart.png").scaled(10, 10)))
{
    this->border.setCoords(-(this->boundingRect().width() - this->heart->boundingRect().width()) / 2,
                           -(this->boundingRect().height() - this->heart->boundingRect().height()) / 2,
                           borderOfCharacter.width() - (this->boundingRect().width() + this->heart->boundingRect().width()) / 2,
                           borderOfCharacter.height() - (this->boundingRect().height() + this->heart->boundingRect().height()) / 2);
    this->setZValue(0);
    this->heart->setZValue(30);
    normalBullets.insert(normalBullets.end(), bullet(":/bullets/res/NTD/10_dollor.png", QPointF(10, M_PI_2), QPointF(30, 30), true));
    normalBullets.insert(normalBullets.end(), bullet(":/bullets/res/NTD/10_dollor.png", QPointF(10, M_PI_2 + 0.09), QPointF(30, 30), true));
    normalBullets.insert(normalBullets.end(), bullet(":/bullets/res/NTD/10_dollor.png", QPointF(10, M_PI_2 - 0.09), QPointF(30, 30), true));
    traceBullets.insert(traceBullets.end(), bullet(":/bullets/res/NTD/50_dollor.png", QPointF(10, M_PI_2 - 0.18), QPointF(30, 30), true));
    traceBullets.insert(traceBullets.end(), bullet(":/bullets/res/NTD/50_dollor.png", QPointF(10, M_PI_2 + 0.18), QPointF(30, 30), true));
}

wallet::~wallet(){
    this->scene()->removeItem(this->heart);
    this->scene()->removeItem(this);
}

void wallet::attack(QTimer *timer){
    if(attackCooldown.elapsed() >= 40){
        for(QList<bullet>::iterator it = normalBullets.begin(); it != normalBullets.end(); ++it){
            bullet *b = new bullet(*it);
            myBulletList->insert(myBulletList->end(), b);
            b->setPos(this->x() + this->boundingRect().width() / 2 - b->boundingRect().width() / 2, this->y());
            this->scene()->addItem(b);
            connect(timer, &QTimer::timeout, b, &bullet::fly);
        }
        if(++attackCounter > 2){
            for(int i = 0; i < 2; ++i){
                bullet *b = new traceBullet(traceBullets[i]);
                myBulletList->insert(myBulletList->end(), b);
                if(i){
                    b->setPos(this->x() - 10 - b->boundingRect().width(), this->y() + (this->boundingRect().height() - b->boundingRect().height()) / 2);
                }else{
                    b->setPos(this->x() + this->boundingRect().width() + 10, this->y() + (this->boundingRect().height() - b->boundingRect().height()) / 2);
                }
                this->scene()->addItem(b);
                if(enemyList->size() > 0){
                    b->setTarget(enemyList->front());
                }
                connect(timer, &QTimer::timeout, b, &bullet::fly);
            }
            attackCounter = 0;
        }
        attackCooldown.start();
    }
}

void wallet::setPosition(qreal x, qreal y){
    this->setPos(x, y);
    this->heart->setPos(x + (this->boundingRect().width() - this->heart->boundingRect().width()) / 2,
                        y + (this->boundingRect().height() - this->heart->boundingRect().height()) / 2);
}

bool wallet::hit(){
    --hp;
    qDebug() << "player hp = " << hp;
    if(hp == 0){
        return true;
    }
    return false;
}
