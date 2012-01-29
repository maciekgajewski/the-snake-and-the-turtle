#ifndef TURTLEMODULE_TURTLE_HPP
#define TURTLEMODULE_TURTLE_HPP

#include <QObject>
#include <QGraphicsItem>
#include <QPolygonF>
#include <QPen>
#include <QMutex>


#include <map>

namespace TurtleModule
{

class TurtleScreen;

/// The turtle class implementation
class Turtle : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    typedef std::pair<QString, QPolygonF> NamedShape;
    typedef std::map<QString, QPolygonF> PolygonDictionary;

    Turtle(TurtleScreen* parent);

public: // QGraphicsItem

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget=0);
    virtual QRectF boundingRect() const;

public: // intra-thread state getters

    bool isvisible();
    QPointF position();
    QSizeF shapesize();
    bool isdown();
    QPen pen();
    QBrush brush();
    double rotationDegrees();
    double rotationRadians();
    double heading();

public:

    // shape dict operations
    QPolygonF getShape(const QString& name);
    QString turtleshape();

public Q_SLOTS:

    void showturtle();
    void hideturtle();
    void reset();
    void shapesize(const QSizeF& size);
    void turtleshape(const QString& s);
    void setPenDown(bool down);
    void setPen(const QPen& pen);
    void setBrush(const QBrush& brush);
    void forward(double steps);
    void goTo(const QPointF& target);
    void circle(double radius, double extent = 0, unsigned int steps = 0);
    int stamp();
    void left(double angle);
    void setheading(double angle);
    void beginFill();
    void endFill();
    void home();

private:

    QMutex _mutex;
    QList<QGraphicsItem*> _drawing;
    TurtleScreen* _screen;

    double angleToRadians(double angle); // convers external angle usnits to radians

    // state, available across threads

    bool _visible;
    QPen _pen;
    QBrush _brush;
    QPointF _position;
    double _rotationRadians;
    QSizeF _shapesize;
    bool _isdown;
    double _fullcircle; // angle units per fullcircle

    QPolygonF _shape;
    QString _shapename;

    QPolygonF _filledShape;
    bool _filling;

    static PolygonDictionary _shapes;

};


} // namespace TurtleModule
#endif // TURTLEMODULE_TURTLE_HPP
