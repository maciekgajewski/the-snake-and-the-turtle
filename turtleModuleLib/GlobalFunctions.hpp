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

// cross-thread call with 1 argument
PyObject* invoke1(QObject* obj, const char* name, QGenericArgument param);
// incokes and blocks until the method completes
PyObject* invoke1wait(QObject* obj, const char* name, QGenericArgument param);
PyObject* invoke0(QObject* s, const char* method);
}

#endif // TURTLEMODULE_GLOBALFUNCTIONS_HPP
