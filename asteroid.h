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
    void destoyItem();                                                                          // Destroy the asteroid

protected:
    void advance(int step);

    ~Asteroid();

private:
     double m_dx;               // Horizontal speed (delta x)
     double m_dy;               // Vertical speed (delta y)
     int size;                  // Size of the asteroid (1 - smallest, 3 - largest)
     double cornerCoor;         // Top-left coordinate of the asteroid - (0,0) will be the center
     double length;             // Length of the asteroid
     double cornerShapeOffset;  // Offset from the corner - turns the shape from a circle into a ellipse
     double lengthShapeOffset;  // Offset of the length - turns the shape from a circle into a ellipse
     QPixmap img;               // The image of the asteroid
     QLabel *gif_anim;
     QMovie *movie;
     QGraphicsProxyWidget *proxy;

signals:
     void smallAsteroidKilled();
     void medAsteroidKilled();
     void bigAsteroidKilled();

public slots:
     void deleteExpl();
};

#endif // ASTEROID_H
