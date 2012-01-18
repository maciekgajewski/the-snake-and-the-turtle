/*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software  Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* Copyright (C) 2012 by Maciej Gajewski <maciej.gajewski0@gmail.com>
* All rights reserved.
*
* The Original Code is: all of this file.
*/
#include "Turtle.hpp"

#include <QDebug>
#include <QThread>

namespace Turtle {

Turtle::Turtle(QObject *parent)
    : QObject(parent),
      _buffer(1000, 1000),
      _pen(Qt::black, 1.0),
      _brush(Qt::black)
{
    reset();
}

Turtle::~Turtle()
{
}

void Turtle::reset()
{
    _buffer.fill(Qt::white);
    _transform.reset();
    _transform.translate(500, 500);
    _transform.scale(1, -1);
    _pen = QPen(Qt::black, 1);
    _brush = QBrush(Qt::black);

    Q_EMIT changed();
}

QPixmap Turtle::render() const
{
    QPixmap result(_buffer);

    QPainter painter(&result);
    initPainter(painter);

    drawTurtle(painter);

    return result;
}

QPointF Turtle::turtlePos() const
{
    return _transform.map(QPointF(0,0));
}

void Turtle::forward(double steps)
{
    QPainter painter(&_buffer);
    initPainter(painter);
    painter.drawLine(QPointF(0,0), QPointF(0, steps));
    _transform.translate(0, steps);

    Q_EMIT changed();
}

void Turtle::left(double degrees)
{
    _transform.rotate(degrees);

    Q_EMIT changed();
}

void Turtle::pencolor(const QColor &color)
{
    if (color != _pen.color())
    {
        _pen.setColor(color);

        Q_EMIT changed();
    }
}

void Turtle::fillcolor(const QColor &color)
{
    if (color != _brush.color())
    {
        _brush.setColor(color);

        Q_EMIT changed();
    }
}

void Turtle::drawTurtle(QPainter &painter) const
{
    QPolygonF shape;
    shape << QPointF(0, 10) << QPointF(-5, -5) << QPointF(0, 0) << QPointF(5, -5);

    painter.drawPolygon(shape);
}

void Turtle::initPainter(QPainter &painter) const
{
    painter.setTransform(_transform);
    painter.setPen(_pen);
    painter.setBrush(_brush);
    painter.setRenderHint(QPainter::Antialiasing);
}


} // namespace Turtle
