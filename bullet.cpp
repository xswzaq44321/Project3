#include "bullet.h"

bullet::bullet(const QString &filename = nullptr, int x = -1, int y = -1, int vx = 0, int vy = 0):
    QGraphicsPixmapItem(QPixmap(filename).scaled(15, 15)),
    vx(vx),
    vy(vy)
{
    this->setPos(x, y);
    borderOfScene = new QRectF(-100, -100, 1122, 866);
}

bullet::~bullet(){}

void bullet::fly(){
    qDebug() << "I beleve I can fly~";
    this->setPos(this->x() + vx, this->y() - vy);
    if(!borderOfScene->contains(this->x(), this->y())){
        this->scene()->removeItem(this);
        delete this;
    }
}

void bullet::tracefly(){
}
