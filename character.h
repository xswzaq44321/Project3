#ifndef CHARACTER_H
#define CHARACTER_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QtGlobal>
#include <QList>
#include <QObject>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include "bullet.h"

extern QRectF borderOfCharacter;
extern QList<QGraphicsItem*> *enemyList;
extern QList<QGraphicsItem*> *enemyBulletList;
extern QList<QGraphicsItem*> *myBulletList;
class character: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    character();
    character(const QString &filename, int hp_init = 1000);
    virtual ~character();
    virtual void move(qreal vx, qreal vy);
    virtual void setPosition(qreal x, qreal y);
    virtual void attack(QTimer *timer) = 0;
    virtual bool hit() = 0;
    bool isdead();
    int hp, initialHp;
    QRectF border;
    QTime attackCooldown;
    QList<bullet> normalBullets, traceBullets;
};

class gaben_reimu: public character{
public:
    gaben_reimu(int health = 1000);
    virtual ~gaben_reimu();
    virtual void attack(QTimer *timer);
    virtual bool hit();
private:
    int attackCounter = 0;
};

class wallet: public character{
public:
    wallet();
    virtual ~wallet();
    virtual void attack(QTimer *timer);
    virtual bool hit();
    virtual void setPosition(qreal x, qreal y);
    QGraphicsPixmapItem *heart;
private:
    int attackCounter = 0;
};

#endif // CHARACTER_H
