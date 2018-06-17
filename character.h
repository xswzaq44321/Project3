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
class gaben;
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
    void move(qreal vx, qreal vy, bool ignore = false);
    void moveTo(qreal x, qreal y, qreal duration, bool ignore = false);
    virtual void setPosition(qreal x, qreal y);
    virtual int attack() = 0;
    virtual bool hit(qreal damage = 1) = 0;
    bool isdead();
    qreal hp, initialHp;
    QRectF border;
    QTimer moveTimer;
    QTime moveClock;
    QTime attackCooldown;
    QTime spellCooldown;
    QList<bullet> bullets;
protected:
    int attackCounter = 0;
};

class gaben_reimu: public character{
public:
    gaben_reimu(int health = 1000);
    virtual ~gaben_reimu();
    virtual int attack();
    virtual bool hit(qreal damage = 1);
    int phase = 1;
private:
    character *minion = nullptr;
};

class gaben: public character{
public:
    gaben(int health = 500);
    virtual ~gaben();
    virtual int attack();
    virtual bool hit(qreal damage = 1);
};

class wallet: public character{
public:
    wallet();
    virtual ~wallet();
    virtual int attack();
    bool bigOneAttack();
    virtual bool hit(qreal damage = 1);
    virtual void setPosition(qreal x, qreal y);
    QGraphicsPixmapItem *heart;
    QList<bullet> missiles;
    int spells = 3;
};

#endif // CHARACTER_H
