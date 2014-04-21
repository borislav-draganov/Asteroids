#include "missile.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <math.h>

Missile::Missile(bool shipMissile, QObject *parent) : QObject(parent) {
    this->shipMissile = shipMissile;
    cornerCoor = -5;
    length = 10;
}

QRectF Missile::boundingRect() const {
    return QRectF(cornerCoor, cornerCoor, length, length);
}

QPainterPath Missile::shape() const {
    QPainterPath path;

    path.addEllipse(cornerCoor - cornerCoor/2, cornerCoor, length - length/2, length);

    return path;
}

void Missile::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QBrush brush = QBrush(Qt::red);
    QPainterPath theShape = shape();

    painter->drawPath(theShape);
    painter->fillPath(theShape, brush);
}

void Missile::destoyItem()
{
    delete this;
}

bool Missile::firedFromShip()
{
    return shipMissile;
}

void Missile::advance(int step){
    if (!step) return;

    // Move the missile
    setPos(mapToParent(0, -10));

    // If the missile leaves the scene, delete it
    if (!scene()->sceneRect().contains(x(), y())) {
        delete this;
    }
}

Missile::~Missile()
{

}
