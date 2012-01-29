#ifndef TURTLEMODULE_GLOBALFUNCTIONS_HPP
#define TURTLEMODULE_GLOBALFUNCTIONS_HPP

#include <QColor>

#include <Python.h>

namespace TurtleModule
{

class TurtleScreen;

// module management

TurtleScreen* getScreen(); // returns/creates anonymous turtle screen, or sets python exception and returns null
PyObject* creteModule();
void resetModule();

// converters

/// Converts function argument tuple into color. The tuple has to containe either a string or 3 integers
/// In case of error, exception is set and invalid color is returned
QColor argsToColor(PyObject* args);

/// Converts color to RGB tuple
PyObject* colorToTuple(const QColor& color);

// other

// Cross thread call
PyObject* invoke(
    QObject* obj,
    const char* method,
    QGenericArgument p1 = QGenericArgument(),
    QGenericArgument p2 = QGenericArgument(),
    QGenericArgument p3 = QGenericArgument(),
    QGenericArgument p4 = QGenericArgument()
    );

// incokes and blocks until the method completes
PyObject* invokeWait(
    QObject* obj,
    const char* name,
    QGenericArgument p1 = QGenericArgument(),
    QGenericArgument p2 = QGenericArgument(),
    QGenericArgument p3 = QGenericArgument(),
    QGenericArgument p4 = QGenericArgument()
    );

}

#endif // TURTLEMODULE_GLOBALFUNCTIONS_HPP
