#include "qturtle_global.hpp"
#include "globalFunctions.hpp"

extern "C"
{


static PyMethodDef qturtleMethods[] = {
    {"setup",  TurtleModule::setup, METH_VARARGS,
     "Set the size and position of the main window."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef qturtleModule = {
   PyModuleDef_HEAD_INIT,
   "qturtle",   /* name of module */
   NULL, /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   qturtleMethods
};

PyMODINIT_FUNC
PyInit_qturtle()
{
    PyObject *m;

    m = PyModule_Create(&qturtleModule);
    if (m == NULL)
        return NULL;

    return m;
}

/*
PyMODINIT_FUNC initqturtle()
{
    return PyInit_qturtle();
}
*/


}
