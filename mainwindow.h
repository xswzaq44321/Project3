#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QKeyEvent>
#include <set>
#include <QRectF>
#include <QList>
#include "character.h"
#include "bullet.h"
using std::set;

namespace Ui {
class MainWindow;
}

extern QRectF borderOfBullet;
extern QRectF borderOfCharacter;
extern QList<QGraphicsItem*> *enemyList;
extern QList<QGraphicsItem*> *myBullitList;
extern QList<QGraphicsItem*> *enemyBullitList;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool eventFilter(QObject *watched, QEvent *event);
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
    void playerDied();

private slots:
    void moveHandler();
    bool collidingDetect();
    void respawn();
    void attackHandler();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *item;
    QTimer *timer;
    QTime *respawnTime;
    character *boss, *player;
    bool moving[4] = {0};
    int speed = 2;
    bool attack = false;
    set<int> moveKeys;
    set<int> functionKeys;
};

#endif // MAINWINDOW_H
