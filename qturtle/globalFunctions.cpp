#include "globalFunctions.hpp"

#include "Module.hpp"
#include "TurtleScreen.hpp"

extern "C" {

namespace TurtleModule
{

TurtleScreen* getScreen()
{
    try
    {
        return Module::instance()->screen();
    }
    catch(const std::exception& e)
    {
        PyErr_SetString(PyExc_RuntimeError, e.what());
        return nullptr;
    }
}

PyObject* setup(PyObject *self, PyObject *args)
{
    TurtleScreen* s = getScreen();

    int top = 100;
    int left = 100;
    int width = 500;
    int height = 500;

    if (s)
    {
        if (PyArg_ParseTuple(args, "iiii", &width, &height, &left, &top))
        {
            QRect g(left, top, width, height);
            QMetaObject::invokeMethod(s, "setup", Qt::QueuedConnection, Q_ARG(QRect, g));
            Py_RETURN_NONE;
        }
        else if(PyArg_ParseTuple(args, "ii", &width, &height))
        {
            PyErr_Clear();
            QRect g(left, top, width, height);
            QMetaObject::invokeMethod(s, "setup", Qt::QueuedConnection, Q_ARG(QRect, g));
            Py_RETURN_NONE;
        }
    }

    return NULL;
}


} }
