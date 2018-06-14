#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QRectF>
#include <QDebug>
#include <QTimer>
#include <QtMath>
#include <QPointF>
#include <algorithm>

extern QTimer *timer;
extern QRectF borderOfBullet;
extern QRectF borderOfCharacter;
extern QList<QGraphicsItem*> *enemyList;
extern QList<QGraphicsItem*> *myBulletList;
extern QList<QGraphicsItem*> *enemyBulletList;

class bullet: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    bullet(const QString &filename = ":/pics/res/error.png", QPointF polar = QPointF(0, 0),
           QPointF picSize = QPointF(15, 15), bool from_player = false);
    bullet(const bullet &old);
    ~bullet();
    bullet& operator =(const bullet& r);
    void setDirection(qreal r, qreal theta);
    bool fromPlayer;
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
    traceBullet(const QString &filename, QPointF polar, QPointF picSize);
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
    bounceBullet(const QString &filename, QPointF polar, QPointF picSize);
    bounceBullet(const bullet &old);
public slots:
    virtual void fly();
};

class missile: public bullet
{
    Q_OBJECT

public:
    missile();
    missile(const QString &filename, QPointF polar, QPointF picSize);
    missile(const missile &old);
public slots:
    virtual void fly();
private:
    QGraphicsItem *target;
};

#endif // BULLET_H
