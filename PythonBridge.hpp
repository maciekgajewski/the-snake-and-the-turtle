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
#ifndef TURTLE_PYTHONBRIDGE_HPP
#define TURTLE_PYTHONBRIDGE_HPP

#include <QObject>
#include <QFutureWatcher>
#include <QWaitCondition>

#include <Python.h>
#include <frameobject.h>

namespace Turtle {

class Turtle;

/// Bridge between python and an instance of turtle
class PythonBridge : public QObject
{
    Q_OBJECT
public:
    PythonBridge(Turtle* turtle, QObject* parent = nullptr);

public Q_SLOTS:

    void executeScript(const QString& script);
    void step();

Q_SIGNALS:

    void currentLineChanged(int);
    void scriptEnded();
    void scriptError(int line, const QString& text);

private Q_SLOTS:

    void scriptCompleted();
    void lineExecuted(int line);

private:

    static Turtle* _turtle; // TODO don't !!!!

    // truelre implementation
    static PyObject* turtle_forward(PyObject *self, PyObject *args);
    static PyObject* turtle_backward(PyObject *self, PyObject *args);
    static PyObject* turtle_left(PyObject *self, PyObject *args);
    static PyObject* turtle_right(PyObject *self, PyObject *args);
    static PyObject* turtle_color(PyObject *self, PyObject *args);

    // utils

    static PyObject* initModule();
    static QString repr(PyObject* o);
    static int trace(PyObject *obj, PyFrameObject *frame, int what, PyObject *arg);

    /// Runs script. Executed in separate thread
    PyObject* runScript(QString code);

    QFutureWatcher<PyObject*> _scriptResult;

    QMutex _mutex;
    QWaitCondition _wait;
};

} // namespace Turtle

#endif // TURTLE_PYTHONBRIDGE_HPP
