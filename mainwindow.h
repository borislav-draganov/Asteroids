#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ship.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QmdiArea>
#include <QLayout>
#include <QPoint>

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static int randInt(int , int );

private slots:
    void newGame();
    void loadGame();
    void saveGame();
    void newLevel();

public slots:
    void playExplosionSound();
    void playLaserSound();

    void updateScore(int);
    void updateLifes();
    void updateLevel(int);

private:
    void createActions();
    void createMenus();

    int score;
    int level;
    int lifes;

    QMenu *GameMenu;
    QAction *newGameAct;
    QAction *loadGameAct;
    QAction *saveGameAct;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QWidget *border;
    QVBoxLayout *borderLayout;
    QHBoxLayout *horizontalBox;

    QLabel *labelScore;
    QLabel *labelLifes;
    QLabel *labelLevel;
    QLabel *labelCurScore;
    QLabel *labelCurLifes;
    QLabel *labelCurLevel;

    QSoundEffect *explosionEffect;
    QSoundEffect *laserEffect;
};

#endif // MAINWINDOW_H
