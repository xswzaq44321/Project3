#include "bullet.h"

bullet::bullet(const QString &filename, QPointF polar, QPointF picSize, QGraphicsItem *who):
    QGraphicsPixmapItem(QPixmap(filename).scaled(picSize.x(), picSize.y(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)),
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
    if(origin == static_cast<QGraphicsItem*>(player)){
        QList<QGraphicsItem*>::iterator index = std::find(myBulletList.begin(), myBulletList.end(), this);
        if(index != myBulletList.end()){
            *index = nullptr;
        }else{
            index = std::find(missileList.begin(), missileList.end(), this);
            if(index != missileList.end()){
                *index = nullptr;
            }
        }
    }else{
        auto index = std::find(enemyBulletList.begin(), enemyBulletList.end(), this);
        if(index != enemyBulletList.end())
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
    for(; theta > M_PI; theta -= 2*M_PI);
    for(; theta < -M_PI; theta += 2*M_PI);
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
        delete this;
    }
}

traceBullet::traceBullet():
    bullet()
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
    if(!enemyList.contains(target)){
        target = nullptr;
    }
    if(target != nullptr){
        double dx =(target->x() + target->boundingRect().width() / 2) - (this->x() + this->boundingRect().width() / 2);
        double dy =(this->y() + this->boundingRect().height() / 2) - (target->y() + target->boundingRect().height() / 2);
        double direction = qAtan2(dy, dx);
        double error = direction - theta;
        if(error > M_PI) error -= 2*M_PI;
        this->setPolar(r, theta + error);
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
    if(fabs(theta) >= qDegreesToRadians((double)10) && fabs(theta) <= qDegreesToRadians((double)170)){
        if(this->x() < 0 || this->x() > (borderOfCharacter.width() - this->boundingRect().width())){
            theta = theta - 2*(M_PI_2 - fabs(theta));
        }
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
        }else if(enemyList.size() > 0){
            target = enemyList.front();
        }
    }
    if(target != nullptr){
        double dx =(target->x() + target->boundingRect().width() / 2) - (this->x() + this->boundingRect().width() / 2);
        double dy =(this->y() + this->boundingRect().height() / 2) - (target->y() + target->boundingRect().height() / 2);
        double direction = qAtan2(dy, dx);
        double error = direction - theta;
        if(error > M_PI) error -= 2*M_PI;
        this->setPolar(r, theta + error / (8*10.0/timer->interval()));
    }
    this->setPos(this->x() + r * qCos(theta), this->y() - r * qSin(theta));
    if(liveTime.elapsed() > 4000){
        this->scene()->removeItem(this);
        delete this;
    }
}

aimBullet::aimBullet():
    bullet()
{
    liveTime.start();
    theta = 0;
    this->setTransformOriginPoint(this->boundingRect().width()/2, this->boundingRect().height()/2);
}

aimBullet::aimBullet(const QString &filename, QPointF polar, QPointF picSize, QGraphicsItem *who):
    bullet(filename, polar, picSize, who)
{
    liveTime.start();
    theta = 0;
    this->setTransformOriginPoint(this->boundingRect().width()/2, this->boundingRect().height()/2);
}

aimBullet::aimBullet(const bullet &old):
    bullet(old)
{
    liveTime.start();
    this->setTransformOriginPoint(this->boundingRect().width()/2, this->boundingRect().height()/2);
}

void aimBullet::setTarget(QGraphicsItem *target){
    this->target = target;
}

void aimBullet::fly(){
    if(liveTime.elapsed() < 1000){
        if(target != nullptr){
            double dx =(target->x() + target->boundingRect().width() / 2) - (this->x() + this->boundingRect().width() / 2);
            double dy =(this->y() + this->boundingRect().height() / 2) - (target->y() + target->boundingRect().height() / 2);
            double direction = qAtan2(dy, dx);
            double error = direction - theta;
            if(error > M_PI) error -= 2*M_PI;
            this->setPolar(r, theta + error);
            this->setRotation(qRadiansToDegrees(-(theta + error)));
        }
    }else{
        this->setPos(this->x() + r * qCos(theta), this->y() - r * qSin(theta));
    }
}
