#include "ScreenFunctions.hpp"
#include "TurtleFunctions.hpp"

#include "Module.hpp"
#include "TurtleScreen.hpp"
#include "NamedColors.hpp"

#include <QApplication>

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

static PyObject* not_implemented_global(PyObject* /*self*/, PyObject* /*args*/)
{
    Py_RETURN_NONE;
}

static PyMethodDef qturtleMethods[] = {
    {"setup",  (PyCFunction)TurtleModule::setup_global, METH_VARARGS| METH_KEYWORDS,
         "Set the size and position of the main window."},
    {"bgcolor",  TurtleModule::bgcolor_global, METH_VARARGS,
         "Set or return background color of the TurtleScreen."},
    {"mode", (PyCFunction)TurtleModule::mode_global,  METH_VARARGS| METH_KEYWORDS,
        "Set turtle mode ('standard', 'logo' or 'world') and perform reset. If mode is not given, current mode is returned."},
    {"setworldcoordinates", TurtleModule::setworldcoordinates_global,  METH_VARARGS,
        "Set up user-defined coordinate system and switch to mode 'world' if necessary."},
    {"mainloop", mainloop_global, METH_NOARGS,
        "Starts event loop - calling Qt's mainloop function. Must be the last statement in a turtle graphics program."},
    {"bye", bye_global,  METH_NOARGS,
        "Shut turtlegraphics window."},
    {"exitonclick", exitonclick_global,  METH_NOARGS,
        "Bind window bye() to window click event."},
    {"resetscreen", resetscreen_global,  METH_NOARGS,
        "Reset all Turtles on the Screen to their initial state."},
    {"colormode", not_implemented_global,  METH_VARARGS| METH_KEYWORDS,
        "Return the colormode or set it to 1.0 or 255 (not implemented)."},

    {"write", not_implemented_global,  METH_VARARGS| METH_KEYWORDS,
        "Wrtie text (not implemented)."},


    {"hideturtle", hideturtle_global, METH_NOARGS,
        "Make the turtle invisible."},
    {"ht", hideturtle_global, METH_NOARGS,
        "Make the turtle invisible."},
    {"showturtle", showturtle_global, METH_NOARGS,
        "Make the turtle visible."},
    {"st", showturtle_global, METH_NOARGS,
        "Make the turtle visible."},
    {"isvisible", isvisible_global, METH_NOARGS,
        "Return True if the Turtle is shown, False if it’s hidden."},
    {"shapesize", shapesize_global, METH_VARARGS,
        "Return or set the pen’s attributes x/y-stretchfactors and/or outline."},
    {"turtlesize", shapesize_global, METH_VARARGS,
        "Return or set the pen’s attributes x/y-stretchfactors and/or outline."},
    {"pendown", pendown_global, METH_NOARGS,
        "Pull the pen down – drawing when moving."},
    {"pd", pendown_global, METH_NOARGS,
        "Pull the pen down – drawing when moving."},
    {"down", pendown_global, METH_NOARGS,
        "Pull the pen down – drawing when moving."},
    {"penup", penup_global, METH_NOARGS,
        "Pull the pen up – no drawing when moving."},
    {"pu", penup_global, METH_NOARGS,
        "Pull the pen up – no drawing when moving."},
    {"up", penup_global, METH_NOARGS,
        "Pull the pen up – no drawing when moving."},
    {"isdown", isdown_global, METH_NOARGS,
        "Return True if pen is down, False if it’s up."},
    {"shape", shape_global, METH_VARARGS,
        "Set turtle shape to shape with given name or, if name is not given, return name of current shape."},
    {"pencolor", pencolor_global, METH_VARARGS,
        "Return or set the pencolor."},
    {"fillcolor", fillcolor_global, METH_VARARGS,
        "Return or set the fillcolor."},
    {"color", color_global, METH_VARARGS,
        "Return or set pencolor and fillcolor."},
    {"tracer", not_implemented_global,  METH_VARARGS| METH_KEYWORDS,
        "Turn turtle animation on/off and set delay for update drawings (not implemented)."},
    {"update", not_implemented_global,  METH_VARARGS| METH_KEYWORDS,
        "Perform a TurtleScreen update. To be used when tracer is turned off (not implemented)."},
    {"setundobuffer", not_implemented_global,  METH_VARARGS| METH_KEYWORDS,
        "Set or disable undobuffer (not implemented)."},
    {"speed", not_implemented_global,  METH_VARARGS| METH_KEYWORDS,
        "Set the turtle’s speed to an integer value in the range 0..10. If no argument is given, return current speed (not implemented)."},
    {"goto", goto_global,  METH_VARARGS,
        "Move turtle to an absolute position. If the pen is down, draw line. Do not change the turtle’s orientation."},
    {"setpos", goto_global,  METH_VARARGS,
        "Move turtle to an absolute position. If the pen is down, draw line. Do not change the turtle’s orientation."},
    {"setposition", goto_global,  METH_VARARGS,
        "Move turtle to an absolute position. If the pen is down, draw line. Do not change the turtle’s orientation."},
    {"forward", forward_global,  METH_VARARGS,
        "Move the turtle forward by the specified distance, in the direction the turtle is headed."},
    {"fd", forward_global,  METH_VARARGS,
        "Move the turtle forward by the specified distance, in the direction the turtle is headed."},
    {"backward", backward_global,  METH_VARARGS,
        "Move the turtle backward by the specified distance, in the direction the turtle is headed."},
    {"back", backward_global,  METH_VARARGS,
        "Move the turtle backward by the specified distance, in the direction the turtle is headed."},
    {"bk", backward_global,  METH_VARARGS,
        "Move the turtle backward by the specified distance, in the direction the turtle is headed."},
    {"stamp", stamp_global,  METH_VARARGS,
        "Stamp a copy of the turtle shape onto the canvas at the current turtle position."},
    {"left", left_global,  METH_VARARGS,
        "Turn turtle left by angle units."},
    {"lt", left_global,  METH_VARARGS,
        "Turn turtle left by angle units."},
    {"right", right_global,  METH_VARARGS,
        "Turn turtle right by angle units."},
    {"rt", right_global,  METH_VARARGS,
        "Turn turtle right by angle units."},
    {"setheading", setheading_global,  METH_VARARGS,
        "Set the orientation of the turtle to to_angle."},
    {"seth", setheading_global,  METH_VARARGS,
        "Set the orientation of the turtle to to_angle."},
    {"heading", heading_global,  METH_NOARGS,
        "Return the turtle’s current heading."},
    {"reset", reset_global,  METH_NOARGS,
        "Delete the turtle’s drawings from the screen, re-center the turtle and set variables to the default values."},
    {"width", width_global,  METH_VARARGS,
        "Set the line thickness to width or return it."},
    {"pensize", width_global,  METH_VARARGS,
        "Set the line thickness to width or return it."},
    {"begin_fill", begin_fill_global,  METH_NOARGS,
        "To be called just before drawing a shape to be filled."},
    {"end_fill", end_fill_global,  METH_NOARGS,
        "Fill the shape drawn after the last call to begin_fill()."},
    {"circle", (PyCFunction)circle_global,  METH_VARARGS | METH_KEYWORDS,
        "Draw a circle with given radius."},
    {"home", home_global,  METH_NOARGS,
        "Move turtle to the origin – coordinates (0,0) – and set its heading to its start-orientation."},
    {"dot", dot_global,  METH_VARARGS,
        "Draw a circular dot with diameter size."},

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

PyObject* creteModule()
{
    PyObject *m;

    m = PyModule_Create(&qturtleModule);
    if (m == NULL)
        return NULL;

    registerTurtleType(m);

    return m;
}

QColor objectToColor(PyObject* o)
{
    if (!o)
        return QColor();
    else if (PyUnicode_Check(o))
    {
        Py_UNICODE* data = PyUnicodeUCS2_AsUnicode(o);
        QString s = QString::fromWCharArray(data);
        return nameToColor(s);
    }
    else if (PyTuple_Check(o))
    {
        double r, g, b;

        if (PyTuple_Size(o) < 3)
            return QColor();

        r = PyFloat_AsDouble(PyTuple_GET_ITEM(o, 0));
        g = PyFloat_AsDouble(PyTuple_GET_ITEM(o, 1));
        b = PyFloat_AsDouble(PyTuple_GET_ITEM(o, 2));

        if (r < 1.0 && g < 1.0 && b < 1.0)
            return QColor::fromRgbF(r, g, b);
        else
            return QColor((int)r, (int)g, (int)b);
    }

    return QColor();
}

PyObject* colorToTuple(const QColor& color)
{
    return Py_BuildValue("iii", color.red(), color.green(), color.blue());
}

void resetModule()
{
    Module::instance()->reset();
}

static PyObject* invokeStatusToPyError(bool status)
{
    if (status)
    {
        Py_RETURN_NONE;
    }
    else
    {
        PyErr_SetString(PyExc_RuntimeError, "Failed to invoke method");
        return NULL;
    }
}

PyObject* invoke1(QObject* obj, const char* method, QGenericArgument param)
{
    bool r = QMetaObject::invokeMethod(obj, method, Qt::QueuedConnection, param);
    QApplication::processEvents();
    return invokeStatusToPyError(r);
}

// invokes and blocks until the method completes
PyObject* invoke(
        QObject* obj,
        const char* method,
        QGenericArgument p1,
        QGenericArgument p2,
        QGenericArgument p3,
        QGenericArgument p4)
{
    // call directly in single-thread mode
    Qt::ConnectionType ct = Module::instance()->isEmbedded() ?
        Qt::BlockingQueuedConnection : Qt::DirectConnection;
    bool r = QMetaObject::invokeMethod(obj, method, ct, p1, p2, p3, p4);
    QApplication::processEvents();
    return invokeStatusToPyError(r);
}

}
