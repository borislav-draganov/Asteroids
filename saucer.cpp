#include "saucer.h"
#include "missile.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QTimer>
#include <QPointer>
#include <math.h>

static const double Pi = 3.14159265358979323846264338327950288419717;

Saucer::Saucer(int size)
{
    // Set the size of the asteroid
    this->size = size;

    // Calculate the rectangle coordinates and lengths
    cornerCoor = -20 * size;
    length = 40 * size;

    // Set the rotation
    setRotation(qrand() % (360 * 16));

    //QTimer * timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(fireMissile()));

    //timer->start(1000 * size);

    img = QPixmap(QString(":/ship/resource/saucer.png"));
    gif_anim = 0;
    movie = 0;
    proxy = 0;
}

QRectF Saucer::boundingRect() const {
    return QRectF(cornerCoor, cornerCoor, length, length);
}

QPainterPath Saucer::shape() const {
    QPainterPath path;

    path.addEllipse(cornerCoor, cornerCoor, length, length);

    return path;
}

void Saucer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    // Print the saucer image
    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);
    painter->drawPixmap(boundingRect().x(), boundingRect().y(), boundingRect().width(), boundingRect().height(), img);

    // Print the actual shape
    //painter->setPen(QPen(Qt::red));
    //painter->drawPath(shape());
}

void Saucer::destoyItem()
{
    // Use the current coordinates
    double thisX = x();
    double thisY = y();

    // Add an explosion effect
    movie = new QMovie(":/FX/resource/explosion.gif");
    movie->setScaledSize(QSize(length, length));
    movie->setSpeed(200);
    movie->start();

    gif_anim = new QLabel();
    gif_anim->setMovie(movie);
    gif_anim->setAttribute( Qt::WA_TranslucentBackground, true );

    proxy = scene()->addWidget(gif_anim);
    proxy->setPos(thisX - gif_anim->width()/2, thisY - gif_anim->height()/2);
    connect(movie, SIGNAL(finished()), this, SLOT(deleteExpl()));

    // Remove the parent asteroid
    scene()->removeItem(this);
}

void Saucer::advance(int step){
    if (!step) return;

    // Move the saucer
    this->setPos(mapToParent(0,-1));
    int thisX = x();
    int thisY = y();

    // If the saucer leaves the scene, move it back
    if (!scene()->sceneRect().contains(thisX, thisY)) {
        setPos(-thisX, -thisY);
    }

    //Respond to collision with other items
    if(!scene()->collidingItems(this).isEmpty()) {
        foreach (QGraphicsItem *item, scene()->collidingItems(this)) {
            // Check if it was a missile
            Missile *aMissile = qgraphicsitem_cast<Missile *>(item);
            if (aMissile != NULL)
            {
                // If it was a missile from the ship, give points to the player
                if (aMissile->firedFromShip()) {
                    emit saucerKilled();
                }

                // In that case - destroy the missile
                aMissile->destoyItem();
            }
        }

        // The saucer hit something so it'll be destroyed
        destoyItem();
    }
}

void Saucer::fireMissile()
{
    Missile* newMissile = new Missile(false, this);
    newMissile->setPos(mapToParent(0,-length/2-20));
/*
    double deltaY = -y();
    double deltaX = -x();

    double angleInDegrees = ::atan2(deltaY, deltaX) * 180 / Pi;

    newMissile->setRotation(angleInDegrees);*/
    newMissile->setRotation(rotation());
    scene()->addItem(newMissile);
}

Saucer::~Saucer()
{
    //if (gif_anim) { delete gif_anim; }
    if (movie) { delete movie; }
    if (proxy) { delete proxy; }
}

void Saucer::deleteExpl()
{
    delete movie;
    movie = 0;

    delete proxy;
    proxy = 0;

    delete this;
}
