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
#include "character.h"

class bullet;
class character;
extern QRectF borderOfCharacter;
extern QList<QGraphicsItem*> enemyList;
extern QList<QGraphicsItem*> enemyBulletList;
extern QList<QGraphicsItem*> myBulletList;
extern QList<QGraphicsItem*> missileList;
extern character *player, *boss;

class character: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    character();
    character(const QString &filename, int hp_init = 1000);
    virtual ~character();
    virtual void move(qreal vx, qreal vy);
    virtual void moveTo(qreal x, qreal y, qreal duration);
    virtual void setPosition(qreal x, qreal y);
    virtual void attack() = 0;
    virtual bool hit() = 0;
    bool isdead();
    int hp, initialHp;
    QRectF border;
    QTimer moveTimer;
    QTime moveClock;
    QTime attackCooldown;
    QTime spellCooldown;
    QList<bullet> bullets;
};

class gaben_reimu: public character{
public:
    gaben_reimu(int health = 1000);
    virtual ~gaben_reimu();
    virtual void attack();
    virtual bool hit();
private:
    int attackCounter = 0;
    int phase = 1;
};

class wallet: public character{
public:
    wallet();
    virtual ~wallet();
    virtual void attack();
    bool bigOneAttack();
    virtual bool hit();
    virtual void setPosition(qreal x, qreal y);
    QGraphicsPixmapItem *heart;
    QList<bullet> missiles;
    int spells = 3;
private:
    int attackCounter = 0;
};

#endif // CHARACTER_H
