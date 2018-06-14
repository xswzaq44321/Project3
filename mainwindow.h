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
#include <QFont>
#include "character.h"
#include "bullet.h"
using std::set;

namespace Ui {
class MainWindow;
}

extern QFont mainFont;
extern QRectF borderOfBullet;
extern QRectF borderOfCharacter;
extern QList<QGraphicsItem*> *enemyList;
extern QList<QGraphicsItem*> *myBulletList;
extern QList<QGraphicsItem*> *enemyBulletList;
extern QTimer *timer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool eventFilter(QObject *watched, QEvent *event);
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
    void gameStart();

private slots:
    void moveHandler();
    bool collidingDetect();
    void respawn();
    void attackHandler();
    void infoBoardHandler();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsRectItem *bossHealth;
    QGraphicsTextItem *scoreText;
    QGraphicsPixmapItem *life;
    QPixmap *lifeCanvas;
    QPainter *lifePainter;
    QTime *respawnTime;
    character *boss = nullptr, *player = nullptr;
    bool moving[4] = {0};
    int speed = 2;
    int score = 0;
    bool attack = false;
    bool playerIsDead = false;
    set<int> moveKeys;
    set<int> functionKeys;
};

#endif // MAINWINDOW_H
