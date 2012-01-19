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

} // namespace TurtleModule
