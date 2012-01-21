#include "Turtle.hpp"

#include <QPainter>
#include <QMutexLocker>
#include <QApplication>

#include <cmath>

namespace TurtleModule
{

static QPolygonF circle(unsigned int segments, double radius)
{
    QPolygonF p;
    for(unsigned int i = 0; i < segments; ++i)
    {
        double a = 3.14159*2.0*i / segments;
        p << QPointF(radius * std::sin(a), radius * std::cos(a));
    }

    return p;
}

static Turtle::NamedShape shapes[] = {
    Turtle::NamedShape(
        "classic",
        QPolygonF(QPolygonF() << QPointF(0,10) << QPointF(-5,-3) << QPointF(0,0) << QPointF(5,-3))),
    Turtle::NamedShape(
        "square",
        QPolygonF(QPolygonF() << QPointF(10,10) << QPointF(10,-10) << QPointF(-10,-10) << QPointF(-10,10))),
    Turtle::NamedShape(
        "circle",
        circle(20,5)),

};

Turtle::PolygonDictionary Turtle::_shapes(shapes, shapes + sizeof(shapes)/sizeof(Turtle::NamedShape));

Turtle::Turtle(QObject *parent) : QObject(parent), _mutex(QMutex::Recursive)
{
    reset();
}

void Turtle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(_pen);
    painter->setBrush(_brush);

    painter->drawPolygon(_shape);
}

QRectF Turtle::boundingRect() const
{
    return _shape.boundingRect().adjusted(_pen.widthF(), _pen.widthF(), _pen.widthF(), _pen.widthF());
}

bool Turtle::isvisible()
{
    QMutexLocker l(&_mutex);
    return _visible;
}

QPointF Turtle::position()
{
    QMutexLocker l(&_mutex);

    return _position;
}

QSizeF Turtle::shapesize()
{
    QMutexLocker l(&_mutex);
    return _shapesize;
}

bool Turtle::isdown()
{
    QMutexLocker l(&_mutex);

    return _isdown;
}

QPen Turtle::pen()
{
    QMutexLocker l(&_mutex);
    return _pen;
}

QBrush Turtle::brush()
{
    QMutexLocker l(&_mutex);
    return _brush;
}

void Turtle::showturtle()
{
    QMutexLocker l(&_mutex);
    _visible = true;

    show();
    QApplication::processEvents();
}

void Turtle::hideturtle()
{
    QMutexLocker l(&_mutex);
    _visible = false;

    hide();
    QApplication::processEvents();
}

void Turtle::reset()
{
    QMutexLocker l(&_mutex);

    prepareGeometryChange();

    turtleshape("classic");
    _pen = QPen(Qt::black, 1);
    _brush = Qt::black;
    _visible = true;
    _position = QPointF(0,0);
    _shapesize = QSizeF(1.0, 1.0);
    setTransform(QTransform());
    _isdown = true;

    setPos(_position);
}

void Turtle::shapesize(const QSizeF &size)
{
    QMutexLocker l (&_mutex);

    if (_shapesize != size)
    {
        prepareGeometryChange();
        _shapesize = size;
        QTransform newTrandform;
        newTrandform.scale(_shapesize.width(), _shapesize.height());
        setTransform(newTrandform);
    }
}

void Turtle::turtleshape(const QString &s)
{
    QMutexLocker l(&_mutex);

    if (s != _shapename)
    {
        auto it = _shapes.find(s);
        if (it != _shapes.end())
        {
            prepareGeometryChange();
            _shapename = s;
            _shape = it->second;
        }
    }

    QApplication::processEvents();
}

void Turtle::setPenDown(bool down)
{
    QMutexLocker l(&_mutex);

    _isdown = down;
    QApplication::processEvents();
}

void Turtle::setPen(const QPen &pen)
{
    prepareGeometryChange();
    _pen = pen;
    QApplication::processEvents();
}

void Turtle::setBrush(const QBrush &brush)
{
    prepareGeometryChange();
    _brush = brush;
    QApplication::processEvents();

}

QString Turtle::turtleshape()
{
    QMutexLocker l(&_mutex);
    return _shapename;

}


} // namespace TurtleModule
