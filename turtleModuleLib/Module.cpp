#include "Module.hpp"

#include "Screen.hpp"

#include <QMutexLocker>
#include <QApplication>
#include <QThread>
#include <QHBoxLayout>

namespace TurtleModule {


QMutex Module::_mutex;
Module* Module::_instance = nullptr;

/// Application thread used when module is run in non-qt environment
class ApplicationThread : public QThread
{
public:
    ApplicationThread(QObject* parent) : QThread(parent) { }

protected:

    virtual void run()
    {
        char appname[] = "python";
        char* argv = appname;
        int argc = 1;

        QApplication app(argc, &argv);
        app.exec();
    }
};

Module::Module()
    : QObject(), _screen(nullptr)
{
    QCoreApplication* app = QCoreApplication::instance();
    if (app)
    {
        // ok, we are created in a good'ol Qt app. Just sort out the threads
        moveToThread(app->thread());
    }
    else
    {
        // ugh... we need to create application
        ApplicationThread* t = new ApplicationThread(this);
        t->start();
        moveToThread(t);
    }

}

Module::Module(QWidget* widget)
    : QObject(), _screen(nullptr)
{
    _screen = new TurtleScreen();
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(_screen);
    widget->setLayout(layout);
}

Module* Module::instance()
{
    qDebug("dupa");
    QMutexLocker locker(&_mutex);

    if(!_instance)
        _instance = new Module(); // some weird things may happen now

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
        _screen = new Screen(); // top-level window
        _screen->moveToThread(thread());
        QMetaObject::invokeMethod(_screen, "show", Qt::QueuedConnection);
    }

    return _screen;
}


} // namespace TurtlrModule
