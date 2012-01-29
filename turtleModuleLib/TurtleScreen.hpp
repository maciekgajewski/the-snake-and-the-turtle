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
#ifndef TURTLEMODULE_TURTLESCREEN_HPP
#define TURTLEMODULE_TURTLESCREEN_HPP

#include <QGraphicsView>
#include <QMutex>

namespace TurtleModule {

class Turtle;

/// The TurtleScreen implementation.
/// By turtle specification it is supposed to be a widget representing turtles arena.
class TurtleScreen : public QGraphicsView
{
    Q_OBJECT
    Q_ENUMS(Mode)

public:
    explicit TurtleScreen(QWidget *parent = 0);
    
    // types
    enum Mode
    {
        MODE_STANDARD,
        MODE_LOGO,
        MODE_WORLD
    };

    // inter-thread getters
    QColor bgcolor();
    Mode mode();

    // turtle's neutral heading
    double neutralRotationRadians() const;
    double rotationMultiplier() const;

    Turtle* turtle() const;

public Q_SLOTS:

    // see python Turtle documentation for description of these methods

    // window control
    void bgcolor(const QColor& color);
    void mode(int m);
    void setworldcoordinates(const QRectF& coords);
    void reset();

    // (not implemented here) Screen methods
    virtual void setup(const QRect& geometry);
    virtual void mainloop();
    virtual void bye();
    virtual void exitonclick();


private:

    QGraphicsScene* _scene;
    QMutex _mutex;
    QList<Turtle*> _turtles;

    // props
    QColor _bgcolor;
    Mode _mode;
    QRectF _worldCoords;
};

} // namespace TurtleModule

#endif // TURTLEMODULE_TURTLESCREEN_HPP
