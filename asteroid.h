#ifndef ASTEROID_H
#define ASTEROID_H

#include <QGraphicsItem>
#include <QGraphicsView>

class Asteroid : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    Asteroid(int size);

    QRectF boundingRect() const;                                                                // Determine the bounding rectangle of the asteroid
    QPainterPath shape() const;                                                                 // Determine the shape of the asteroid
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);     // Paint the object

protected:
    void advance(int step);

    ~Asteroid();

private:
     double m_dx;               // Horizontal speed (delta x)
     double m_dy;               // Vertical speed (delta y)
     int size;                  // Size of the asteroid (1 - smallest, 3 - largest)
     int cornerCoor;            // Top-left coordinate of the asteroid - (0,0) will be the center
     int length;                // Length of the asteroid
     int cornerShapeOffset;     // Offset from the corner - turns the shape from a circle into a ellipse
     int lengthShapeOffset;     // Offset of the length - turns the shape from a circle into a ellipse
     QPixmap img;               // The image of the asteroid
};

#endif // ASTEROID_H
