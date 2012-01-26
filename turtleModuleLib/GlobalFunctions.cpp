#include "ScreenFunctions.hpp"
#include "TurtleFunctions.hpp"

#include "Module.hpp"
#include "TurtleScreen.hpp"

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
        "Turn turtle animation on/off and set delay for update drawings."},
    {"speed", not_implemented_global,  METH_VARARGS| METH_KEYWORDS,
        "Set the turtle’s speed to an integer value in the range 0..10. If no argument is given, return current speed."},
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
    {"backward", forward_global,  METH_VARARGS,
        "Move the turtle backward by the specified distance, in the direction the turtle is headed."},
    {"back", forward_global,  METH_VARARGS,
        "Move the turtle backward by the specified distance, in the direction the turtle is headed."},
    {"bk", forward_global,  METH_VARARGS,
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

    return m;
}

QColor argsToColor(PyObject* args)
{
    // try string format
    char* colorName = nullptr;
    unsigned char r, g, b;
    unsigned char a = 255;

    if (PyArg_ParseTuple(args, "s", &colorName))
    {
        if (QColor::isValidColor(QString::fromUtf8(colorName)))
        {
            return QColor(colorName);
        }
        else
        {
            PyErr_SetString(PyExc_RuntimeError, "invalid color name");
            return QColor();
        }
    }
    else if (PyArg_ParseTuple(args, "(bbb)", &r, &g, &b))
    {
        PyErr_Clear();
        return QColor((int)r, (int)g, (int)b, (int)a);
    }
    else if (PyArg_ParseTuple(args, "(bbbb)", &r, &g, &b, &a))
    {
        PyErr_Clear();
        return QColor((int)r, (int)g, (int)b, (int)a);
    }
    else
    {
        PyErr_Clear();
        PyErr_SetString(PyExc_RuntimeError, "Color has to be specified either as valid color name or 3 or 4 element tuple");
        return QColor();
    }
}

PyObject* colorToTuple(const QColor& color)
{
    return Py_BuildValue("iii", color.red(), color.green(), color.blue());
}

void resetModule()
{
    Module::instance()->reset();
}

PyObject* invoke1(QObject* obj, const char* name, QGenericArgument param)
{
    bool r = QMetaObject::invokeMethod(obj, name, Qt::QueuedConnection, param);
    QApplication::processEvents();
    if (r)
    {
        Py_RETURN_NONE;
    }
    else
    {
        PyErr_SetString(PyExc_RuntimeError, "Failed to invoke method");
        return NULL;
    }
}
// incokes and blocks until the method completes
PyObject* invoke1wait(QObject* obj, const char* name, QGenericArgument param)
{
    Qt::ConnectionType ct = Module::instance()->isEmbedded() ?
        Qt::BlockingQueuedConnection : Qt::DirectConnection;
    bool r = QMetaObject::invokeMethod(obj, name, ct, param);
    QApplication::processEvents();

    if (r)
    {
        Py_RETURN_NONE;
    }
    else
    {
        PyErr_SetString(PyExc_RuntimeError, "Failed to invoke method");
        return NULL;
    }
}

PyObject* invoke0(QObject* s, const char* method)
{
    if (s)
    {
        QMetaObject::invokeMethod(s, method, Qt::QueuedConnection);
        QApplication::processEvents();
        Py_RETURN_NONE;
    }

    return NULL;
}

}
