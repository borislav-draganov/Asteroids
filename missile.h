#ifndef MISSILE_H
#define MISSILE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QSoundEffect>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <math.h>

class Missile : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    Missile(bool shipMissile, QObject *parent = 0);

    QRectF boundingRect() const;                                                                // Determine the bounding rectangle of the missile
    QPainterPath shape() const;                                                                 // Determine the shape of the missile
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);     // Paint the object
    void destoyItem();                                                                          // Destroy the missile
    bool firedFromShip();                                                                       // Return true if fired from ship
    int getLength();                                                                            // Get the length of the missile

    // Enable qgraphicsitem_cast
    enum { Type = UserType + 1 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

protected:
    void advance(int step);
    ~Missile();

private:
     int speed;                    // Speed of the missile - negative value
     double cornerCoor;            // Top-left coordinate of the missile - (0,0) will be the center
     double length;                // Length of the missile
     bool shipMissile;             // Is this missile fired from the ship
     QSoundEffect *soundEffect;    // The laser sound effect

signals:
     void laserSound();            // Tell the main window to play the laser sound effect
};

#endif // MISSILE_H
