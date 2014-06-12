#include "ship.h"

/* Constructor
 * @param parent : the parent in the Qt hierarchy
 */
Ship::Ship(QObject *parent) : QObject(parent) {
    // Flashes count
    flashes = 20;

    // Ship dimmensions
    width = 60;
    height = 45;

    // Start position and rotation
    int startX = 0;
    int startY = 0;
    setPos(mapToParent(startX, startY));
    setRotation(0);

    // Make the ship focusable
    setFlags(this->flags() | QGraphicsItem::ItemIsFocusable );

    // Key flags
    keyLeft = false;
    keyRight = false;
    keyUP = false;

    // Set angle
    angle = 7;

    // Set speed
    speed = -5;

    // Set the image of the ship
    shipImage = QPixmap(QString(":/ship/resource/Ship.png"));
    gif_anim = 0;

    // Connect to the slot that plays the sound
    connect(this, SIGNAL(explosionSound()), parent, SLOT(playExplosionSound()));

    // Flashing
    flash = new QTimer(this);
    flash->start(100);
    connect(flash, SIGNAL(timeout()), this, SLOT(flashEvent()));

    inertia = 0;

    setFocus();
}

// Determine the maximum area available for displaying content
QRectF Ship::boundingRect() const
{
    return QRect(-(width / 2), -(height / 2), width, height);
}

// Determine the actual shape of the ship - this is used in collision detection
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

// Determine the look of the ship
void Ship::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //QBrush brush = QBrush(Qt::red);
    //QPainterPath shipShape = shape();

    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);
    painter->drawPixmap(boundingRect().x(), boundingRect().y(), boundingRect().width(), boundingRect().height(), shipImage);

    //painter->drawPath(shipShape);
    //painter->fillPath(shipShape, brush);
}

void Ship::advance(int step) {
    if (!step) return;

    double thisX = x();
    double thisY = y();

    // If the ship leaves the scene, move it back
    if (!scene()->sceneRect().contains(thisX, thisY)) {
        setPos(-thisX, -thisY);
    }

    // Ship movement
    if (keyUP) setPos(mapToParent(0, speed));
    if (keyLeft) setRotation(rotation() - angle);
    if (keyRight) setRotation(rotation() + angle);

    // Ship Inertia
    if(inertia < 0) {
        // Decrease the inertion
        inertia += 0.1;

        // Move the Ship
        setPos(mapToParent(0, inertia));
    }

    // Respond to collision with other items
    if (flashes < 1) {
        QList<QGraphicsItem *> collidingItems = scene()->collidingItems(this);
        if (!collidingItems.isEmpty()) {
            // Check if it was a missile
            QGraphicsItem * item = collidingItems[0];

            Missile *aMissile = qgraphicsitem_cast<Missile *>(item);
            if (aMissile != NULL) {
                // Check if the missile was fired from the ship, in which case - ignore it
                if(aMissile->firedFromShip()) {
                    return;
                }

                aMissile->destoyItem();
            }

            emit shipDestroyed();
            destroyShip(); // The ship was hit, so it has to be destroyed
        }
    }
}

// Destroy the ship
void Ship::destroyShip() {
    // Use the current coordinates
    double thisX = x();
    double thisY = y();

    // Add an explosion effect
    emit explosionSound();

    QMovie *movie = new QMovie(this);
    movie->setFileName(":/FX/resource/explosion.gif");
    movie->setScaledSize(QSize(height, height));
    movie->setSpeed(250);
    movie->start();

    gif_anim = new QLabel();
    gif_anim->setMovie(movie);
    gif_anim->setAttribute(Qt::WA_TranslucentBackground, true);

    QGraphicsProxyWidget *proxy = scene()->addWidget(gif_anim);
    proxy->setParent(this);
    proxy->setPos(thisX - gif_anim->width()/2, thisY - gif_anim->height()/2);

    // Schedule the object for deleting after the explosion gif finishes
    connect(movie, SIGNAL(finished()), this, SLOT(deleteLater()));

    // Remove the ship from the field
    scene()->removeItem(this);
}

// Sets the flags for the keys to true if they are pressed or fires a missile
void Ship::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) { return; }
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W) { keyUP = true; }
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) { keyLeft = true; }
    if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) { keyRight = true; }
    if (event->key() == Qt::Key_Space) { fireMissile(); }
}

// Sets the flags for the keys to false when the keys are released, also sets the inertia speed
void Ship::keyReleaseEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) { return; }
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W) { keyUP = false; inertia = speed; }
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) { keyLeft = false; }
    if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) { keyRight = false; }
}

// Fire a missile towards the target
void Ship::fireMissile() {
    Missile *newMissile = new Missile(true, parent());
    int frontOffset = -(height / 2 + newMissile->getLength());

    newMissile->setPos(mapToParent(0, 1.05 * frontOffset));
    newMissile->setRotation(rotation());
    scene()->addItem(newMissile);
}

// Blinking the ship for 2 seconds, used for the ship invulnerability
void Ship::flashEvent() {
    if(effectiveOpacity() > 0) {
        setOpacity(0);
    } else {
        setOpacity(1);
    }

    --flashes;
    if (flashes < 1) { flash->stop(); flash->deleteLater();}
}

Ship::~Ship() {}
