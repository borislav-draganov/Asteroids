#ifndef SAUCER_H
#define SAUCER_H

#include "ship.h"
#include "missile.h"
#include "mainwindow.h"

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
    Saucer(int size, QObject *parent = 0);                                                      // Constructor - size: 1 - small, 2 - large; target - player's ship

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
     QPixmap img;                   // The image of the saucer
     QTimer *fireTimer;             // The timer that fires missiles
     QPointer<QLabel> gif_anim ;    // The explosion effect .gif

signals:
     void saucerKilled(int size);   // Tell the main window a saucer's been killed by the player
     void explosionSound();         // Tell the main window to play the explosion sound effect
     void updateObjectCount(int upd); // Tell the main window to update the object count that is used for level change

private slots:
     void fireMissile();            // Fire a new missile
};

#endif // SAUCER_H
