#include "ship.h"
#include <QTimer>

Ship::Ship()
{
    // Start position
    int startX = 0;
    int startY = 0;

    setPos(mapToParent(startX, startY));
    setFlags(this->flags() | QGraphicsItem::ItemIsFocusable );
    setRotation(0);

    // Key flags
    keyLeft = false;
    keyRight = false;
    keyUP = false;

    // Set angle
    angle = 10;

    // Set speed
    speed = -5;

    // Set the image of the ship
    shipImage = QPixmap(QString(":/ship/resource/Ship.png"));

    setFocus();
}

QRectF Ship::boundingRect() const
{
    return QRect(-30, -22.5, 60, 45);
}

QPainterPath Ship::shape() const {
    QPainterPath path;
    QPolygonF ship;
    ship << QPointF(0, -22.5);
    ship << QPointF(7, 0);
    ship << QPointF(22, -1);
    ship << QPointF(30, -11);
    ship << QPointF(28, 5);
    ship << QPointF(13, 22.5);
    ship << QPointF(0, 22.5);
    ship << QPointF(-13, 22.5);
    ship << QPointF(-28, 5);
    ship << QPointF(-30, -11);
    ship << QPointF(-22, -1);
    ship << QPointF(-7, 0);
    ship << QPointF(0, -22.5);

    path.addPolygon(ship);

    return path;
}

void Ship::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //QBrush brush = QBrush(Qt::red);
    //QPainterPath shipShape = shape();

    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);
    painter->drawPixmap(boundingRect().x(), boundingRect().y(), boundingRect().width(), boundingRect().height(), shipImage);

    //painter->drawPath(shipShape);
    //painter->fillPath(shipShape, brush);
}

void Ship::advance(int step)
{
    if (!step) return;

    double thisX = x();
    double thisY = y();

    // If the ship leaves the scene, move it back
    if (!scene()->sceneRect().contains(thisX, thisY)) {
        setPos(-thisX, -thisY);
    }

    if (keyLeft && keyUP)
    {
        setRotation(rotation() - angle);
        setPos(mapToParent(0, speed));
    }
    else if (keyRight && keyUP)
    {
        setRotation(rotation() + angle);
        setPos(mapToParent(0, speed));
    }
    else if (keyUP) setPos(mapToParent(0, speed));
    else if (keyLeft) setRotation(rotation() - angle);
    else if (keyRight) setRotation(rotation() + angle);

    if (!scene()->collidingItems(this).isEmpty()) {
        //qDebug() << "Got hit!";
    }
}

void Ship::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) { return; }
    if (event->key() == Qt::Key_Up) keyUP = true;
    if (event->key() == Qt::Key_Left) keyLeft = true;
    if (event->key() == Qt::Key_Right) keyRight = true;
    if (event->key() == Qt::Key_Space) fireMissile();
}

void Ship::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat()) { return; }
    if (event->key() == Qt::Key_Up) keyUP = false;
    if (event->key() == Qt::Key_Left) keyLeft = false;
    if (event->key() == Qt::Key_Right) keyRight = false;
}

void Ship::fireMissile()
{
    Missile* newMissile = new Missile(true, this);
    newMissile->setPos(mapToParent(0, -29));

    newMissile->setRotation(rotation());
    scene()->addItem(newMissile);
}

Ship::~Ship()
{

}
