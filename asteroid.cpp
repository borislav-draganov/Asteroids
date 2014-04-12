#include "asteroid.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <math.h>

Asteroid::Asteroid(int size) {
    // Set the size of the asteroid
    this->size = size;

    // Calculate the rectangle coordinates and lengths
    cornerCoor = -10 * size;
    length = 20 * size;

    // Calculate the offset that's going to be used for the actual shape of the asteroid
    cornerShapeOffset = size * 1;
    lengthShapeOffset = size * 2;

    // Set the speed
    m_dx = qrand()%2 + 1;
    m_dy = qrand()%2 + 1;
    setRotation(qrand() % (360 * 16));

    // Pick an image
    int asteroidImg = qrand() % 4 + 1;
    img = QPixmap(QString(":/asteroids/resource/%1.png").arg(asteroidImg));
}

QRectF Asteroid::boundingRect() const {
    return QRectF(cornerCoor, cornerCoor, length, length);
}

QPainterPath Asteroid::shape() const {
    QPainterPath path;

    path.addEllipse(cornerCoor + cornerShapeOffset, cornerCoor, length - lengthShapeOffset, length);
    //path.addEllipse(cornerCoor, cornerCoor, length, length);

    return path;
}

void Asteroid::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    // Print the asteroid image
    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);
    painter->drawPixmap(boundingRect().x(), boundingRect().y(), boundingRect().width(), boundingRect().height(), img);

    // Print the actual shape
    painter->drawPath(shape());
}

void Asteroid::advance(int step){
    if (!step) return;

    int thisX = x();
    int thisY = y();

    // Move the asteroid
    this->setPos(thisX + m_dx, thisY + m_dy);

    // If the asteroid leaves the scene, move it back
    if (!scene()->sceneRect().contains(x(), y())) {
        setPos(-x(), -y());
    }

    //Respond to collision with other items
    const QList<QGraphicsItem *> others = collidingItems();
    if (!others.isEmpty()) {
        const QGraphicsItem * const other = others[0];
        int otherX = other->x();
        int otherY = other->y();

        if (thisX < otherX) { m_dx = -::abs(m_dx); }
        else { m_dx =  ::abs(m_dx); }

        if (thisY < otherY) { m_dy = -::abs(m_dy); }
        else { m_dy =  ::abs(m_dy); }
    }
}

Asteroid::~Asteroid()
{

}
