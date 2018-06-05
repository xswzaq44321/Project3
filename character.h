#ifndef CHARACTER_H
#define CHARACTER_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QtGlobal>

class character: public QGraphicsPixmapItem
{
public:
    character();
    character(const QString &filename, int hp_init = 1000);
    ~character();
    void move(int vx, int vy);
    virtual void attack() = 0;
    virtual void hit() = 0;
    virtual void died() = 0;
    int hp;
    int borderX, borderY;
};

class gaben_reimu: public character{
public:
    gaben_reimu();
    virtual ~gaben_reimu();
    virtual void attack();
    virtual void hit();
    virtual void died();
};

class wallet: public character{
public:
    wallet();
    virtual ~wallet();
    virtual void attack();
    virtual void hit();
    virtual void died();
};

#endif // CHARACTER_H
