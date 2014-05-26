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
    QPointF getShipCoordinates();

private slots:
    void newGame();
    void cntGame();
    void showLoadGames();
    void showSavedGames();
    void pshdButton();

public slots:
    void playExplosionSound();
    void playLaserSound();

    void updateScore(int);
    void updateLives();
    void updateLevel();

    void updateObjectCounter(int);
    int checkObjectCounter();

private:

    /* Initially used to add top menu to the window
    !!!No longer needed!!!
    void createActions();
    void createMenus();
    */

    void setObjectCounter(int);
    void newLevel();
    void addShip();
    void displayGameOver();
    void setBtnVisibility(int);
    void addButtons();
    void loadGame(int);
    void saveGame(int);
    void saveCurGame(int);
    void parseSavesXML();

    int score;
    int level;
    int lives;
    int objectCounter;
    QPushButton * test;
    QPushButton * btnSave;
    QPushButton * btnContinue;
    QPushButton * btnStart;
    QPushButton * btnLoad;


    QPushButton * btnSave1;
    QPushButton * btnSave2;
    QPushButton * btnSave3;
    QPushButton * btnSave4;
    QPushButton * btnSave5;
    QPushButton * btnSave6;
    QPushButton * btnSave7;
    QPushButton * btnSave8;
    QPushButton * btnSave9;
    QPushButton * btnSave10;

    QPushButton * btnLoad1;
    QPushButton * btnLoad2;
    QPushButton * btnLoad3;
    QPushButton * btnLoad4;
    QPushButton * btnLoad5;
    QPushButton * btnLoad6;
    QPushButton * btnLoad7;
    QPushButton * btnLoad8;
    QPushButton * btnLoad9;
    QPushButton * btnLoad10;

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

    QLabel *labelEmptyPos1;
    QLabel *labelEmptyPos2;

    QSoundEffect *explosionEffect;
    QSoundEffect *laserEffect;

    QPointer<Ship> theShip;
};

#endif // MAINWINDOW_H
