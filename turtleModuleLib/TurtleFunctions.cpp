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

struct TurtlePyObject
{
    PyObject_HEAD
    Turtle* turtle;
};



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

PyObject* showturtle_method(TurtlePyObject* self, PyObject *args)
{
    return invoke(self->turtle, "showturtle");
}

PyObject* hideturtle_method(TurtlePyObject* self, PyObject *args)
{
    return invoke(self->turtle, "hideturtle");
}

PyObject* isvisible_method(TurtlePyObject* self, PyObject *args)
{
    bool visible = self->turtle->isvisible();
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

PyObject* shapesize_method(TurtlePyObject *self, PyObject *args)
{
    return shapesize(self->turtle, args);
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
PyObject* penup_method(TurtlePyObject* self, PyObject *args)
{
    return setdown(self->turtle, false);
}

PyObject* pendown_method(TurtlePyObject* self, PyObject *args)
{
    return setdown(self->turtle, true);
}

PyObject* isdown_method(TurtlePyObject* self, PyObject *args)
{
    if (self->turtle->isdown())
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

PyObject* shape_method(TurtlePyObject *self, PyObject *args)
{
    return shape(self->turtle, args);
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

PyObject* pencolor_method(TurtlePyObject *self, PyObject *args)
{
    return pencolor(self->turtle, args);
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

PyObject* fillcolor_method(TurtlePyObject *self, PyObject *args)
{
    return fillcolor(self->turtle, args);
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

PyObject* color_method(TurtlePyObject* self, PyObject *args)
{
    return color(self->turtle, args);
}

static PyObject* forward(Turtle* t, PyObject* args, double mply)
{
    if (t)
    {
        double steps;
        if (PyArg_ParseTuple(args, "d", &steps))
        {
            return invoke(t, "forward", Q_ARG(double, steps*mply));
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
            return invoke(t, "goTo", Q_ARG(QPointF, p));
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

PyObject* forward_method(TurtlePyObject* self, PyObject *args)
{
    return forward(self->turtle, args, 1.0);
}

PyObject* backward_method(TurtlePyObject* self, PyObject *args)
{
    return forward(self->turtle, args, -1.0);
}

PyObject* goto_method(TurtlePyObject* self, PyObject *args)
{
    return goTo(self->turtle, args);
}

PyObject* stamp_method(TurtlePyObject* self, PyObject *args)
{
    return invoke(self->turtle, "stamp");
}

PyObject* reset_method(TurtlePyObject* self, PyObject *args)
{
    return invoke(self->turtle, "reset");
}

static PyObject* left(Turtle* t, PyObject* args, double mply)
{
    if (t)
    {
        double angle;
        if (PyArg_ParseTuple(args, "d", &angle))
        {
            return invoke(t, "left", Q_ARG(double, angle*mply));
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

PyObject* left_method(TurtlePyObject *self, PyObject *args)
{
    return left(self->turtle, args, 1.0);
}

PyObject* right_method(TurtlePyObject *self, PyObject *args)
{
    return left(self->turtle, args, -1.0);
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

PyObject* setheading_method(TurtlePyObject *self, PyObject *args)
{
    return setheading(self->turtle, args);
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

PyObject* heading_method(TurtlePyObject *self, PyObject *args)
{
    return heading(self->turtle);
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

PyObject* width_method(TurtlePyObject *self, PyObject *args)
{
    return width(self->turtle,args);
}

PyObject* begin_fill_method(TurtlePyObject *self, PyObject *args)
{
    return invoke(self->turtle, "beginFill");
}

PyObject* end_fill_method(TurtlePyObject *self, PyObject *args)
{
    return invoke(self->turtle, "endFill");
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
            return invoke(t, "circle", Q_ARG(double,radius), Q_ARG(double,extent), Q_ARG(unsigned int,steps));
        }
    }

    return NULL;
}

PyObject* circle_global(PyObject *self, PyObject *args, PyObject* keywords)
{
    return circle(getTurtle(), args, keywords);
}

PyObject* circle_method(TurtlePyObject *self, PyObject *args, PyObject* keywords)
{
    return circle(self->turtle, args, keywords);
}

PyObject* home_global(PyObject *self, PyObject *args)
{
    return invoke(getTurtle(), "home");
}

PyObject* home_method(TurtlePyObject *self, PyObject *args)
{
    return invoke(self->turtle, "home");
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

PyObject* dot_method(TurtlePyObject* self, PyObject* args)
{
    return dot(self->turtle, args);
}

// ====================== turtle type =============

static void turtle_dealloc(TurtlePyObject* self)
{
    delete self->turtle;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *turtle_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    TurtlePyObject *self;

    self = (TurtlePyObject *)type->tp_alloc(type, 0);
    self->turtle = Module::instance()->screen()->createTurtle();
    return (PyObject *)self;
}

static PyObject* not_implemented_method(PyObject* /*self*/, PyObject* /*args*/)
{
    Py_RETURN_NONE;
}

static PyMethodDef Turtle_methods[] = {
    {"hideturtle", (PyCFunction)hideturtle_method, METH_NOARGS,
        "Make the turtle invisible."},
    {"ht", (PyCFunction)hideturtle_method, METH_NOARGS,
        "Make the turtle invisible."},
    {"showturtle", (PyCFunction)showturtle_method, METH_NOARGS,
        "Make the turtle visible."},
    {"st", (PyCFunction)showturtle_method, METH_NOARGS,
        "Make the turtle visible."},
    {"isvisible", (PyCFunction)isvisible_method, METH_NOARGS,
        "Return True if the Turtle is shown, False if it’s hidden."},
    {"shapesize", (PyCFunction)shapesize_method, METH_VARARGS,
        "Return or set the pen’s attributes x/y-stretchfactors and/or outline."},
    {"turtlesize", (PyCFunction)shapesize_method, METH_VARARGS,
        "Return or set the pen’s attributes x/y-stretchfactors and/or outline."},
    {"pendown", (PyCFunction)pendown_method, METH_NOARGS,
        "Pull the pen down – drawing when moving."},
    {"pd", (PyCFunction)pendown_method, METH_NOARGS,
        "Pull the pen down – drawing when moving."},
    {"down", (PyCFunction)pendown_method, METH_NOARGS,
        "Pull the pen down – drawing when moving."},
    {"penup", (PyCFunction)penup_method, METH_NOARGS,
        "Pull the pen up – no drawing when moving."},
    {"pu", (PyCFunction)penup_method, METH_NOARGS,
        "Pull the pen up – no drawing when moving."},
    {"up", (PyCFunction)penup_method, METH_NOARGS,
        "Pull the pen up – no drawing when moving."},
    {"isdown", (PyCFunction)isdown_method, METH_NOARGS,
        "Return True if pen is down, False if it’s up."},
    {"shape", (PyCFunction)shape_method, METH_VARARGS,
        "Set turtle shape to shape with given name or, if name is not given, return name of current shape."},
    {"pencolor", (PyCFunction)pencolor_method, METH_VARARGS,
        "Return or set the pencolor."},
    {"fillcolor", (PyCFunction)fillcolor_method, METH_VARARGS,
        "Return or set the fillcolor."},
    {"color", (PyCFunction)color_method, METH_VARARGS,
        "Return or set pencolor and fillcolor."},
    {"tracer", (PyCFunction)not_implemented_method,  METH_VARARGS| METH_KEYWORDS,
        "Turn turtle animation on/off and set delay for update drawings (not implemented)."},
    {"update", (PyCFunction)not_implemented_method,  METH_VARARGS| METH_KEYWORDS,
        "Perform a TurtleScreen update. To be used when tracer is turned off (not implemented)."},
    {"setundobuffer", (PyCFunction)not_implemented_method,  METH_VARARGS| METH_KEYWORDS,
        "Set or disable undobuffer (not implemented)."},
    {"speed", (PyCFunction)not_implemented_method,  METH_VARARGS| METH_KEYWORDS,
        "Set the turtle’s speed to an integer value in the range 0..10. If no argument is given, return current speed (not implemented)."},
    {"goto", (PyCFunction)goto_method,  METH_VARARGS,
        "Move turtle to an absolute position. If the pen is down, draw line. Do not change the turtle’s orientation."},
    {"setpos", (PyCFunction)goto_method,  METH_VARARGS,
        "Move turtle to an absolute position. If the pen is down, draw line. Do not change the turtle’s orientation."},
    {"setposition", (PyCFunction)goto_method,  METH_VARARGS,
        "Move turtle to an absolute position. If the pen is down, draw line. Do not change the turtle’s orientation."},
    {"forward", (PyCFunction)forward_method,  METH_VARARGS,
        "Move the turtle forward by the specified distance, in the direction the turtle is headed."},
    {"fd", (PyCFunction)forward_method,  METH_VARARGS,
        "Move the turtle forward by the specified distance, in the direction the turtle is headed."},
    {"backward", (PyCFunction)backward_method,  METH_VARARGS,
        "Move the turtle backward by the specified distance, in the direction the turtle is headed."},
    {"back", (PyCFunction)backward_method,  METH_VARARGS,
        "Move the turtle backward by the specified distance, in the direction the turtle is headed."},
    {"bk", (PyCFunction)backward_method,  METH_VARARGS,
        "Move the turtle backward by the specified distance, in the direction the turtle is headed."},
    {"stamp", (PyCFunction)stamp_method,  METH_VARARGS,
        "Stamp a copy of the turtle shape onto the canvas at the current turtle position."},
    {"left", (PyCFunction)left_method,  METH_VARARGS,
        "Turn turtle left by angle units."},
    {"lt", (PyCFunction)left_method,  METH_VARARGS,
        "Turn turtle left by angle units."},
    {"right", (PyCFunction)right_method,  METH_VARARGS,
        "Turn turtle right by angle units."},
    {"rt", (PyCFunction)right_method,  METH_VARARGS,
        "Turn turtle right by angle units."},
    {"setheading", (PyCFunction)setheading_method,  METH_VARARGS,
        "Set the orientation of the turtle to to_angle."},
    {"seth", (PyCFunction)setheading_method,  METH_VARARGS,
        "Set the orientation of the turtle to to_angle."},
    {"heading", (PyCFunction)heading_method,  METH_NOARGS,
        "Return the turtle’s current heading."},
    {"reset", (PyCFunction)reset_method,  METH_NOARGS,
        "Delete the turtle’s drawings from the screen, re-center the turtle and set variables to the default values."},
    {"width", (PyCFunction)width_method,  METH_VARARGS,
        "Set the line thickness to width or return it."},
    {"pensize", (PyCFunction)width_method,  METH_VARARGS,
        "Set the line thickness to width or return it."},
    {"begin_fill", (PyCFunction)begin_fill_method,  METH_NOARGS,
        "To be called just before drawing a shape to be filled."},
    {"end_fill", (PyCFunction)end_fill_method,  METH_NOARGS,
        "Fill the shape drawn after the last call to begin_fill()."},
    {"circle", (PyCFunction)circle_method,  METH_VARARGS | METH_KEYWORDS,
        "Draw a circle with given radius."},
    {"home", (PyCFunction)home_method,  METH_NOARGS,
        "Move turtle to the origin – coordinates (0,0) – and set its heading to its start-orientation."},
    {"dot", (PyCFunction)dot_method,  METH_VARARGS,
        "Draw a circular dot with diameter size."},

    {NULL, NULL, 0, NULL}        /* Sentinel */
};



PyTypeObject TurtleType =
{
    PyVarObject_HEAD_INIT(0, 0)
    "qturtle.Type",     /* tp_name */
    sizeof(TurtlePyObject),       /* tp_basicsize */
    0,                    /* tp_itemsize */
    (destructor)turtle_dealloc,  /* tp_dealloc */
    0,                    /* tp_print */
    0,                    /* tp_getattr */
    0,                    /* tp_setattr */
    0,                    /* tp_reserved */
    0,                    /* tp_repr */
    0,                    /* tp_as_number */
    0,                    /* tp_as_sequence */
    0,                    /* tp_as_mapping */
    0,                    /* tp_hash  */
    0,                    /* tp_call */
    0,                    /* tp_str */
    0,                    /* tp_getattro */
    0,                    /* tp_setattro */
    0,                    /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "Turtle object",      /* tp_doc */
    0,                    /* tp_traverse */
    0,                    /* tp_clear */
    0,                    /* tp_richcompare */
    0,                    /* tp_weaklistoffset */
    0,                    /* tp_iter */
    0,                    /* tp_iternext */
    Turtle_methods,       /* tp_methods */
    0,                    /* tp_members */
    0,                    /* tp_getset */
    0,                    /* tp_base */
    0,                    /* tp_dict */
    0,                    /* tp_descr_get */
    0,                    /* tp_descr_set */
    0,                    /* tp_dictoffset */
    0,                    /* tp_init */
    0,                    /* tp_alloc */
    turtle_new,                    /* tp_new */
};

void registerTurtleType(PyObject* module)
{
    PyType_Ready(&TurtleType);
    Py_INCREF(&TurtleType);

    PyModule_AddObject(module, "Turtle", (PyObject *)&TurtleType);
}


} // namespace TurtleModule
