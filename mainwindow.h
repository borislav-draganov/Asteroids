#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ship.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QmdiArea>
#include <QLayout>
#include <QPoint>
#include <topscorewin.h>

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

    bool checkForTopScore();
    void updateTopScores();

    QString askUserName();

    int score; // Score counter
    int level; // Levels counter
    int lives; // Lives counter
    int objectCounter; // Object counter. Object = any asteroid or saucer on the field. Used for checks if we have to start new level
    QPushButton * test;
    QPushButton * btnSave;
    QPushButton * btnContinue;
    QPushButton * btnStart;
    QPushButton * btnLoad;
    QPushButton * btnTopScore;


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
    QPushButton * btnSaveBack;

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
    QPushButton * btnLoadBack;

    QWidget * topWin;
    topScoreWin * topScore;
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

    QString userName;
};

#endif // MAINWINDOW_H
