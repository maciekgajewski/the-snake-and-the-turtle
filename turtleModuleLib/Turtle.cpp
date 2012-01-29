#include "Turtle.hpp"

#include "TurtleScreen.hpp"

#include <QPainter>
#include <QMutexLocker>
#include <QApplication>
#include <QDebug>
#include <QTransform>

#include <cmath>

namespace TurtleModule
{

static const double PI = 3.14159;
static const double TWOPI = PI*2.0;

static QPolygonF circle(unsigned int segments, double radius)
{
    QPolygonF p;
    for(unsigned int i = 0; i < segments; ++i)
    {
        double a = TWOPI*i / segments;
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
        "triangle",
        QPolygonF(QPolygonF() << QPointF(0,20) << QPointF(-10,0) << QPointF(10,0))),
    Turtle::NamedShape(
        "circle",
        circle(20,5)),

};

Turtle::PolygonDictionary Turtle::_shapes(shapes, shapes + sizeof(shapes)/sizeof(Turtle::NamedShape));

Turtle::Turtle(TurtleScreen *parent)
    : QObject(parent), _mutex(QMutex::Recursive), _screen(parent)
{
    reset();
    setZValue(1.0);
    setFlag(ItemIgnoresTransformations);
}

void Turtle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(_pen);
    painter->setBrush(_brush);

    painter->drawPolygon(_shape);
}

QRectF Turtle::boundingRect() const
{
    return _shape.boundingRect().adjusted(-_pen.widthF(), -_pen.widthF(), _pen.widthF(), _pen.widthF());
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

double Turtle::rotationDegrees()
{
    QMutexLocker l(&_mutex);
    return 360*_rotationRadians / TWOPI;
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
    _rotationRadians = _screen->neutralRotationRadians();
    _shapesize = QSizeF(1.0, 1.0);
    setTransform(QTransform());
    _isdown = true;
    _fullcircle = 360.0;
    _filling = false;
    _filledShape.clear();

    setPos(_position);
    setRotation(rotationDegrees());

    // delete drawing
    Q_FOREACH(QGraphicsItem* i, _drawing)
    {
        delete i;
    }
    _drawing.clear();
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
        update();
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
    update();
    QApplication::processEvents();
}

void Turtle::setBrush(const QBrush &brush)
{
    prepareGeometryChange();
    _brush = brush;
    update();
    QApplication::processEvents();

}

void Turtle::forward(double steps)
{
    QPointF to = _position + QTransform().scale(1,-1).rotateRadians(_rotationRadians).map(QPointF(0, steps));
    goTo(to);
}

void Turtle::goTo(const QPointF &target)
{
    QMutexLocker l(&_mutex);

    // TODO implement animation
    if (_isdown)
    {
        QLineF line(_screen->transform().map(QLineF(_position, target)));
        QGraphicsLineItem* item = new QGraphicsLineItem(line, 0, scene());
        item->setFlag(ItemIgnoresTransformations);
        item->setPen(_pen);
        _drawing.append(item);
    }
    _position = target;
    if (_filling)
        _filledShape.append(_position);
    prepareGeometryChange();
    setPos(_position);
    QApplication::processEvents();
}

void Turtle::circle(double radius, double extent, unsigned int steps)
{
    // calculate extent
    if (extent == 0)
        extent = _fullcircle;

    // calculate steps
    if (steps == 0)
        steps = 36; // TODO make it depending on radius and extent

    // calculate move components
    double leftAngle = extent/steps;
    double stepsForward = radius * std::sin(angleToRadians(leftAngle));

    // draw!
    for (unsigned int i = 0; i < steps; ++i)
    {
        left(leftAngle);
        forward(stepsForward);
    }
}

int Turtle::stamp()
{
    // create a copy of turtle
    QGraphicsPolygonItem* item = new QGraphicsPolygonItem(_shape, 0, scene());
    item->setFlag(ItemIgnoresTransformations);
    item->setPen(_pen);
    item->setBrush(_brush);
    item->setPos(_position);
    item->setRotation(rotationDegrees());
    item->setTransform(transform());

    _drawing.append(item);
    QApplication::processEvents();
    return _drawing.size() - 1; // stamp-id is an index in the _drawing
}

void Turtle::left(double angle)
{
    QMutexLocker l(&_mutex);
    // TODO implement animation
    _rotationRadians -= angle*TWOPI/_fullcircle;

    while(_rotationRadians > PI)
        _rotationRadians -= TWOPI;
    while(_rotationRadians < -PI)
        _rotationRadians += TWOPI;

    setRotation(rotationDegrees());
}

void Turtle::setheading(double angle)
{
    // TODO implement animation
    _rotationRadians = _screen->neutralRotationRadians() +  _screen->rotationMultiplier() *angle*TWOPI/_fullcircle;
    setRotation(rotationDegrees());
}

void Turtle::beginFill()
{
    _filling = true;
    _filledShape.clear();
    _filledShape.append(_position);
}

void Turtle::endFill()
{
    _filling = false;
    QGraphicsPolygonItem* filledShape = new QGraphicsPolygonItem(_filledShape, 0, scene());
    filledShape->setZValue(-1);
    filledShape->setBrush(_brush);
    filledShape->setPen(Qt::NoPen);

    _drawing.append(filledShape);

    QApplication::processEvents();
}

double Turtle::angleToRadians(double angle)
{
    return TWOPI * angle / _fullcircle;
}

QString Turtle::turtleshape()
{
    QMutexLocker l(&_mutex);
    return _shapename;

}

double Turtle::rotationRadians()
{
    QMutexLocker l(&_mutex);
    return _rotationRadians;
}

double Turtle::heading()
{
    QMutexLocker l(&_mutex);



    return (_rotationRadians-_screen->neutralRotationRadians()*_screen->rotationMultiplier()) * _fullcircle / TWOPI;
}


} // namespace TurtleModule
