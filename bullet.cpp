#include "bullet.h"

bullet::bullet(const QString &filename, QPointF polar, QPointF picSize, QGraphicsItem *who):
    QGraphicsPixmapItem(QPixmap(filename).scaled(picSize.x(), picSize.y())),
    origin(who),
    r(polar.x() * timer->interval() / 10.0),
    theta(polar.y())
{
    this->setPos(-1, -1);
}

bullet::bullet(const bullet &old):
    QObject(),
    QGraphicsPixmapItem(old.pixmap()),
    origin(old.origin),
    r(old.r),
    theta(old.theta)
{
    this->setPos(old.x(), old.y());
}

bullet::~bullet(){
//    qDebug() << "bullet dtor";
    if(static_cast<void*>(origin) == static_cast<void*>(player)){
        auto index = std::find(myBulletList->begin(), myBulletList->end(), this);
        if(index != myBulletList->end()){
            *index = nullptr;
            return;
        }
        index = std::find(missileList->begin(), missileList->end(), this);
        if(index != missileList->end()){
            *index = nullptr;
            return;
        }
    }else{
        auto index = std::find(enemyBulletList->begin(), enemyBulletList->end(), this);
        *index = nullptr;
    }
}

bullet& bullet::operator =(const bullet& R){
    this->setPixmap(R.pixmap());
    this->setPos(R.x(), R.y());
    origin = R.origin;
    r = R.r;
    theta = R.theta;
    return *this;
}

void bullet::setDirection(qreal r, qreal theta){
    r *= timer->interval() / 10.0;
    this->r = r;
    this->theta = theta;
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

traceBullet::traceBullet():
    bullet(":/pics/res/error.png")
{
}

traceBullet::traceBullet(const QString &filename, QPointF polar, QPointF picSize, QGraphicsItem *who):
    bullet(filename, polar, picSize, who)
{
}

traceBullet::traceBullet(const bullet &old):
    bullet(old)
{
}

void traceBullet::setTarget(QGraphicsItem *target){
    this->target = target;
}

void traceBullet::fly(){
    if(!enemyList->contains(target)){
        target = nullptr;
    }
    if(target != nullptr){
        double dx =(target->x() + target->boundingRect().width() / 2) - (this->x() + this->boundingRect().width() / 2);
        double dy =(this->y() + this->boundingRect().height() / 2) - (target->y() + target->boundingRect().height() / 2);
        double direction = qAtan2(dy, dx);
        double error = direction - theta;
        if(error > M_PI) error -= 2*M_PI;
        this->setPolar(r, theta + (error) / 1);
//        qDebug() << direction;
    }
    this->setPos(this->x() + r * qCos(theta), this->y() - r * qSin(theta));
    if(!borderOfBullet.contains(this->x(), this->y())){
        this->scene()->removeItem(this);
        delete this;
    }
}

bounceBullet::bounceBullet():
    bullet()
{
}

bounceBullet::bounceBullet(const QString &filename, QPointF polar, QPointF picSize, QGraphicsItem *who):
    bullet(filename, polar, picSize, who)
{
}

bounceBullet::bounceBullet(const bullet &old):
    bullet(old)
{
}

void bounceBullet::fly(){
    this->setPos(this->x() + r * qCos(theta), this->y() - r * qSin(theta));
    if(this->x() < 0 || this->x() > (borderOfCharacter.width() - this->boundingRect().width())){
        theta = theta - 2*(M_PI_2 - fabs(theta));
    }
    if(!borderOfBullet.contains(this->x(), this->y())){
        this->scene()->removeItem(this);
        delete this;
    }
}

missile::missile():
    bullet()
{
    liveTime.start();
}

missile::missile(const QString &filename, QPointF polar, QPointF picSize, QGraphicsItem *who):
    bullet(filename, polar, picSize, who)
{
    liveTime.start();
}

missile::missile(const bullet &old):
    bullet(old)
{
    liveTime.start();
}

void missile::setTarget(QGraphicsItem *target){
    this->target = target;
}

void missile::fly(){
    if(liveTime.elapsed() < 1000){
        target = origin;
    }else{
        target = nullptr;
        if(boss != nullptr){
            target = static_cast<QGraphicsItem*>(boss);
        }else if(enemyList->size() > 0){
            target = enemyList->front();
        }
    }
    if(target != nullptr){
        double dx =(target->x() + target->boundingRect().width() / 2) - (this->x() + this->boundingRect().width() / 2);
        double dy =(this->y() + this->boundingRect().height() / 2) - (target->y() + target->boundingRect().height() / 2);
        double direction = qAtan2(dy, dx);
        double error = direction - theta;
        if(error > M_PI) error -= 2*M_PI;
        this->setPolar(r, theta + (error) / 5);
    }
    this->setPos(this->x() + r * qCos(theta), this->y() - r * qSin(theta));
    if(liveTime.elapsed() > 4000){
        this->scene()->removeItem(this);
        delete this;
    }
}
