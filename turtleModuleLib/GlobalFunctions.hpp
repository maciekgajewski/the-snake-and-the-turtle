#ifndef TURTLEMODULE_GLOBALFUNCTIONS_HPP
#define TURTLEMODULE_GLOBALFUNCTIONS_HPP

#include <Python.h>

namespace TurtleModule
{

class TurtleScreen;

// utilities

TurtleScreen* getScreen(); // returns/creates anonymous turtle screen, or sets python exception and returns null

PyObject* creteModule();



}

#endif // TURTLEMODULE_GLOBALFUNCTIONS_HPP
