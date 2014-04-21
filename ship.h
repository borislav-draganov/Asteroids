#ifndef SHIP_H
#define SHIP_H

#include "missile.h"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QtGui>
#include <QtCore>
#include <QPoint>

class Ship : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    Ship();
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPoint coordinates();

protected:
    void advance(int step);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void fireMissile();
    ~Ship();

private:
    qreal angle;
    qreal speed;
    bool keyLeft;
    bool keyRight;
    bool keyUP;
    QPixmap shipImage;
};

#endif // SHIP_H
