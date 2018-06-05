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
    borderX = 1022 - this->boundingRect().width();
    borderY = 766 - this->boundingRect().height();
}

character::~character(){
}

void character::move(int vx, int vy){
    int x = this->x() + vx;
    int y = this->y() - vy;
    if(x > borderX) x = borderX;
    else if(x < 0) x = 0;
    if(y > borderY) y = borderY;
    else if(y < 0) y = 0;
    this->setPos(x, y);
}

gaben_reimu::gaben_reimu():
    character(":/enemy/res/Gaben_Reimu.png", 1000)
{
}

gaben_reimu::~gaben_reimu(){
}

void gaben_reimu::attack(){
}

void gaben_reimu::hit(){
    --hp;
}

void gaben_reimu::died(){
    // some animation
}

wallet::wallet():
    character(":/player/res/Savings.png", 1000)
{
}

wallet::~wallet(){
}

void wallet::attack(){
}

void wallet::hit(){
    --hp;
}

void wallet::died(){
    // some animation;
}
