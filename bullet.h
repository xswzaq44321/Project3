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

extern QRectF borderOfBullet;
extern QRectF borderOfCharacter;
extern QList<QGraphicsItem*> *enemyList;
extern QList<QGraphicsItem*> *myBulletList;
extern QList<QGraphicsItem*> *enemyBulletList;

class bullet: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    bullet(const QString &filename = nullptr, QPointF polar = QPointF(0, 0),
           QPointF picSize = QPointF(15, 15), bool from_player = false);
    bullet(const bullet &old);
    ~bullet();
    bullet& operator =(const bullet& r);
    void setPolar(qreal r, qreal theta);
    void setTarget(QGraphicsItem *target);
    QGraphicsItem *target;
    bool fromPlayer;
public slots:
    virtual void fly();
protected:
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
public slots:
    virtual void fly();
};

class bounceSale: public bullet
{
    Q_OBJECT

public:
    bounceSale();
    bounceSale(const QString &filename, QPointF polar, QPointF picSize);
    bounceSale(const bullet &old);
public slots:
    virtual void fly();
};

#endif // BULLET_H
