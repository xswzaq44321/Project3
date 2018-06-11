#include "bullet.h"

bullet::bullet(const QString &filename, QPointF polar, QPointF picSize, bool from_player):
    QGraphicsPixmapItem(QPixmap(filename).scaled(picSize.x(), picSize.y())),
    target(nullptr),
    fromPlayer(from_player),
    r(polar.x()),
    theta(polar.y())
{
    this->setPos(-1, -1);
}

bullet::bullet(const bullet &old):
    QObject(),
    QGraphicsPixmapItem(old.pixmap()),
    target(nullptr),
    fromPlayer(old.fromPlayer),
    r(old.r),
    theta(old.theta)
{
    this->setPos(old.x(), old.y());
}

bullet::~bullet(){
//    qDebug() << "bullet dtor";
    if(fromPlayer){
        auto index = std::find(myBulletList->begin(), myBulletList->end(), this);
        *index = nullptr;
    }else{
        auto index = std::find(enemyBulletList->begin(), enemyBulletList->end(), this);
        *index = nullptr;
    }
}

bullet& bullet::operator =(const bullet& R){
    this->setPixmap(R.pixmap());
    this->setPos(R.x(), R.y());
    target = R.target;
    fromPlayer = R.fromPlayer;
    r = R.r;
    theta = R.theta;
    return *this;
}

void bullet::setPolar(qreal r, qreal theta){
    this->r = r;
    this->theta = theta;
}

void bullet::fly(){
    this->setPos(this->x() + r * qCos(theta), this->y() - r * qSin(theta));
    if(!borderOfBullet.contains(this->x(), this->y())){
        this->scene()->removeItem(this);
        delete this;
    }
}

void bullet::setTarget(QGraphicsItem *target){
    this->target = target;
}

traceBullet::traceBullet():
    bullet(":/pics/res/error.png", QPointF(10, M_PI_2), QPointF(30, 30), true)
{
}

traceBullet::traceBullet(const QString &filename, QPointF polar, QPointF picSize):
    bullet(filename, polar, picSize, true)
{
}

traceBullet::traceBullet(const bullet &old):
    bullet(old)
{
}

void traceBullet::fly(){
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
        this->setPolar(r, theta + (direction - theta) / 3);
//        qDebug() << (target->y() - this->y()) << (target->x() - this->x());
    }
    this->setPos(this->x() + r * qCos(theta), this->y() - r * qSin(theta));
    if(!borderOfBullet.contains(this->x(), this->y())){
        this->scene()->removeItem(this);
        delete this;
    }
}

bounceSale::bounceSale():
    bullet(":/pics/res/error.png")
{
}

bounceSale::bounceSale(const QString &filename, QPointF polar, QPointF picSize):
    bullet(filename, polar, picSize)
{
}

bounceSale::bounceSale(const bullet &old):
    bullet(old)
{
}

void bounceSale::fly(){
    this->setPos(this->x() + r * qCos(theta), this->y() - r * qSin(theta));
    if(this->x() < 0 || this->x() > (borderOfCharacter.width() - this->boundingRect().width())){
        theta = theta - 2*(M_PI_2 - fabs(theta));
    }
    if(!borderOfBullet.contains(this->x(), this->y())){
        this->scene()->removeItem(this);
        delete this;
    }
}
