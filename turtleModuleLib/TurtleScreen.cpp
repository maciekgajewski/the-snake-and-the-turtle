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
#include "TurtleScreen.hpp"
#include "Turtle.hpp"

#include <QApplication>
#include <QDebug>

namespace TurtleModule {

TurtleScreen::TurtleScreen(QWidget *parent) :
    QGraphicsView( parent),

    _bgcolor(Qt::white),
    _mode((Mode)-1)
{
    setRenderHint(QPainter::Antialiasing);
    setScene(&_scene);
    _scene.setBackgroundBrush(_bgcolor);

    _turtles.append(new Turtle(this));
    _scene.addItem(_turtles[0]);
    mode(MODE_STANDARD);
}

QColor TurtleScreen::bgcolor()
{
    QMutexLocker l(&_mutex);
    return _bgcolor;
}

TurtleScreen::Mode TurtleScreen::mode()
{
    QMutexLocker l(&_mutex);
    return _mode;
}

double TurtleScreen::neutralRotationRadians() const
{
    if (_mode == MODE_LOGO)
        return 0;
    else
        return -3.14159/2;
}

double TurtleScreen::rotationMultiplier() const
{
    if (_mode == MODE_LOGO)
        return -1.0;
    else
        return 1.0;
}

Turtle *TurtleScreen::turtle() const
{
    return _turtles[0];
}

void TurtleScreen::bgcolor(const QColor &color)
{
    QMutexLocker l(&_mutex);
    _bgcolor = color;
    _scene.setBackgroundBrush(color);

    QApplication::processEvents();
}

void TurtleScreen::mode(int m)
{
    QMutexLocker l(&_mutex);
    if (m != _mode)
    {
        _mode = (Mode)m;
        switch(_mode)
        {
            case MODE_LOGO:
            {
                QTransform t;
                t.scale(1.0, -1.0);
                setTransform(t);
                break;
            }

            case MODE_STANDARD:
            {
                QTransform t;
                t.scale(1.0, -1.0);
                setTransform(t);
            }

            case MODE_WORLD:
            {
                if (!_worldCoords.isNull())
                {
                    fitInView(_worldCoords);
                }
                break;
            }
        }
        reset();
    }
}

void TurtleScreen::setworldcoordinates(const QRectF& coords)
{
    mode(MODE_WORLD);
    _worldCoords = coords;
    fitInView(coords);

    QApplication::processEvents();
}

void TurtleScreen::reset()
{
    QTransform t;
    t.scale(1.0, -1.0);
    setTransform(t);

    Q_FOREACH(Turtle* t, _turtles)
    {
        t->reset();
    }

    QApplication::processEvents();
}

void TurtleScreen::setup(const QRect &geometry)
{
    qDebug() << "TurtleScreen: setup";
    // does nothing in this class
}

void TurtleScreen::mainloop()
{
    qDebug() << "TurtleScreen: mainloop";
    // does nothing in this class
}

void TurtleScreen::bye()
{
    qDebug() << "TurtleScreen: bye";
    // does nothing in this class
}

void TurtleScreen::exitonclick()
{
    qDebug() << "TurtleScreen: exitonclick";
    // does nothing in this class
}

} // namespace TurtleModule
