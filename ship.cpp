#include "ship.h"

Ship::Ship(QObject *parent) : QObject(parent) {
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
    angle = 10;

    // Set speed
    speed = -5;

    // Set the image of the ship
    shipImage = QPixmap(QString(":/ship/resource/Ship.png"));
    gif_anim = 0;

    // Connect to the slot that plays the sound
    connect(this, SIGNAL(explosionSound()), parent, SLOT(playExplosionSound()));

    setFocus();
}

QRectF Ship::boundingRect() const
{
    return QRect(-(width / 2), -(height / 2), width, height);
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

QPoint Ship::coordinates()
{
    return QPoint(x(), y());
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

    // Respond to collision with other items
    QList<QGraphicsItem *> collidingItems = scene()->collidingItems(this);
    if (!collidingItems.isEmpty()) {
        QGraphicsItem * item = collidingItems[0];
        destroyShip(); // The ship was hit, so it has to be destroyed

        // Check if it was a missile
        Missile *aMissile = qgraphicsitem_cast<Missile *>(item);
        if (aMissile != NULL) {
            // If a missile from the saucer hit the ship, destroy the ship
            if (aMissile->firedFromSaucer()) {
                emit shipDestroyed();
            }

            // Destroy the missile
            aMissile->destoyItem();
            return;
        }
    }
}

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

void Ship::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) { return; }
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W) { keyUP = true; }
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) { keyLeft = true; }
    if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) { keyRight = true; }
    if (event->key() == Qt::Key_Space) { fireMissile(); }
}

void Ship::keyReleaseEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) { return; }
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W) keyUP = false;
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) keyLeft = false;
    if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) keyRight = false;
}

void Ship::fireMissile() {
    Missile *newMissile = new Missile(true, false, parent());
    int frontOffset = -(height / 2 + newMissile->getLength());

    newMissile->setPos(mapToParent(0, 1.05 * frontOffset));
    newMissile->setRotation(rotation());
    scene()->addItem(newMissile);
}

Ship::~Ship() {}
