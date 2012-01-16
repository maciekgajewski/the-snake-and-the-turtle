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
#ifndef TURTLE_TURTLE_HPP
#define TURTLE_TURTLE_HPP

#include <QPainter>
#include <QObject>
#include <QPen>

namespace Turtle {

/// Actual Qt turtle
class Turtle : public QObject
{
    Q_OBJECT
public:
    Turtle(QObject* parent = nullptr); // painter to paint on
    virtual ~Turtle();

    // restes state
    void reset();

    // renders to pixmap
    QPixmap render() const;

    // turtle pos in pixmap coords
    QPointF turtlePos() const;

Q_SIGNALS:

    void changed(); // buffer changed

public Q_SLOTS:

    // intyerface methods are slots, so hey can be easyli called across threads

    void forward(double steps);
    void left(double degrees);
    void pencolor(const QColor& color);
    void fillcolor(const QColor& color);

private:

    // draws turtle on the painter at 0,0
    void drawTurtle(QPainter& painter) const;

    void initPainter(QPainter& painter) const;

    QPixmap _buffer; // data buffer w/o the turtle
    QTransform _transform;
    QPen _pen;
    QBrush _brush;

};

} // namespace Turtle

#endif // TURTLE_TURTLE_HPP
