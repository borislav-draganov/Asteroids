#ifndef ASTEROID_H
#define ASTEROID_H

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QSoundEffect>
#include <QtWidgets>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QtGui>
#include <math.h>

class Asteroid : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    Asteroid(int size, QObject *parent = 0);

    QRectF boundingRect() const;                                                                // Determine the bounding rectangle of the asteroid
    QPainterPath shape() const;                                                                 // Determine the shape of the asteroid
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);     // Paint the object
    void destoyItem();                                                                          // Destroy the asteroid and split it to smaller one if possible

protected:
    void advance(int step);
    ~Asteroid();

private:
     int size;                      // Size of the asteroid (1 - smallest, 3 - largest)
     double m_dx;                   // Horizontal speed (delta x)
     double m_dy;                   // Vertical speed (delta y)
     double cornerCoor;             // Top-left coordinate of the asteroid - (0,0) will be the center
     double length;                 // Length of the asteroid
     double cornerShapeOffset;      // Offset from the corner - turns the shape from a circle into a ellipse
     double lengthShapeOffset;      // Offset of the length - turns the shape from a circle into a ellipse
     qreal randRotation;            // A random rotation while moving
     QPixmap img;                   // The image of the asteroid

signals:
     void smallAsteroidKilled();    // Tell the main window a small asteroid's been killed by the player
     void medAsteroidKilled();      // Tell the main window a medium asteroid's been killed by the player
     void bigAsteroidKilled();      // Tell the main window a big asteroid's been killed by the player
     void explosionSound();         // Tell the main window to play the explosion sound effect

private slots:
     void finalDelete();            // Delete the explosion animation after its done + the actual asteroid
};

#endif // ASTEROID_H
