#include "Screen.hpp"

#include <QDebug>
#include <QThread>
#include <QApplication>
#include <QMouseEvent>

namespace TurtleModule {

Screen::Screen() :
    TurtleScreen(), _loop(nullptr), _exitOnClick(false)
{
    qDebug() << "Screen created in thread" << QThread::currentThreadId();
}

void Screen::setup(const QRect &geometry)
{
    qDebug() << "Screen: setup" << geometry << "in thread" << QThread::currentThreadId();

    move(geometry.topLeft());
    resize(geometry.size());

    QApplication::processEvents();
}

void Screen::mainloop()
{
    qDebug() << "Screen: mainloop";
    if (!_loop)
    {
        qDebug() << "Screen: entering the event loop";
        _loop = new QEventLoop(this);
        _loop->exec();
    }
}

void Screen::bye()
{
    close();
}

void Screen::exitonclick()
{
    qDebug() << "Screen: exitonclick";

    _exitOnClick = true;
    QApplication::processEvents();
}

void Screen::closeEvent(QCloseEvent *e)
{
    TurtleScreen::closeEvent(e);
    exitLoop();
}

void Screen::mouseReleaseEvent(QMouseEvent *event)
{
    TurtleScreen::mouseReleaseEvent(event);

    if (_exitOnClick)
        close();
}

void Screen::exitLoop()
{
    if (_loop)
    {
        _loop->exit();
        delete _loop;
    }
}

} // namespace TurtleModule
