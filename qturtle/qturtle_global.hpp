#ifndef QTURTLE_GLOBAL_HPP
#define QTURTLE_GLOBAL_HPP

#include <QtCore/qglobal.h>

#include <Python.h>

#if defined(QTURTLE_LIBRARY)
#  define QTURTLESHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTURTLESHARED_EXPORT Q_DECL_IMPORT
#endif

extern "C"
{

//PyMODINIT_FUNC initqturtle();
PyMODINIT_FUNC PyInit_qturtle();

}

#endif // QTURTLE_GLOBAL_HPP
