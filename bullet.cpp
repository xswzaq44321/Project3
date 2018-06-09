#include "bullet.h"

bullet::bullet(const QString &filename, qreal x, qreal y, qreal vx, qreal vy, int size, QGraphicsItem *parent):
    QGraphicsPixmapItem(QPixmap(filename).scaled(size, size)),
    vx(vx),
    vy(vy),
    origin(parent)
{
    this->setPos(x, y);
}

bullet::bullet(const bullet &old):
    QGraphicsPixmapItem(old.pixmap()),
    vx(old.vx),
    vy(old.vy),
    origin(old.origin)
{
    this->setPos(old.x(), old.y());
}

bullet::~bullet(){
//    qDebug() << "bullet dtor";
}

bullet& bullet::operator =(const bullet& r){
    this->setPixmap(r.pixmap());
    this->setPos(r.x(), r.y());
    vx = r.vx;
    vy = r.vy;
    origin = r.origin;
    return *this;
}

void bullet::setVector(qreal vx, qreal vy){
    this->vx = vx;
    this->vy = vy;
}

void bullet::setVectorByPolar(qreal r, qreal theta){
    if(r == 0){
        this->vx = 0;
        this->vy = 0;
        return;
    }
    this->vx = r * qCos(theta);
    this->vy = r * qSin(theta);
}

void bullet::fly(){
    this->setPos(this->x() + vx, this->y() - vy);
    if(!borderOfBullet.contains(this->x(), this->y())){
        this->scene()->removeItem(this);
        delete this;
    }
}

void bullet::tracefly(){
}
