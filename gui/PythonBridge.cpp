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

#include "turtleModuleLib/GlobalFunctions.hpp"

#include <QDebug>
#include <QtConcurrentRun>
#include <QTimer>

#include <Python.h>


#include <functional>

namespace Turtle {

PythonBridge* PythonBridge::_instance = nullptr;

PythonBridge::PythonBridge(QObject *parent)
    : QObject(parent)
{
    _instance = this;

    PyImport_AppendInittab("qturtle", &PythonBridge::initModule);
    PyEval_SetTrace(&PythonBridge::trace, nullptr);
    connect(&_scriptResult, SIGNAL(finished()), SLOT(scriptCompleted()));
}

void PythonBridge::executeScript(const QString& script)
{
    if (!_scriptResult.isRunning())
    {
        TurtleModule::resetModule();
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
    return TurtleModule::creteModule();
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
        QMetaObject::invokeMethod(_instance, "lineExecuted", Qt::QueuedConnection, Q_ARG(int, lineNum));
        _instance->_mutex.lock();
        _instance->_wait.wait(&_instance->_mutex);
        _instance->_mutex.unlock();
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


} // namespace Turtle
