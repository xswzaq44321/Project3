#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QRectF>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QtMath>
#include <QPointF>
#include <algorithm>
#include "character.h"

class character;

extern QRectF borderOfBullet;
extern QRectF borderOfCharacter;
extern QList<QGraphicsItem*> enemyList;
extern QList<QGraphicsItem*> myBulletList;
extern QList<QGraphicsItem*> enemyBulletList;
extern QList<QGraphicsItem*> missileList;
extern QTimer *timer;
extern character *boss, *player;

class bullet: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    bullet(const QString &filename = ":/pics/res/error.png", QPointF polar = QPointF(0, 0),
           QPointF picSize = QPointF(15, 15), QGraphicsItem *who = nullptr);
    bullet(const bullet &old);
    ~bullet();
    bullet& operator =(const bullet& r);
    void setDirection(qreal r, qreal theta);
    QGraphicsItem *origin;
public slots:
    virtual void fly();
protected:
    void setPolar(qreal r, qreal theta);
    qreal r;
    qreal theta;
};

class traceBullet: public bullet
{
    Q_OBJECT

public:
    traceBullet();
    traceBullet(const QString &filename, QPointF polar, QPointF picSize, QGraphicsItem *who);
    traceBullet(const bullet &old);
    void setTarget(QGraphicsItem *target);
public slots:
    virtual void fly();
private:
    QGraphicsItem *target;
};

class bounceBullet: public bullet
{
    Q_OBJECT

public:
    bounceBullet();
    bounceBullet(const QString &filename, QPointF polar, QPointF picSize, QGraphicsItem *who);
    bounceBullet(const bullet &old);
public slots:
    virtual void fly();
};

class missile: public bullet
{
    Q_OBJECT

public:
    missile();
    missile(const QString &filename, QPointF polar, QPointF picSize, QGraphicsItem *who);
    missile(const bullet &old);
    void setTarget(QGraphicsItem *target);
public slots:
    virtual void fly();
private:
    QGraphicsItem *target;
    QTime liveTime;
};

#endif // BULLET_H
