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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    // Set main window fixed size
    this->setFixedSize(650,700);

    // Create widget to use for CentralWidget in the window
    border = new QWidget( this );
    setCentralWidget( border );


    // Create Vertical Box Layout where we will place the scene and the labels
    borderLayout = new QVBoxLayout( border );
    borderLayout->addStretch( 1 );


    // Create Horizontal Boz layout where we will place the labels for score, level, etc
    QWidget *topWin = new QWidget( border );
    topWin->setFixedSize(600, 50);
    horizontalBox = new QHBoxLayout ( topWin );

    QLabel *label;
    QFont smallFont( "helvetica", 14 );
    label = new QLabel( tr("Score"), topWin );
    label->setFont( smallFont );
    horizontalBox->addWidget( label );
    label = new QLabel( tr("0"), topWin );
    label->setFont( smallFont );
    horizontalBox->addWidget( label );
    label = new QLabel( tr("Level"), topWin );
    label->setFont( smallFont );
    horizontalBox->addWidget( label );
    label = new QLabel( tr("0"), topWin );
    label->setFont( smallFont );
    horizontalBox->addWidget( label );
    label = new QLabel( tr("Lifes"), topWin );
    label->setFont( smallFont );
    horizontalBox->addWidget( label );
    label = new QLabel( tr("0"), topWin );
    label->setFont( smallFont );
    horizontalBox->addWidget( label );
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

    // Add n asteroids
    int total = 2;
    for(int i = 0; i < total; i++) {
        QPointer<Asteroid> aSmallAsteroid = new Asteroid(1, this);
        QPointer<Asteroid> aMediumAsteroid = new Asteroid(2, this);
        QPointer<Asteroid> aBigAsteroid = new Asteroid(3, this);

        aSmallAsteroid->setPos(randInt(-200,200), randInt(-200,200));
        scene->addItem(aSmallAsteroid);

        aMediumAsteroid->setPos(randInt(-200,200), randInt(-200,200));
        scene->addItem(aMediumAsteroid);

        aBigAsteroid->setPos(randInt(-200,200), randInt(-200,200));
        scene->addItem(aBigAsteroid);
    }

    QPointer<Ship> ship = new Ship(this);
    scene->addItem(ship);

    QPointer<Saucer> aSmallSaucer = new Saucer(1, ship, this);
    aSmallSaucer->setPos(200, -200);
    scene->addItem(aSmallSaucer);

    QPointer<Saucer> aBigSaucer = new Saucer(2, ship, this);
    aBigSaucer->setPos(-200, -200);
    scene->addItem(aBigSaucer);

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
    explosionEffect->setSource(QUrl::fromLocalFile("explosion.wav"));
    explosionEffect->setVolume(0.5f);

    // Prepare the sound file - laser
    laserEffect = new QSoundEffect(this);
    laserEffect->setSource(QUrl::fromLocalFile("laser.wav"));
    laserEffect->setVolume(0.5f);
}

int MainWindow::randInt(int low, int high) {
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}


void MainWindow::newGame()
{
}

void MainWindow::loadGame()
{
}

void MainWindow::saveGame()
{
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
