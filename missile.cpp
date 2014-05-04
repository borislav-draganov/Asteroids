#include "missile.h"

/* Constructor
 * @param shipMissile : true if fired form the player ship
 * @param parent : the parent in the Qt hierarchy
 */
Missile::Missile(bool shipMissile, QObject *parent) : QObject(parent) {
    this->shipMissile = shipMissile;

    // Set the size parameters
    cornerCoor = -5;
    length = 10;

    // Set the movement speed
    speed = -10;

    // Play the laser sound effect
    connect(this, SIGNAL(laserSound()), parent, SLOT(playLaserSound()));
    emit laserSound();
}

// Determine the maximum area available for displaying content
QRectF Missile::boundingRect() const {
    return QRectF(cornerCoor, cornerCoor, length, length);
}

// Determine the actual shape of the asteroid - this is used in collision detection
QPainterPath Missile::shape() const {
    QPainterPath path;

    path.addEllipse(cornerCoor - cornerCoor/2, cornerCoor, length - length/2, length);

    return path;
}

// Determine the look of the missile
void Missile::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QBrush brush = QBrush(Qt::red);
    QPainterPath theShape = shape();

    painter->drawPath(theShape);
    painter->fillPath(theShape, brush);
}

// Move the missile
void Missile::advance(int step){
    if (!step) return;

    // Move the missile
    setPos(mapToParent(0, speed));

    // If the missile leaves the scene, delete it
    if (!scene()->sceneRect().contains(x(), y())) {
        deleteLater();
    }
}

// Delete the missile
void Missile::destoyItem()
{
    deleteLater();
}

/* Return true if the missile was fired from the player ship
 * @return : true - fired from ship; false - fired from saucer
 */
bool Missile::firedFromShip()
{
    return shipMissile;
}

/* Get the length of the missile
 * @return : height of the missile
 */
int Missile::getLength()
{
    return length;
}

Missile::~Missile()
{

}
