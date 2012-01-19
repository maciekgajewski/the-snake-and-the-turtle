#include "Screen.hpp"

#include <QDebug>
#include <QThread>

namespace TurtleModule {

Screen::Screen() :
    TurtleScreen()
{
    qDebug() << "Screen created in thread" << QThread::currentThreadId();
}

void Screen::setup(const QRect &geometry)
{
    qDebug() << "Screen: setup" << geometry << "in thread" << QThread::currentThreadId();

    move(geometry.topLeft());
    resize(geometry.size());
}

} // namespace TurtleModule
