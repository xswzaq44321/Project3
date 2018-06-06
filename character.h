#ifndef CHARACTER_H
#define CHARACTER_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QtGlobal>
#include <QList>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QVector>
#include "bullet.h"

class character: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    character();
    character(const QString &filename, int hp_init = 1000);
    virtual ~character();
    void move(int vx, int vy);
    virtual void attack(QTimer *timer, character *enemy) = 0;
    virtual void hit() = 0;
    bool isdead();
    int hp;
    int borderX, borderY;
    QVector<bullet> normalBullets;
};

class gaben_reimu: public character{
public:
    gaben_reimu();
    virtual ~gaben_reimu();
    virtual void attack(QTimer *timer, character *enemy);
    virtual void hit();
};

class wallet: public character{
public:
    wallet();
    virtual ~wallet();
    virtual void attack(QTimer *timer, character *enemy);
    virtual void hit();
};

#endif // CHARACTER_H
