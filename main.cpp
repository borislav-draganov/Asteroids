#include "asteroid.h"
#include <QApplication>
#include <QtGui>
#include <QPointer>
#include "math.h"

int randInt(int low, int high) {
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    // Set the scene
    QGraphicsScene scene;
    scene.setSceneRect(-300, -300, 600, 600);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);

    // Add n asteroids
    int total = 3;
    for(int i = 0; i < total; i++) {
        QPointer<Asteroid> aSmallAsteroid = new Asteroid(1);
        QPointer<Asteroid> aMediumAsteroid = new Asteroid(2);
        QPointer<Asteroid> aBigAsteroid = new Asteroid(3);

        aSmallAsteroid->setPos(randInt(-200,200), randInt(-200,200));
        scene.addItem(aSmallAsteroid);

        aMediumAsteroid->setPos(randInt(-200,200), randInt(-200,200));
        scene.addItem(aMediumAsteroid);

        aBigAsteroid->setPos(randInt(-200,200), randInt(-200,200));
        scene.addItem(aBigAsteroid);
    }

    QGraphicsView view(&scene);
    view.setRenderHint(QPainter::Antialiasing);
    view.setCacheMode(QGraphicsView::CacheBackground);
    view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view.setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Asteroids"));

    view.resize(800,800);
    view.show();
    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
    timer.start(1000 / 33);

    return a.exec();
}
