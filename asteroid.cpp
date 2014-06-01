#include "asteroid.h"

/* Constructor
 * @param size : 1 - small, 2 - medium, 3 - large
 * @param parent : the parent in the Qt hierarchy
 */
Asteroid::Asteroid(int size, QObject *parent) : QObject(parent) {
    // Set the size of the asteroid
    this->size = size;

    // Calculate the rectangle coordinates and lengths
    cornerCoor = -13 * size;
    length = 26 * size;

    // Calculate the offset that's going to be used for the actual shape of the asteroid
    cornerShapeOffset = size;
    lengthShapeOffset = size * 2;

    // Set the moving direction
    m_dx = qrand()%2 + 1;
    m_dy = qrand()%2 + 1;

    // Set the random rotation
    int maxRotation = 2;
    if (qrand() % 1) {
        randRotation = (qrand() % maxRotation + 1);
    } else {
        randRotation = - (qrand() % maxRotation + 1);
    }

    // Pick an image
    int asteroidImg = qrand() % 4 + 1;
    img = QPixmap(QString(":/asteroids/resource/%1.png").arg(asteroidImg));

    // Connect to the slot that plays the sound
    connect(this, SIGNAL(explosionSound()), parent, SLOT(playExplosionSound()));

    // Connect to the slot that updates the score
    connect(this, SIGNAL(asteroidKilled(int)), parent, SLOT(updateScore(int)));

    // Connect to the slot that updates the object counter
    connect(this, SIGNAL(updateObjectCount(int)), parent, SLOT(updateObjectCounter(int)));

    // Update the object counter: +1 as we spawn the asteroid
    emit updateObjectCount(1);

    // Nullify the pointer for the explosion .gif
    gif_anim = 0;
}

// Determine the maximum area available for displaying content
QRectF Asteroid::boundingRect() const {
    return QRectF(cornerCoor, cornerCoor, length, length);
}

// Determine the actual shape of the asteroid - this is used in collision detection
QPainterPath Asteroid::shape() const {
    QPainterPath path;

    path.addEllipse(cornerCoor + cornerShapeOffset, cornerCoor, length - lengthShapeOffset, length);

    return path;
}

// Determine the look of the asteroid
void Asteroid::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    // Print the asteroid image
    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);
    painter->drawPixmap(boundingRect().x(), boundingRect().y(), boundingRect().width(), boundingRect().height(), img);
}

// Move the asteroid
void Asteroid::advance(int step){
    if (!step) return;

    // Move the asteroid
    double thisX = x() + m_dx;
    double thisY = y() + m_dy;
    this->setPos(thisX, thisY);
    setRotation(rotation() + randRotation);

    // If the asteroid leaves the scene, move it back
    if (!scene()->sceneRect().contains(thisX, thisY)) {
        // Test - fix the issue of blinking/stuck asteroids
        if(thisX > 0) { thisX -= 10; }
        else { thisX += 10; }

        if(thisY > 0) { thisY -= 10; }
        else { thisY += 10; }

        setPos(-thisX, -thisY);
    }

    // Respond to collision with other items
    foreach (QGraphicsItem *item, scene()->collidingItems(this)) {
        // Check if it was a missile
        Missile *aMissile = qgraphicsitem_cast<Missile *>(item);
        if (aMissile != NULL)
        {
            // If it was a missile from the ship, give points to the player
            if (aMissile->firedFromShip()) {
                emit asteroidKilled(size);
            }

            // Destroy the asteroid and the missile
            destoyItem();
            aMissile->destoyItem();
            return;
        }

        // Bounce the asteroids off each other
        int otherX = item->x();
        int otherY = item->y();

        if (thisX < otherX) { m_dx = -::abs(m_dx); }
        else { m_dx = ::abs(m_dx); }

        if (thisY < otherY) { m_dy = -::abs(m_dy); }
        else { m_dy = ::abs(m_dy); }
    }
}

// Destroy the asteroid and brake it down to smaller one if possible
void Asteroid::destoyItem()
{
    // Use the current coordinates
    double thisX = x();
    double thisY = y();

    // Spawn smaller asteroids
    if(size > 1) {
        QObject *asteroidParent = parent();
        int childSize = --size;

        // Build the first child and set its coordinates somewhere within the parent asteroid
        Asteroid *childOne = new Asteroid(childSize, asteroidParent);
        double oneX = MainWindow::randInt(thisX - length/2, thisX + length/2);
        double oneY = MainWindow::randInt(thisY - length/2, thisY + length/2);
        childOne->setPos(oneX, oneY);

        // Build the second child and set its coordinates somewhere within the parent asteroid
        Asteroid *childTwo = new Asteroid(childSize, asteroidParent);
        double twoX = MainWindow::randInt(thisX - length/2, thisX + length/2);
        double twoY = MainWindow::randInt(thisY - length/2, thisY + length/2);
        childTwo->setPos(twoX, twoY);

        // Add both child asteroids to the scene
        scene()->addItem(childOne);
        scene()->addItem(childTwo);
    }

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

    // Remove the asteroid from the field
    scene()->removeItem(this);

    // Update the object counter: -1 as we will remove the asteroid
    emit updateObjectCount(-1);
}

Asteroid::~Asteroid()
{
    if(gif_anim) delete gif_anim;
}
