#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QObject>
#include <QRectF>
#include <QDebug>

class bullet: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    bullet(const QString &filename, int x, int y, int vx, int vy);
    ~bullet();
    QRectF *borderOfScene;
public slots:
    void fly();
    void tracefly();
private:
    int vx, vy;
};

#endif // BULLET_H
