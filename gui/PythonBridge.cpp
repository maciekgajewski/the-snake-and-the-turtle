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
#include "Emb.hpp"

#include "turtleModuleLib/GlobalFunctions.hpp"

#include <QDebug>
#include <QtConcurrentRun>
#include <QTimer>
#include <QTcpServer>
#include <QDateTime>

#include <Python.h>


#include <functional>

namespace Turtle {

PythonBridge* PythonBridge::_instance = nullptr;
PyObject* PythonBridge::_exception = nullptr;
bool PythonBridge::_stop = false;

PythonBridge::PythonBridge(QObject *parent)
    : QObject(parent)
{
    Q_ASSERT(!_instance);
    _instance = this;

    PyImport_AppendInittab("qturtle", &PythonBridge::initModule);
    PyImport_AppendInittab("emb", emb::PyInit_emb);
    Py_Initialize();
    PyEval_SetTrace(&PythonBridge::trace, nullptr);

    // create new exception type
    _exception = PyErr_NewException("__main__.ScriptBreakException", NULL, NULL);

    connect(&_scriptResult, SIGNAL(finished()), SLOT(scriptCompleted()));

    // redirect stdout
    PyImport_ImportModule("emb");
    emb::set_stdout([this](const std::string& s){onPrint(s);});
}

PythonBridge::~PythonBridge()
{
    Py_DECREF(_exception);
    Py_Finalize();
}

void PythonBridge::executeScript(const QString& script)
{
    if (!_scriptResult.isRunning())
    {
        _stop = false;
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

void PythonBridge::reset()
{
    TurtleModule::resetModule();
}

void PythonBridge::stop()
{
    {
        QMutexLocker l(&_mutex);
        _stop = true;
    }
    _wait.wakeAll();
}

void PythonBridge::scriptCompleted()
{
    PyObject* res = _scriptResult.result();
    if (!res)
    {
        qDebug("scrpt finished with exception");
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

        // ignore 'our' exception
        if (type != _exception)
        {
            Q_EMIT scriptError(line, errorText);
        }
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

void PythonBridge::onPrintInMain(const QString& s)
{
    Q_EMIT stdoutData(s);
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
    {
        QMutexLocker l(&_instance->_mutex);
        if (_stop)
        {
            //PyErr_SetInterrupt();
            PyErr_SetString(_exception, "Script interrupted");
            _stop = false;
            return -1;
        }
    }
    if (what == PyTrace_LINE)
    {
        int lineNum = PyFrame_GetLineNumber(frame);
        QMetaObject::invokeMethod(_instance, "lineExecuted", Qt::QueuedConnection, Q_ARG(int, lineNum));

        // wait for the green light
        _instance->_mutex.lock();
        _instance->_wait.wait(&_instance->_mutex);
        _instance->_mutex.unlock();
    }

    return 0;
}

PyObject* PythonBridge::runScript(QString code)
{
    PyObject* mainModule = PyImport_AddModule("__main__");
    PyObject* globals = PyModule_GetDict(mainModule);
    PyObject* res = PyRun_String(code.toAscii(), Py_file_input, globals, globals);

    return res;
}

void PythonBridge::onPrint(const std::string &data)
{
    QMetaObject::invokeMethod(
        this, "onPrintInMain", Qt::QueuedConnection, Q_ARG(QString, QString::fromStdString(data)));
}


} // namespace Turtle
