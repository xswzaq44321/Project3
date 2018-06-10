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

extern QRectF borderOfBullet;
extern QList<QGraphicsItem*> *enemyList;

class bullet: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    bullet(const QString &filename = nullptr, qreal x = -1, qreal y = -1, qreal r = 0, qreal theta = 0, int size = 15, QGraphicsItem *parent = nullptr);
    bullet(const bullet &old);
    ~bullet();
    bullet& operator =(const bullet& r);
    void setPolar(qreal r, qreal theta);
    void setTarget(QGraphicsItem *target);
    QGraphicsItem *origin, *target;
public slots:
    void fly();
private:
    qreal r;
    qreal theta;
};

#endif // BULLET_H
