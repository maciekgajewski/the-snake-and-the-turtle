#ifndef TURTLEMODULE_GLOBALFUNCTIONS_HPP
#define TURTLEMODULE_GLOBALFUNCTIONS_HPP

#include <Python.h>

extern "C"
{

namespace TurtleModule
{

class TurtleScreen;

// utilities (not exported)

TurtleScreen* getScreen(); // returns/creates anonymous turtle screen, or sets python exception and returns null

// screen-related globals

PyObject* setup(PyObject *self, PyObject *args);

} }

#endif // TURTLEMODULE_GLOBALFUNCTIONS_HPP
