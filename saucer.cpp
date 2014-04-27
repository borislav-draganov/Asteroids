#include "saucer.h"
#include "missile.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

 static qreal normalizeAngle(qreal angle)
 {
     while (angle < 0)
         angle += TwoPi;
     while (angle > TwoPi)
         angle -= TwoPi;
     return angle;
 }

/* Constructor
 * @param size : 1 - small, 2 - large
 * @param parent : the parent in the Qt hierarchy
 */
Saucer::Saucer(int size, Ship *target, QObject *parent) : QObject(parent)
{
    // Set the size of the saucer
    this->size = size;
    this->target = target;

    // Set the movement speed
    speed = - ( 2 / size );

    // Calculate the rectangle coordinates and lengths
    cornerCoor = -20 * size;
    length = 40 * size;

    // Set the rotation
    setRotation(qrand() % (360 * 16));

    // Create a timer for firing missiles
    fireTimer = new QTimer(this);
    fireTimer->start(1000 * size);
    connect(fireTimer, SIGNAL(timeout()), this, SLOT(fireMissile()));

    // Set the saucer's image
    img = QPixmap(QString(":/ship/resource/saucer.png"));

    // Connect to the slot that plays the sound
    connect(this, SIGNAL(explosionSound()), parent, SLOT(playExplosionSound()));

    gif_anim = 0;
}

// Determine the maximum area available for displaying content
QRectF Saucer::boundingRect() const {
    return QRectF(cornerCoor, cornerCoor, length, length);
}

// Determine the actual shape of the saucer - this is used in collision detection
QPainterPath Saucer::shape() const {
    QPainterPath path;

    path.addEllipse(cornerCoor, cornerCoor, length, length);

    return path;
}

// Determine the look of the saucer
void Saucer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    // Print the saucer image
    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);
    painter->drawPixmap(boundingRect().x(), boundingRect().y(), boundingRect().width(), boundingRect().height(), img);
}

void Saucer::advance(int step){
    if (!step) return;
/*
    // Try not to crash with any other objects
    qreal angle = 0;
    QList<QGraphicsItem *> dangerItems = scene()->items(QPolygonF()
                                                            << mapToScene(0, 0)
                                                            << mapToScene(-30 * size, -50 * size)
                                                            << mapToScene(30 * size, -50 * size));
    foreach (QGraphicsItem *item, dangerItems) {
        if (item == this) continue;

        QLineF lineToItem(QPointF(0, 0), mapFromItem(item, 0, 0));
        qreal angleToItem = ::acos(lineToItem.dx() / lineToItem.length());

        if (lineToItem.dy() < 0) { angleToItem = TwoPi - angleToItem; }

        angleToItem = normalizeAngle((Pi - angleToItem) + Pi / 2);

        if (angleToItem >= 0 && angleToItem < Pi / 2) {
            // Rotate right
            angle += 10;
        } else if (angleToItem <= TwoPi && angleToItem > (TwoPi - Pi / 2)) {
            // Rotate left
            angle -= 10;
        }
    }

    setRotation(rotation() + angle);
*/
    // Move the saucer
    this->setPos(mapToParent(0, speed));
    int thisX = x();
    int thisY = y();

    // If the saucer leaves the scene, move it back
    if (!scene()->sceneRect().contains(thisX, thisY)) {
        setPos(-thisX, -thisY);
    }

    // Respond to collision with other items
    QList<QGraphicsItem *> collidingItems = scene()->collidingItems(this);
    if (!collidingItems.isEmpty()) {
        QGraphicsItem * item = collidingItems[0];
        // The saucer hit something so it'll be destroyed
        destoyItem();

        // Check if it was a missile
        Missile *aMissile = qgraphicsitem_cast<Missile *>(item);
        if (aMissile != NULL) {
            // If it was a missile from the ship, give points to the player
            if (aMissile->firedFromShip()) {
                int maxAsteroidSize = 3;
                emit saucerKilled(size + maxAsteroidSize); // +3 because we'll use the same slot as the asteroids do
            }

            // In that case - destroy the missile
            aMissile->destoyItem();
        }
    }
}

// Destroy the saucer
void Saucer::destoyItem()
{
    // Stop firing
    fireTimer->stop();

    // Use the current coordinates
    double thisX = x();
    double thisY = y();

    // Add an explosion effect
    emit explosionSound();

    QMovie *movie = new QMovie(this);
    movie->setFileName(":/FX/resource/explosion.gif");
    movie->setScaledSize(QSize(length, length));
    movie->setSpeed(250);
    movie->start();

    gif_anim = new QLabel();
    gif_anim->setMovie(movie);
    gif_anim->setAttribute( Qt::WA_TranslucentBackground, true );

    QGraphicsProxyWidget *proxy = scene()->addWidget(gif_anim);
    proxy->setParent(this);
    proxy->setPos(thisX - gif_anim->width()/2, thisY - gif_anim->height()/2);

    // Schedule the object for deleting after the explosion gif finishes
    connect(movie, SIGNAL(finished()), this, SLOT(deleteLater()));

    // Remove the saucer from the field
    scene()->removeItem(this);
}

// Fire a missile towards the player ship
void Saucer::fireMissile()
{
    // Get the target's coordinates
    QPoint targetCoor = target->coordinates();

    // Calculate the angle
    double deltaY = targetCoor.y() - y();
    double deltaX = targetCoor.x() - x();

    double angleInDegrees = ::atan2(deltaY, deltaX) * 180 / Pi + 90;

    // Temporarily rotate the coordinate system and crate the missile object
    qreal oldRotation = rotation();
    setRotation(angleInDegrees);

    Missile *newMissile = new Missile(false, parent());
    int frontOffset = - ( length / 2 + newMissile->getLength() );
    newMissile->setPos(mapToParent(0, frontOffset));
    newMissile->setRotation(angleInDegrees);
    scene()->addItem(newMissile);

    // Restore old rotation
    setRotation(oldRotation);
}

Saucer::~Saucer()
{
    if(gif_anim) delete gif_anim;
}
