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

static PyObject* no_params(Turtle* s, const char* method)
{
    if (s)
    {
        qDebug() << "turtle no_params: " << method;
        bool res = QMetaObject::invokeMethod(s, method, Qt::QueuedConnection);
        QApplication::processEvents();
        qDebug() << "invoked: " << res;
        Py_RETURN_NONE;
    }

    return NULL;
}

PyObject* showturtle_global(PyObject *self, PyObject *args)
{
    return no_params(getTurtle(), "showturtle");
}

PyObject* hideturtle_global(PyObject *self, PyObject *args)
{
    return no_params(getTurtle(), "hideturtle");
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
        if (PyArg_ParseTuple(args, "|ff", &w, &h))
        {
            if (w == 0 && h == 0)
            {
                QSizeF s = t->shapesize();
                return Py_BuildValue("fff", s.width(), s.height(), 1.0);
            }

            if (h == 0)
                h = w;

            QSizeF newSize(w, h);
            QMetaObject::invokeMethod(t, "shapesize", Qt::QueuedConnection, Q_ARG(QSizeF, newSize));
            QApplication::processEvents();
            Py_RETURN_NONE;
        }
    }
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
    setdown(getTurtle(), false);
}

PyObject* pendown_global(PyObject *self, PyObject *args)
{
    setdown(getTurtle(), true);
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
    shape(getTurtle(), args);
}


static PyObject* pencolor(Turtle* t, PyObject *args)
{
    if (t)
    {
        QColor c = argsToColor(args);
        QPen p = t->pen();
        if (c.isValid())
        {
            p.setColor(c);
            QMetaObject::invokeMethod(t, "setPen", Qt::QueuedConnection, Q_ARG(QPen, p));
            QApplication::processEvents();
            Py_RETURN_NONE;
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
        QColor c = argsToColor(args);
        QBrush b = t->brush();
        if (c.isValid())
        {
            b.setColor(c);
            QMetaObject::invokeMethod(t, "setBrush", Qt::QueuedConnection, Q_ARG(QBrush, b));
            QApplication::processEvents();
            Py_RETURN_NONE;
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

PyObject* color_global(PyObject *self, PyObject *args)
{
    // TODO handle both colors
    pencolor_global(self, args);
}


} // namespace TurtleModule
