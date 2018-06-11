#include "bullet.h"

bullet::bullet(const QString &filename, qreal x, qreal y, qreal r, qreal theta, int size, QGraphicsItem *parent):
    QGraphicsPixmapItem(QPixmap(filename).scaled(size, size)),
    r(r),
    theta(theta),
    origin(parent),
    target(nullptr)
{
    this->setPos(x, y);
}

bullet::bullet(const bullet &old):
    QGraphicsPixmapItem(old.pixmap()),
    r(old.r),
    theta(old.theta),
    origin(old.origin),
    target(nullptr)
{
    this->setPos(old.x(), old.y());
}

bullet::~bullet(){
//    qDebug() << "bullet dtor";
}

bullet& bullet::operator =(const bullet& R){
    this->setPixmap(R.pixmap());
    this->setPos(R.x(), R.y());
    r = R.r;
    theta = R.theta;
    origin = R.origin;
    target = R.target;
    return *this;
}

void bullet::setPolar(qreal r, qreal theta){
    this->r = r;
    this->theta = theta;
}

void bullet::fly(){
    if(!enemyList->contains(target)){
        target = nullptr;
    }
    if(target != nullptr){
        double dx =(target->x() + target->boundingRect().width() / 2) - (this->x() + this->boundingRect().width() / 2);
        double dy =(this->y() + this->boundingRect().height() / 2) - (target->y() + target->boundingRect().height() / 2);
        double direction = qAtan(dy / dx);
        if(dx < 0){
            direction += M_PI;
        }
        this->setPolar(r, theta + (direction - theta) / 1);
//        qDebug() << (target->y() - this->y()) << (target->x() - this->x());
    }
    this->setPos(this->x() + r * qCos(theta), this->y() - r * qSin(theta));
    if(!borderOfBullet.contains(this->x(), this->y())){
        this->scene()->removeItem(this);
        delete this;
    }
}

void bullet::setTarget(QGraphicsItem *target){
    this->target = target;
}
