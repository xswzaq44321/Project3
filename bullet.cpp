#include "bullet.h"

bullet::bullet(const QString &filename, qreal x, qreal y, qreal vx, qreal vy, int size):
    QGraphicsPixmapItem(QPixmap(filename).scaled(size, size)),
    vx(vx),
    vy(vy)
{
    this->setPos(x, y);
}

bullet::bullet(const bullet &old):
    QGraphicsPixmapItem(old.pixmap()),
    vx(old.vx),
    vy(old.vy)
{
    this->setPos(old.x(), old.y());
}

bullet::~bullet(){
    qDebug() << "dtor";
}

bullet& bullet::operator =(const bullet& r){
    this->setPixmap(r.pixmap());
    this->setPos(r.x(), r.y());
    vx = r.vx;
    vy = r.vy;
    return *this;
}

void bullet::setVector(qreal vx, qreal vy){
    this->vx = vx;
    this->vy = vy;
}

bool bullet::fly(const QGraphicsPixmapItem *enemy){
    this->setPos(this->x() + vx, this->y() - vy);
    if(this->collidesWithItem(enemy)){
        this->scene()->removeItem(this);
        delete this;
        return true;
    }
    if(!borderOfBullet.contains(this->x(), this->y())){
        this->scene()->removeItem(this);
        delete this;
        return false;
    }
    return false;
}

void bullet::tracefly(){
}
