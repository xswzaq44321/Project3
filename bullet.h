#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QRectF>
#include <QDebug>
#include <QTimer>

extern QRectF borderOfBullet;
extern QList<QGraphicsItem*> *enemyList;

class bullet: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    bullet(const QString &filename = nullptr, qreal x = -1, qreal y = -1, qreal vx = 0, qreal vy = 0, int size = 15, QGraphicsItem *parent = nullptr);
    bullet(const bullet &old);
    ~bullet();
    bullet& operator =(const bullet& r);
    void setVector(qreal vx, qreal vy);
    QGraphicsItem *origin;
public slots:
    QGraphicsPixmapItem* fly();
    void tracefly();
private:
    qreal vx, vy;
};

#endif // BULLET_H
