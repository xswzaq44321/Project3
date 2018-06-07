#include "character.h"

character::character():
    QGraphicsPixmapItem(NULL),
    hp(0)
{
}

character::character(const QString &filename, int hp_init):
    QGraphicsPixmapItem(QPixmap(filename).scaled(64, 88)),
    hp(hp_init)
{
    borderX = borderOfCharacter.width() - this->boundingRect().width();
    borderY = borderOfCharacter.height() - this->boundingRect().height();
}

character::~character(){
}

bool character::isdead(){
    return (hp <= 0 ? true : false);
}

void character::move(qreal vx, qreal vy){
    qreal x = this->x() + vx;
    qreal y = this->y() - vy;
    if(x > borderX) x = borderX;
    else if(x < 0) x = 0;
    if(y > borderY) y = borderY;
    else if(y < 0) y = 0;
    this->setPos(x, y);
}

gaben_reimu::gaben_reimu():
    character(":/enemy/res/Gaben_Reimu.png", 1000)
{
    normalBullets.insert(normalBullets.end(), bullet(":/bullets/res/steam_logo.png", 0, 0, 0, 0, 15));
}

gaben_reimu::~gaben_reimu(){
}

void gaben_reimu::attack(QTimer *timer, character *enemy){
}

void gaben_reimu::hit(){
    --hp;
    qDebug() << "ouch! " << hp;
    if(hp == 0){
        this->scene()->removeItem(this);
        this->setPos(-1000, -1000);
    }
}

wallet::wallet():
    character(":/player/res/Savings.png", 1000)
{
    normalBullets.insert(normalBullets.end(), bullet(":/bullets/res/USD/50_cent.png", 0, 0, 0, 10, 30));
    normalBullets.insert(normalBullets.end(), bullet(":/bullets/res/USD/50_cent.png", 0, 0, 1, 10, 30));
    normalBullets.insert(normalBullets.end(), bullet(":/bullets/res/USD/50_cent.png", 0, 0, -1, 10, 30));
}

void wallet::attack(QTimer *timer, character *enemy){
    for(QVector<bullet>::iterator it = normalBullets.begin(); it != normalBullets.end(); ++it){
        bullet *b = new bullet(static_cast<bullet>(*it));
        b->setPos(this->x() + this->boundingRect().width() / 2 - b->boundingRect().width() / 2, this->y());
        this->scene()->addItem(b);
        connect(timer, &QTimer::timeout, b, [this, b, enemy]{
            bool hit = b->fly(enemy);
            if(hit){
                enemy->hit();
                qDebug() << "hit";
            }
        });
    }
}

wallet::~wallet(){
}

void wallet::hit(){
    --hp;
}
