#ifndef SAUCER_H
#define SAUCER_H

#include <ship.h>
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

class Saucer : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    Saucer(int size);

    QRectF boundingRect() const;                                                                // Determine the bounding rectangle of the saucer
    QPainterPath shape() const;                                                                 // Determine the shape of the saucer
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);     // Paint the object
    void destoyItem();                                                                          // Destroy the saucer

protected:
    void advance(int step);
    ~Saucer();

private:
     int size;                  // Size of the saucer (0 - small, 1 - big)
     int cornerCoor;            // Top-left coordinate of the saucer - (0,0) will be the center
     int length;                // Length of the saucer
     QPixmap img;               // The image of the saucer
     QLabel *gif_anim;
     QMovie *movie;
     QGraphicsProxyWidget *proxy;

signals:
     void saucerKilled();

public slots:
    void fireMissile();
    void deleteExpl();
};

#endif // SAUCER_H
