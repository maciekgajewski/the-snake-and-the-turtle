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
    : QObject(), _screen(nullptr), _embedded(false)
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

        // We need to create out own app
        const char* argv[] = {"python", NULL};
        int argc = 1;
        new QApplication(argc, (char**)argv);
    }

}

Module::Module(QWidget* widget)
    : QObject(), _screen(nullptr), _embedded(true)
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
        QMetaObject::invokeMethod(_screen, "show", Qt::QueuedConnection);
        QApplication::processEvents();
    }

    return _screen;
}

void Module::reset()
{
    if (_screen)
        _screen->mode(TurtleScreen::MODE_STANDARD);
}

} // namespace TurtlrModule
