#include "Module.hpp"

#include "Screen.hpp"

#include <QMutexLocker>
#include <QApplication>
#include <QThread>
#include <QHBoxLayout>
#include <QDebug>

namespace TurtleModule {


QMutex Module::_mutex;
Module* Module::_instance = nullptr;

Module::Module()
    : QObject(), _screen(nullptr)
{
    qDebug() << "Module: created in thread" << QThread::currentThreadId();

    QCoreApplication* app = QCoreApplication::instance();
    if (app)
    {
        qDebug() << "Module: instance created in eviroment with QApplication";
        // ok, we are created in a good'ol Qt app. Just sort out the threads
        moveToThread(app->thread());
    }
    else
    {
        qDebug() << "Module: instance created in eviroment WITHOUT QApplication";

        // TODO the only robust way of doing it is to spawn new process and send commands over IPC.
        // I'll pass for now

        // TODO
        Q_ASSERT(false);
    }

}

Module::Module(QWidget* widget)
    : QObject(), _screen(nullptr)
{
    _screen = new TurtleScreen();
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(_screen);
    layout->setMargin(0);
    widget->setLayout(layout);
}

Module* Module::instance()
{
    QMutexLocker locker(&_mutex);

    if(!_instance)
    {
        qDebug() << "Module: instance does not exist, creating";
        _instance = new Module(); // some weird things may happen now
    }

    return _instance;
}

Module *Module::initialiseEmbedded(QWidget *parent)
{
    Q_ASSERT(!_instance);
    _instance = new Module(parent);
    return _instance;
}

TurtleScreen *Module::screen()
{
    if (!_screen)
    {
        qDebug() << "Module: creating screen";
        _screen = new Screen(); // top-level window
        _screen->moveToThread(thread());
        QMetaObject::invokeMethod(_screen, "show", Qt::QueuedConnection);
    }

    return _screen;
}

bool Module::event(QEvent *e)
{
    if (e->type() == QEvent::ThreadChange)
    {
        qDebug() << "Module: thread changed";
    }
    return QObject::event(e);
}


} // namespace TurtlrModule
