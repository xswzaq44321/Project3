#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QObject>
#include <QRectF>
#include <QDebug>
#include <QTimer>

extern QRectF borderOfBullet;
class bullet: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    bullet(const QString &filename = nullptr, qreal x = -1, qreal y = -1, qreal vx = 0, qreal vy = 0, int size = 15);
    bullet(const bullet &old);
    ~bullet();
    bullet& operator =(const bullet& r);
    void setVector(qreal vx, qreal vy);
public slots:
    bool fly(const QGraphicsPixmapItem *enemy);
    void tracefly();
private:
    qreal vx, vy;
};

#endif // BULLET_H
