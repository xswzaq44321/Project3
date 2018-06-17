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
extern QList<QGraphicsItem*> enemyList;
extern QList<QGraphicsItem*> myBulletList;
extern QList<QGraphicsItem*> missileList;
extern QList<QGraphicsItem*> enemyBulletList;
extern QTimer *timer;
extern character *boss, *player;

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
    void gameWin();
    void gameOver();

private slots:
    void moveHandler();
    void attackHandler();
    void collidingDetect();
    void infoBoardHandler();
    void playerExplode(QPointF center);
    void respawn();
    void gameJudger();

private:
    Ui::MainWindow *ui;
    QMetaObject::Connection explodeConnection;
    QGraphicsScene *scene;
    QGraphicsRectItem *bossHealth;
    QGraphicsTextItem *scoreText, *costText;
    QGraphicsPixmapItem *life, *infoItem;
    QGraphicsPixmapItem *backgroundItem[2];
    QGraphicsPixmapItem *shockWave = nullptr;
    QGraphicsPixmapItem *overItem, *winItem;
    QPixmap *lifeCanvas;
    QPainter *lifePainter;
    QTime *respawnTime;
    QTime *shockWaveTime;
    QPointF sizeOfBackground;
    bool moving[4] = {0};
    int speed = 2.5;
    qreal score = 0, spend = 0, card = 0;
    bool attack = false, bigOne = false;
    bool playerIsDead = false;
    set<int> moveKeys;
    set<int> functionKeys;
};

#endif // MAINWINDOW_H
