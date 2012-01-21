#include "ScreenFunctions.hpp"
#include "TurtleFunctions.hpp"

#include "Module.hpp"
#include "TurtleScreen.hpp"

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
    {"tracer", not_implemented_global,  METH_VARARGS| METH_KEYWORDS,
        "Turn turtle animation on/off and set delay for update drawings."},
    {"mainloop", mainloop_global,  METH_VARARGS,
        "Starts event loop - calling Qt's mainloop function. Must be the last statement in a turtle graphics program."},
    {"bye", bye_global,  METH_VARARGS,
        "Shut turtlegraphics window."},
    {"exitonclick", exitonclick_global,  METH_VARARGS,
        "Bind window bye() to window click event."},

    {"hideturtle", hideturtle_global, METH_VARARGS,
        "Make the turtle invisible."},
    {"ht", hideturtle_global, METH_VARARGS,
        "Make the turtle invisible."},
    {"showturtle", showturtle_global, METH_VARARGS,
        "Make the turtle visible."},
    {"st", showturtle_global, METH_VARARGS,
        "Make the turtle visible."},
    {"isvisible", isvisible_global, METH_VARARGS,
        "Return True if the Turtle is shown, False if it’s hidden."},
    {"shapesize", shapesize_global, METH_VARARGS,
        "Return or set the pen’s attributes x/y-stretchfactors and/or outline."},
    {"turtlesize", shapesize_global, METH_VARARGS,
        "Return or set the pen’s attributes x/y-stretchfactors and/or outline."},
    {"pendown", pendown_global, METH_VARARGS,
        "Pull the pen down – drawing when moving."},
    {"pd", pendown_global, METH_VARARGS,
        "Pull the pen down – drawing when moving."},
    {"penup", penup_global, METH_VARARGS,
        "Pull the pen up – no drawing when moving."},
    {"pu", penup_global, METH_VARARGS,
        "Pull the pen up – no drawing when moving."},
    {"isdown", isdown_global, METH_VARARGS,
        "Return True if pen is down, False if it’s up."},
    {"shape", shape_global, METH_VARARGS,
        "Set turtle shape to shape with given name or, if name is not given, return name of current shape."},
    {"pencolor", pencolor_global, METH_VARARGS,
        "Return or set the pencolor."},
    {"fillcolor", fillcolor_global, METH_VARARGS,
        "Return or set the fillcolor."},
    {"color", color_global, METH_VARARGS,
        "Return or set pencolor and fillcolor."},

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


}
