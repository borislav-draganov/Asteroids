#include "mainwindow.h"
#include "asteroid.h"
#include "saucer.h"
#include "ship.h"

#include <QMenuBar>
#include <QApplication>
#include <QtGui>
#include <QPointer>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>


#define maxLevels   10

struct levelDef
{
    int levelNum;
    int asteroidsNum;
    int saucerNum;
};

levelDef levelDescr[maxLevels] = {
    { 1,  3, 2},
    { 2,  4, 2},
    { 3,  5, 2},
    { 4,  6, 3},
    { 5,  7, 3},
    { 6,  8, 3},
    { 7,  9, 3},
    { 8, 10, 4},
    { 9, 11, 4},
    {10, 12, 4}
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    // Set main window fixed size
    this->setFixedSize(700,700);

    // Create widget to use for CentralWidget in the window
    border = new QWidget( this );
    setCentralWidget( border );


    // Create Vertical Box Layout where we will place the scene and the labels
    borderLayout = new QVBoxLayout( border );
    borderLayout->addStretch( 1 );


    // Create Horizontal Boz layout where we will place the labels for score, level, etc
    QWidget *topWin = new QWidget( border );
    topWin->setFixedSize(650, 50);
    horizontalBox = new QHBoxLayout ( topWin );

    // set initial lifes, scores and level
    score = 0;
    lifes = 2;
    level = 1;


    QFont smallFont( "helvetica", 14 );

    labelScore = new QLabel( tr("Score"), topWin );
    labelScore->setFont( smallFont );
    horizontalBox->addWidget( labelScore );

    labelCurScore = new QLabel(topWin);
    labelCurScore -> setText(QString::number(score));
    labelCurScore ->setFont( smallFont );
    horizontalBox->addWidget( labelCurScore );

    labelLevel = new QLabel( tr("Level"), topWin );
    labelLevel->setFont( smallFont );
    horizontalBox->addWidget( labelLevel );

    labelCurLevel = new QLabel( topWin );
    labelCurLevel -> setText(QString::number(level));
    labelCurLevel ->setFont( smallFont );
    horizontalBox->addWidget( labelCurLevel );

    labelLifes = new QLabel( tr("Lifes"), topWin );
    labelLifes->setFont( smallFont );
    horizontalBox->addWidget( labelLifes );

    labelCurLifes = new QLabel( topWin );
    labelCurLifes -> setText(QString::number(lifes));
    labelCurLifes->setFont( smallFont );
    horizontalBox->addWidget( labelCurLifes );
    borderLayout->addWidget( topWin, 0, Qt::AlignHCenter );

    // Create scene
    scene = new QGraphicsScene( border );
    scene->setSceneRect(-300, -300, 600, 600);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    // Set random background for the scene
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int randBackGround = qrand() % 2 + 1;
    QPixmap bckGround (QString(":/background/resource/backGround%1.jpg").arg(randBackGround));
    scene->setBackgroundBrush(bckGround);

    // Create QGraphicsView that will be nested as CentralWidget in the MainWindow

    view = new QGraphicsView(scene);

    borderLayout->addWidget( view, 0, Qt::AlignHCenter );
    borderLayout->addStretch( 1 );

    //Test
    newGame();

    // Create the menus
    createActions();
    createMenus();

    // Set window title
    setWindowTitle(tr("Asteroids"));

    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), scene, SLOT(advance()));
    timer->start(1000 / 33);

    // Prepare the sound file - explosion
    explosionEffect = new QSoundEffect(this);
    explosionEffect->setSource(QUrl::fromLocalFile(":FX/resource/explosion.wav"));
    explosionEffect->setVolume(0.5f);

    // Prepare the sound file - laser
    laserEffect = new QSoundEffect(this);
    laserEffect->setSource(QUrl::fromLocalFile(":FX/resource/laser.wav"));
    laserEffect->setVolume(0.5f);
}

int MainWindow::randInt(int low, int high) {
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

void MainWindow::newLevel()
{
    // Add n asteroids
    int total = 2;
    for(int i = 0; i < total; i++) {
        QPointer<Asteroid> aSmallAsteroid = new Asteroid(1, this);
        QPointer<Asteroid> aMediumAsteroid = new Asteroid(2, this);
        QPointer<Asteroid> aBigAsteroid = new Asteroid(3, this);

        aSmallAsteroid->setPos(randInt(-200,200), randInt(-200,200));
        scene->addItem(aSmallAsteroid);
        connect(aSmallAsteroid, SIGNAL(asteroidKilled(int)), this, SLOT(updateScore(int)));

        aMediumAsteroid->setPos(randInt(-200,200), randInt(-200,200));
        scene->addItem(aMediumAsteroid);
        connect(aMediumAsteroid, SIGNAL(asteroidKilled(int)), this, SLOT(updateScore(int)));

        aBigAsteroid->setPos(randInt(-200,200), randInt(-200,200));
        scene->addItem(aBigAsteroid);
        connect(aBigAsteroid, SIGNAL(asteroidKilled(int)), this, SLOT(updateScore(int)));
    }

    QPointer<Ship> ship = new Ship(this);
    scene->addItem(ship);

    QPointer<Saucer> aSmallSaucer = new Saucer(1, ship, this);
    aSmallSaucer->setPos(200, -200);
    scene->addItem(aSmallSaucer);
    connect(aSmallSaucer, SIGNAL(saucerKilled(int)), this, SLOT(updateScore(int)));

    QPointer<Saucer> aBigSaucer = new Saucer(2, ship, this);
    aBigSaucer->setPos(-200, -200);
    scene->addItem(aBigSaucer);
    connect(aBigSaucer, SIGNAL(saucerKilled(int)), this, SLOT(updateScore(int)));

}

void MainWindow::newGame()
{
    lifes = 2;
    labelCurLifes -> setText(QString::number(lifes));
    level = 1;
    labelCurLevel -> setText(QString::number(level));
    score = 0;
    labelCurScore -> setText(QString::number(score));
    scene->clear();
    newLevel();
}

void MainWindow::loadGame()
{
}

void MainWindow::saveGame()
{
}

void MainWindow::updateScore(int size)
{
    if      (size == 1) { score += 100; }
    else if (size == 2) { score += 50; }
    else if (size == 3) { score += 20; }
    else if (size == 4) { score += 1000; }
    else if (size == 5) { score += 500; }
    else { }

    labelCurScore -> setText(QString::number(score));
}

void MainWindow::updateLifes()
{
    lifes--;
    labelCurLifes -> setText(QString::number(lifes));
}

void MainWindow::updateLevel(int)
{
    level++;
    labelCurLevel -> setText(QString::number(level));
}


void MainWindow::playExplosionSound()
{
    explosionEffect->play();
}

void MainWindow::playLaserSound()
{
    laserEffect->play();
}

void MainWindow::createActions()
{
     newGameAct = new QAction(tr("&New"), this);
     newGameAct->setShortcuts(QKeySequence::New);
     connect(newGameAct, SIGNAL(triggered()), this, SLOT(newGame()));

     loadGameAct = new QAction(tr("&Load"), this);
     loadGameAct->setShortcuts(QKeySequence::Open);
     connect(loadGameAct, SIGNAL(triggered()), this, SLOT(loadGame()));

     saveGameAct = new QAction(tr("&Save"), this);
     saveGameAct->setShortcuts(QKeySequence::Save);
     connect(saveGameAct, SIGNAL(triggered()), this, SLOT(saveGame()));

}

void MainWindow::createMenus()
{
    GameMenu = menuBar()->addMenu(tr("&Menu"));
    GameMenu->addAction(newGameAct);
    GameMenu->addAction(loadGameAct);
    GameMenu->addAction(saveGameAct);
}

MainWindow::~MainWindow()
{
}
