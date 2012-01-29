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
#include "TurtleFunctions.hpp"
#include "Turtle.hpp"
#include "Module.hpp"
#include "TurtleScreen.hpp"
#include "globalFunctions.hpp"

#include <QDebug>
#include <QApplication>

#include <limits>

namespace TurtleModule {

static Turtle* getTurtle()
{
    return Module::instance()->screen()->turtle();
}

PyObject* showturtle_global(PyObject *self, PyObject *args)
{
    return invoke(getTurtle(), "showturtle");
}

PyObject* hideturtle_global(PyObject *self, PyObject *args)
{
    return invoke(getTurtle(), "hideturtle");
}

PyObject* isvisible_global(PyObject *self, PyObject *args)
{
    bool visible = getTurtle()->isvisible();
    if (visible)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyObject* shapesize(Turtle* t, PyObject *args)
{
    double w = 0.0;
    double h = 0.0;

    if (t)
    {
        if (PyArg_ParseTuple(args, "|dd", &w, &h))
        {
            if (w == 0 && h == 0)
            {
                QSizeF s = t->shapesize();
                return Py_BuildValue("ddd", s.width(), s.height(), 1.0);
            }

            if (h == 0)
                h = w;

            QSizeF newSize(w, h);
            QMetaObject::invokeMethod(t, "shapesize", Qt::QueuedConnection, Q_ARG(QSizeF, newSize));
            QApplication::processEvents();
            Py_RETURN_NONE;
        }
    }

    return NULL;
}


PyObject* shapesize_global(PyObject */*self*/, PyObject *args)
{
    return shapesize(getTurtle(), args);
}

static PyObject* setdown(Turtle* t, bool down)
{
    if (t)
    {
        QMetaObject::invokeMethod(t, "setPenDown", Qt::QueuedConnection, Q_ARG(bool, down));
        QApplication::processEvents();
        Py_RETURN_NONE;
    }

    return NULL;
}

PyObject* penup_global(PyObject *self, PyObject *args)
{
    return setdown(getTurtle(), false);
}

PyObject* pendown_global(PyObject *self, PyObject *args)
{
    return setdown(getTurtle(), true);
}

PyObject* isdown_global(PyObject *self, PyObject *args)
{
    if (getTurtle()->isdown())
    {
        Py_RETURN_TRUE;
    }
    else
    {
        Py_RETURN_FALSE;
    }
}

static PyObject* shape(Turtle* t, PyObject* args)
{
    if (t)
    {
        const char* shapeName = nullptr;
        if (PyArg_ParseTuple(args, "|s", &shapeName))
        {
            if (shapeName)
            {
                QString s(shapeName);
                QMetaObject::invokeMethod(t, "turtleshape", Qt::QueuedConnection, Q_ARG(QString, s));
                QApplication::processEvents();
                Py_RETURN_NONE;
            }
            else
            {
                QString s = t->turtleshape();
                return PyUnicodeUCS2_FromString(s.toAscii().data());
            }
        }
    }

    return NULL;
}

PyObject* shape_global(PyObject */*self*/, PyObject *args)
{
    return shape(getTurtle(), args);
}


static PyObject* pencolor(Turtle* t, PyObject *args)
{
    if (t)
    {
        PyObject* o = PyTuple_GetItem(args, 0);
            QColor c = objectToColor(o);
            QPen p = t->pen();
            if (c.isValid())
            {
                p.setColor(c);
                return invoke(t, "setPen", Q_ARG(QPen, p));
            }
            else
            {
                return colorToTuple(p.color());
            }
    }

    return NULL;
}

PyObject* pencolor_global(PyObject */*self*/, PyObject *args)
{
    return pencolor(getTurtle(), args);
}

static PyObject* fillcolor(Turtle* t, PyObject *args)
{
    if (t)
    {
        PyObject* o = PyTuple_GetItem(args, 0);
            QColor c = objectToColor(o);
            QBrush b = t->brush();
            if (c.isValid())
            {
                b.setColor(c);
                return invoke(t, "setBrush", Q_ARG(QBrush, b));
            }
            else
            {
                return colorToTuple(b.color());
            }
    }

    return NULL;
}

PyObject* fillcolor_global(PyObject */*self*/, PyObject *args)
{
    return fillcolor(getTurtle(), args);
}

static PyObject* color(Turtle* t, PyObject *args)
{
    if (t)
    {
        QColor penColor;
        QColor fillColor;

        PyObject* o1 = PyTuple_GetItem(args, 0);
        PyObject* o2 = PyTuple_GetItem(args, 1);;
        PyErr_Clear();

            penColor = objectToColor(o1);
            fillColor = objectToColor(o2);
            QPen pen = t->pen();
            QBrush brush = t->brush();

            if (penColor.isValid())
            {
                pen.setColor(penColor);
                invoke(t, "setPen", Q_ARG(QPen, pen));
            }
            if (!fillColor.isValid())
                fillColor = penColor;
            if (fillColor.isValid())
            {
                brush.setColor(fillColor);
                invoke(t, "setBrush", Q_ARG(QBrush, brush));
            }

        if (PyErr_Occurred() == NULL)
            Py_RETURN_NONE;
    }

    return NULL;
}

PyObject* color_global(PyObject *self, PyObject *args)
{
    return color(getTurtle(), args);
}

static PyObject* forward(Turtle* t, PyObject* args, double mply)
{
    if (t)
    {
        double steps;
        if (PyArg_ParseTuple(args, "d", &steps))
        {
            return invokeWait(t, "forward", Q_ARG(double, steps*mply));
        }
    }

    return NULL;
}

static PyObject* goTo(Turtle* t, PyObject* args)
{
    if (t)
    {
        double x,y;
        if (PyArg_ParseTuple(args, "dd", &x, &y))
        {
            QPointF p(x, y);
            return invokeWait(t, "goTo", Q_ARG(QPointF, p));
        }
    }

    return NULL;
}

PyObject* forward_global(PyObject *self, PyObject *args)
{
    return forward(getTurtle(), args, 1.0);
}

PyObject* backward_global(PyObject *self, PyObject *args)
{
    return forward(getTurtle(), args, -1.0);
}


PyObject* goto_global(PyObject *self, PyObject *args)
{
    return goTo(getTurtle(), args);
}

PyObject* stamp_global(PyObject *self, PyObject *args)
{
    return invoke(getTurtle(), "stamp");
}

PyObject* reset_global(PyObject *self, PyObject *args)
{
    return invoke(getTurtle(), "reset");
}

static PyObject* left(Turtle* t, PyObject* args, double mply)
{
    if (t)
    {
        double angle;
        if (PyArg_ParseTuple(args, "d", &angle))
        {
            return invokeWait(t, "left", Q_ARG(double, angle*mply));
        }
    }

    return NULL;
}

PyObject* left_global(PyObject *self, PyObject *args)
{
    return left(getTurtle(), args, 1.0);
}

PyObject* right_global(PyObject *self, PyObject *args)
{
    return left(getTurtle(), args, -1.0);
}

static PyObject* setheading(Turtle* t, PyObject* args)
{
    if (t)
    {
        double angle;
        if(PyArg_ParseTuple(args, "d", &angle))
        {
            return invoke(t, "setheading", Q_ARG(double, angle));
        }
    }
    return NULL;
}

PyObject* setheading_global(PyObject *self, PyObject *args)
{
    return setheading(getTurtle(), args);
}

static PyObject* heading(Turtle* t)
{
    if (t)
    {
        return PyFloat_FromDouble(t->heading());
    }
    return NULL;
}

PyObject* heading_global(PyObject *self, PyObject *args)
{
    return heading(getTurtle());
}

static PyObject* width(Turtle* t, PyObject *args)
{
    if (t)
    {
        double w = -1;
        if(PyArg_ParseTuple(args, "|d", &w))
        {
            QPen pen = t->pen();
            if (w >= 0)
            {
                pen.setWidthF(w);
                return invoke(t, "setPen", Q_ARG(QPen, pen));
            }
            else
            {
                return PyFloat_FromDouble(pen.widthF());
            }
        }
    }
    return NULL;
}

PyObject* width_global(PyObject *self, PyObject *args)
{
    return width(getTurtle(),args);
}

PyObject* begin_fill_global(PyObject *self, PyObject *args)
{
    return invoke(getTurtle(), "beginFill");
}

PyObject* end_fill_global(PyObject *self, PyObject *args)
{
    return invoke(getTurtle(), "endFill");
}

static PyObject* circle(Turtle* t, PyObject *args, PyObject* keywords)
{
    if (t)
    {
        static char *kwlist[] = {"radius", "extent", "steps", NULL};
        double radius = 0;
        double extent = 0;
        unsigned int steps = 0;
        if (PyArg_ParseTupleAndKeywords(args, keywords, "d|dI", kwlist, &radius, &extent, &steps))
        {
            return invokeWait(t, "circle", Q_ARG(double,radius), Q_ARG(double,extent), Q_ARG(unsigned int,steps));
        }
    }

    return NULL;
}

PyObject* circle_global(PyObject *self, PyObject *args, PyObject* keywords)
{
    return circle(getTurtle(), args, keywords);
}

PyObject* home_global(PyObject *self, PyObject *args)
{
    return invoke(getTurtle(), "home");
}

static PyObject* dot(Turtle* t, PyObject *args)
{
    if(t)
    {
        double size = 0;
        if (PyArg_ParseTuple(args, "|d", &size))
        {
            if (size <= 0.0)
            {
                double pensize = t->pen().widthF();
                size = std::max(pensize * 2, pensize + 4);
            }
            return invoke(t, "dot", Q_ARG(double, size));
        }
    }

    return NULL;
}

PyObject* dot_global(PyObject *self, PyObject *args)
{
    return dot(getTurtle(), args);
}


} // namespace TurtleModule
