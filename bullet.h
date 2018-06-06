#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QObject>
#include <QRectF>
#include <QDebug>
#include <QTimer>

extern QRectF borderOfScene;
class bullet: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    bullet(const QString &filename = nullptr, int x = -1, int y = -1, int vx = 0, int vy = 0, int size = 15);
    bullet(const bullet &old);
    ~bullet();
    bullet& operator =(const bullet& r);
public slots:
    bool fly(const QGraphicsPixmapItem *enemy);
    void tracefly();
private:
    int vx, vy;
};

#endif // BULLET_H
