#ifndef TURTLEMODULE_SCREENFUNCTIONS_HPP
#define TURTLEMODULE_SCREENFUNCTIONS_HPP

#include <Python.h>

namespace TurtleModule {

PyObject* setup_global(PyObject *self, PyObject *args, PyObject *keywd);
PyObject* bgcolor_global(PyObject *self, PyObject *args);
PyObject* mode_global(PyObject */*self*/, PyObject *args, PyObject* keywords);

} // namespace TurtleModule

#endif // TURTLEMODULE_SCREENFUNCTIONS_HPP
