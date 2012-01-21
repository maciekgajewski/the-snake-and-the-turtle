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

/// The turtle class implementation
class Turtle : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    typedef std::pair<QString, QPolygonF> NamedShape;
    typedef std::map<QString, QPolygonF> PolygonDictionary;

    Turtle(QObject* parent = NULL);

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

private:

    QMutex _mutex;

    // state, available across threads

    bool _visible;
    QPen _pen;
    QBrush _brush;
    QPointF _position;
    QSizeF _shapesize;
    bool _isdown;

    QPolygonF _shape;
    QString _shapename;

    static PolygonDictionary _shapes;

};


} // namespace TurtleModule
#endif // TURTLEMODULE_TURTLE_HPP
