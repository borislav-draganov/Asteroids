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
    void cntGame();

public slots:
    void playExplosionSound();
    void playLaserSound();

    void updateScore(int);
    void updateLives();
    void updateLevel();

    void updateObjectCounter(int);
    int checkObjectCounter();

private:
    void createActions();
    void createMenus();
    void setObjectCounter(int);
    void newLevel();
    void addShip();
    void displayGameOver();
    void setBtnVisibility(int);

    int score;
    int level;
    int lives;
    int objectCounter;

    QPushButton * btnSave;
    QPushButton * btnContinue;
    QPushButton * btnStart;
    QPushButton * btnLoad;

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
    QLabel *labelLives;
    QLabel *labelLevel;
    QLabel *labelCurScore;
    QLabel *labelCurLives;
    QLabel *labelCurLevel;

    QSoundEffect *explosionEffect;
    QSoundEffect *laserEffect;

    Ship *theShip;
};

#endif // MAINWINDOW_H
