#include "ScreenFunctions.hpp"

#include "GlobalFunctions.hpp"
#include "TurtleScreen.hpp"

#include <QDesktopWidget>
#include <QApplication>
#include <Qdebug>

#include <limits>

namespace TurtleModule {

static PyObject* setup(TurtleScreen* s, PyObject *args, PyObject *keywds)
{
    double width = 0.5;
    double height = 0.75;
    int startx = std::numeric_limits<int>::max();
    int starty = std::numeric_limits<int>::max();

    static char *kwlist[] = {"width", "height", "startx", "starty", NULL};

    if (s)
    {
        if (PyArg_ParseTupleAndKeywords(args, keywds, "ff|ii", kwlist, &width, &height, &startx, &starty))
        {
            // foolproof
            if (width < 0 || height < 0)
            {
                PyErr_SetString(PyExc_AttributeError, "width and height can not be negative");
                return NULL;
            }

            QRect sg = QApplication::desktop()->screenGeometry();

            // handle floating point args
            if (width < 1.0) width *= sg.width();
            if (height < 1.0) height *= sg.height();

            // handle unset/negative pos
            if (startx == std::numeric_limits<int>::max())
                startx = sg.left() + (sg.width()-width);
            else if (startx < 0)
                startx = sg.right() - width + startx;

            if (starty == std::numeric_limits<int>::max())
                starty = sg.top() + (sg.height()-height);
            else if (startx < 0)
                starty = sg.bottom() - height + starty;

            QRect g(startx, starty, width, height);
            qDebug() << "invoking setup" << g;
            QMetaObject::invokeMethod(s, "setup", Qt::QueuedConnection, Q_ARG(QRect, g));
            Py_RETURN_NONE;
        }
    }

    return NULL;
}

PyObject* setup_global(PyObject */*self*/, PyObject *args, PyObject* keywords)
{
    TurtleScreen* s = getScreen();
    setup(s, args, keywords);
}

static PyObject* bgcolor(TurtleScreen* s, PyObject *args)
{
    char* colorName = nullptr;

    int r,g,b;

    if (s)
    {
        if (PySequence_Length(args) == 0)
        {
            qDebug() << "Parameterless bgcolor called";
            return colorToTuple(s->bgcolor());
        }
        else
        {
            QColor color = argsToColor(args);
            if (color.isValid())
            {
                qDebug() << "invoking bgcolor" << color;
                QMetaObject::invokeMethod(s, "bgcolor", Qt::QueuedConnection, Q_ARG(QColor, color));
                Py_RETURN_NONE;
            }
            else
            {
                qDebug() << "bgcolor called with invalid color";
                return NULL;
            }
        }
    }

    return NULL;
}

PyObject* bgcolor_global(PyObject */*self*/, PyObject *args)
{
    TurtleScreen* s = getScreen();
    bgcolor(s, args);
}

static PyObject* mode(TurtleScreen* s, PyObject *args, PyObject *keywds)
{
    const char* modeName = nullptr;
    static char *kwlist[] = {"mode", NULL};

    if (s)
    {
        if (PyArg_ParseTupleAndKeywords(args, keywds, "|s", kwlist, &modeName))
        {
            if (modeName == NULL)
            {
                TurtleScreen::Mode m = s->mode();
                switch(m)
                {
                case TurtleScreen::MODE_LOGO:
                    return PyUnicodeUCS2_FromString("logo");
                case TurtleScreen::MODE_STANDARD:
                    return PyUnicodeUCS2_FromString("standard");
                case TurtleScreen::MODE_WORLD:
                    return PyUnicodeUCS2_FromString("world");
                default:
                    PyErr_SetString(PyExc_RuntimeError, "Screen in unknown mode");
                    return NULL;
                }
            }
            else
            {
                TurtleScreen::Mode newMode = TurtleScreen::MODE_STANDARD;

                if (QString(modeName) == "logo") newMode = TurtleScreen::MODE_LOGO;
                else if (QString(modeName) == "standard") newMode = TurtleScreen::MODE_STANDARD;
                else if (QString(modeName) == "world") newMode = TurtleScreen::MODE_WORLD;
                else
                {
                    PyErr_SetString(PyExc_RuntimeError, "Invalid mode name");
                    return NULL;
                }

                qDebug() << "Setting mode to " << newMode;
                QMetaObject::invokeMethod(s, "mode", Qt::QueuedConnection, Q_ARG(TurtleScreen::Mode, newMode));
                Py_RETURN_NONE;
            }
        }
    }

    return NULL;
}

PyObject* mode_global(PyObject */*self*/, PyObject *args, PyObject* keywords)
{
    TurtleScreen* s = getScreen();
    mode(s, args, keywords);
}

} // namespace TurtleModule
