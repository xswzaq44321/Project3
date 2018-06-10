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

gaben_reimu::gaben_reimu():
    character(":/enemy/res/Gaben_Reimu.png", 1000)
{
    this->setZValue(20);
    normalBullets.insert(normalBullets.end(), bullet(":/bullets/res/steam_logo.png", 0, 0, 0, 0, 15));
}

gaben_reimu::~gaben_reimu(){
    qDebug() << "gaben dtor";
    this->scene()->removeItem(this);
}

void gaben_reimu::attack(QTimer *timer){
    if(attackCooldown.elapsed() >= 2000){
        if(attackCooldown.elapsed() <= 3000){
            if((attackCounter += timer->interval()) % 200){
                return;
            }
            for(double theta = 0; theta <= 2*M_PI; theta += 2*M_PI/50){
                bullet *b = new bullet(normalBullets.at(0));
                b->setZValue(10);
                b->setPos(this->x() + this->boundingRect().width()/2 + 100 * cos(theta),
                          this->y() + this->boundingRect().height()/2 - 100 * sin(theta));
                b->setVectorByPolar(5, theta + M_PI/2);
                this->scene()->addItem(b);
                connect(timer, &QTimer::timeout, b, &bullet::fly);
            }
        }else{
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
    normalBullets.insert(normalBullets.end(), bullet(":/bullets/res/USD/50_cent.png", 0, 0, 0, 10, 30, this));
    normalBullets.insert(normalBullets.end(), bullet(":/bullets/res/USD/50_cent.png", 0, 0, 1, 10, 30, this));
    normalBullets.insert(normalBullets.end(), bullet(":/bullets/res/USD/50_cent.png", 0, 0, -1, 10, 30, this));
}

wallet::~wallet(){
    this->scene()->removeItem(this->heart);
    this->scene()->removeItem(this);
}

void wallet::attack(QTimer *timer){
    if(attackCooldown.elapsed() >= 40){
        for(QList<bullet>::iterator it = normalBullets.begin(); it != normalBullets.end(); ++it){
            bullet *b = new bullet(static_cast<bullet>(*it));
            b->setPos(this->x() + this->boundingRect().width() / 2 - b->boundingRect().width() / 2, this->y());
            this->scene()->addItem(b);
            connect(timer, &QTimer::timeout, b, &bullet::fly);
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
}
