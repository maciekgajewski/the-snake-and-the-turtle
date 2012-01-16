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
#include "PythonBridge.hpp"

#include "Turtle.hpp"

#include <QDebug>
#include <QtConcurrentRun>
#include <QTimer>

#include <Python.h>


#include <functional>

namespace Turtle {

Turtle* PythonBridge::_turtle = nullptr;
static PythonBridge* globalThis = nullptr; // stupid..

PythonBridge::PythonBridge(Turtle *turtle, QObject *parent)
    : QObject(parent)
{
    // TODO don't use global turtle!!!
   _turtle = turtle;
    globalThis = this;

    PyImport_AppendInittab("turtle", &PythonBridge::initModule);
    PyEval_SetTrace(&PythonBridge::trace, nullptr);
    connect(&_scriptResult, SIGNAL(finished()), SLOT(scriptCompleted()));
}

void PythonBridge::executeScript(const QString& script)
{
    if (!_scriptResult.isRunning())
    {
        QFuture<PyObject*> future = QtConcurrent::run(std::bind(&PythonBridge::runScript, this, script));
        _scriptResult.setFuture(future);
    }
    else
    {
        qWarning("script executed while already running");
    }
}

void PythonBridge::step()
{
    _wait.wakeAll();
}

void PythonBridge::scriptCompleted()
{
    PyObject* res = _scriptResult.result();
    if (!res)
    {
        qDebug("error running script");
        PyObject* type = nullptr;
        PyObject* value = nullptr;
        PyObject* traceback = nullptr;

        PyErr_Fetch(&type, &value, &traceback);

        qDebug() << repr(type) << repr(value) << repr(traceback);
        QString errorText = repr(value);
        int line = 0;
        if (traceback && PyTraceBack_Check(traceback))
        {
            PyTracebackObject* t = reinterpret_cast<PyTracebackObject*>(traceback);
            line = t->tb_lineno;
        }

        Q_EMIT scriptError(line, errorText);
    }
    else
    {
        qDebug("script executed OK");
    }

    Q_EMIT scriptEnded();
}

void PythonBridge::lineExecuted(int line)
{
    Q_EMIT currentLineChanged(line);
}


PyObject* PythonBridge::initModule()
{
    static PyMethodDef turtleMethods[] = {
        {"forward", &PythonBridge::turtle_forward, METH_VARARGS,
             "Move the turtle forward by the specified distance, in the direction the turtle is headed."},
        {"fw", &PythonBridge::turtle_forward, METH_VARARGS,
             "Move the turtle forward by the specified distance, in the direction the turtle is headed."},

        {"backward", &PythonBridge::turtle_backward, METH_VARARGS,
            "Move the turtle backward by the specified distance, in the direction the turtle is headed."},
        {"back", &PythonBridge::turtle_backward, METH_VARARGS,
            "Move the turtle backward by the specified distance, in the direction the turtle is headed."},
        {"bk", &PythonBridge::turtle_backward, METH_VARARGS,
            "Move the turtle backward by the specified distance, in the direction the turtle is headed."},

        {"left", &PythonBridge::turtle_left, METH_VARARGS,
            "Turn turtle left by specified degrees."},
        {"lt", &PythonBridge::turtle_left, METH_VARARGS,
            "Turn turtle left by specified degrees."},

        {"right", &PythonBridge::turtle_right, METH_VARARGS,
            "Turn turtle right by specified degrees."},
        {"rt", &PythonBridge::turtle_right, METH_VARARGS,
            "Turn turtle right by specified degrees."},

        {"color", &PythonBridge::turtle_color, METH_VARARGS,
            "Set turtle pen color."},
        {NULL, NULL, 0, NULL}
    };

    static PyModuleDef turtleModule = {
        PyModuleDef_HEAD_INIT, "turtle", NULL, -1, turtleMethods,
        NULL, NULL, NULL, NULL
    };

    return PyModule_Create(&turtleModule);
}

QString PythonBridge::repr(PyObject *o)
{
    if (o)
    {
        PyObject* r = PyObject_Repr(o);
        Py_UNICODE* data = PyUnicodeUCS2_AsUnicode(r);
        QString s = QString::fromWCharArray(data);
        Py_DECREF(r);

        return s;
    }
    else
    {
        return "NULL";
    }

}

int PythonBridge::trace(PyObject *obj, PyFrameObject *frame, int what, PyObject *arg)
{
    if (what == PyTrace_LINE)
    {
        int lineNum = PyFrame_GetLineNumber(frame);
        QMetaObject::invokeMethod(globalThis, "lineExecuted", Qt::QueuedConnection, Q_ARG(int, lineNum));
        globalThis->_mutex.lock();
        globalThis->_wait.wait(&globalThis->_mutex);
        globalThis->_mutex.unlock();
    }

    return 0;
}

PyObject* PythonBridge::runScript(QString code)
{
    PyObject* locals = PyDict_New();
    PyObject* mainModule = PyImport_AddModule("__main__");
    PyObject* globals = PyModule_GetDict(mainModule);
    PyObject* res = PyRun_String(code.toAscii(), Py_file_input, globals, locals);
    Py_DECREF(locals);

    return res;
}

PyObject* PythonBridge::turtle_forward(PyObject *self, PyObject *args)
{
    double steps = 0;
    int ok =  PyArg_ParseTuple(args, "d", &steps);
    if (!ok)
        return NULL;
    QMetaObject::invokeMethod(_turtle, "forward", Qt::QueuedConnection, Q_ARG(double, steps));

    // TODO: pause here and wait for completion

    Py_RETURN_NONE;
}

PyObject *PythonBridge::turtle_backward(PyObject *self, PyObject *args)
{
    double steps = 0;
    int ok =  PyArg_ParseTuple(args, "d", &steps);
    if (!ok)
        return NULL;
    QMetaObject::invokeMethod(_turtle, "forward", Qt::QueuedConnection, Q_ARG(double, -steps));

    // TODO: pause here and wait for completion

    Py_RETURN_NONE;
}

PyObject *PythonBridge::turtle_left(PyObject *self, PyObject *args)
{
    double degrees = 0;
    int ok =  PyArg_ParseTuple(args, "d", &degrees);
    if (!ok)
        return NULL;
    QMetaObject::invokeMethod(_turtle, "left", Qt::QueuedConnection, Q_ARG(double, degrees));

    // TODO: pause here and wait for completion

    Py_RETURN_NONE;
}

PyObject *PythonBridge::turtle_right(PyObject *self, PyObject *args)
{
    double degrees = 0;
    int ok =  PyArg_ParseTuple(args, "d", &degrees);
    if (!ok)
        return NULL;
    QMetaObject::invokeMethod(_turtle, "left", Qt::QueuedConnection, Q_ARG(double, -degrees));

    // TODO: pause here and wait for completion

    Py_RETURN_NONE;
}

PyObject *PythonBridge::turtle_color(PyObject *self, PyObject *args)
{
    QColor fillColor;
    QColor penColor;

    // parse args
    const char* pencolorparam = nullptr;
    const char* fillcolorparam = nullptr;
    if (PyArg_ParseTuple(args, "ss", &pencolorparam, &fillcolorparam))
    {
        penColor.setNamedColor(pencolorparam);
        fillColor.setNamedColor(fillcolorparam);
    }
    else if (PyArg_ParseTuple(args, "s", &pencolorparam))
    {
        PyErr_Clear();
        penColor.setNamedColor(pencolorparam);
        fillColor.setNamedColor(pencolorparam);
    }
    else
    {
        return NULL;
    }

    if (penColor.isValid())
    {
        QMetaObject::invokeMethod(_turtle, "pencolor", Qt::QueuedConnection, Q_ARG(QColor, penColor));
    }
    if (fillColor.isValid())
    {
        QMetaObject::invokeMethod(_turtle, "fillcolor", Qt::QueuedConnection, Q_ARG(QColor, fillColor));
    }

    // TODO: pause here and wait for completion

    Py_RETURN_NONE;
}

} // namespace Turtle
