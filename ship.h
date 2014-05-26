#ifndef SHIP_H
#define SHIP_H

#include "missile.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QSoundEffect>
#include <QStyleOption>
#include <QKeyEvent>
#include <QtWidgets>
#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QtCore>
#include <QPoint>
#include <QTimer>
#include <QtGui>

class Ship : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    double width;                   // Width of the ship
    double height;                  // Height of the ship
    Ship(QObject *parent = 0);      // Constructor
    QRectF boundingRect() const;    // Create the bounding rectangle of the ship
    QPainterPath shape() const;     // Create the shape of the ship
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);     // Paint the ship
    void destroyShip();             // Destroy the ship

protected:
    void advance(int step);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void fireMissile();
    ~Ship();

private:
    int flashes;                    // How many times the ship will flash
    qreal angle;                    // The angle used for rotation
    qreal speed;                    // The movement speed
    bool keyLeft;                   // Flag for the Left key
    bool keyRight;                  // Flag for the Right key
    bool keyUP;                     // Flag for the thrust key
    QPixmap shipImage;              // The image of the ship
    QPointer<QLabel> gif_anim;      // The explosion effect .gif
    QTimer *flash;                  // Timer for flashing the ship when it is created

signals:
    void shipDestroyed();           // Tell the main window that a ship has been destroyed
    void explosionSound();          // Tell the main window to play the explosion sound effect

private slots:
    void flashEvent();              // Make the newly created ship flashing
};

#endif // SHIP_H
