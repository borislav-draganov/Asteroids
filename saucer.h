#ifndef SAUCER_H
#define SAUCER_H

#include "ship.h"

#include <QGraphicsItem>
#include <QLabel>
#include <QSoundEffect>
#include <QtWidgets>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QtGui>
#include <QTimer>
#include <math.h>

class Saucer : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    Saucer(int size, Ship *target, QObject *parent = 0);                                        // Constructor - size: 1 - small, 2 - large; target - player's ship

    QRectF boundingRect() const;                                                                // Determine the bounding rectangle of the saucer
    QPainterPath shape() const;                                                                 // Determine the shape of the saucer
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);     // Paint the object
    void destoyItem();                                                                          // Destroy the saucer

protected:
    void advance(int step);
    ~Saucer();

private:
     int size;                      // Size of the saucer (0 - small, 1 - big)
     int cornerCoor;                // Top-left coordinate of the saucer - (0,0) will be the center
     int length;                    // Length of the saucer
     qreal speed;                   // Speed of the saucer - negative value
     Ship *target;                  // Pointer to the player ship that's going to be targeted
     QPixmap img;                   // The image of the saucer
     QTimer *fireTimer;             // The timer that fires missiles

signals:
     void saucerKilled(int size);   // Tell the main window a saucer's been killed by the player
     void explosionSound();         // Tell the main window to play the explosion sound effect

private slots:
     void fireMissile();            // Fire a new missile
     void finalDelete();            // Delete the explosion animation after its done + the actual saucer
};

#endif // SAUCER_H
