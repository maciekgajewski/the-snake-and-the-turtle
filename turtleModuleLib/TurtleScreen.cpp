#include "TurtleScreen.hpp"

namespace TurtleModule {

TurtleScreen::TurtleScreen(QWidget *parent) :
    QGraphicsView( parent),

    _bgcolor(Qt::white),
    _mode(MODE_STANDARD)
{
    setScene(&_scene);
    _scene.setBackgroundBrush(_bgcolor);
}

QColor TurtleScreen::bgcolor()
{
    QMutexLocker l(&_mutex);
    return _bgcolor;
}

TurtleScreen::Mode TurtleScreen::mode()
{
    QMutexLocker l(&_mutex);
    return _mode;
}

void TurtleScreen::bgcolor(const QColor &color)
{
    QMutexLocker l(&_mutex);
    _bgcolor = color;
    _scene.setBackgroundBrush(color);
}

void TurtleScreen::mode(TurtleScreen::Mode m)
{
    QMutexLocker l(&_mutex);
    if (m != _mode)
    {
        // TODO reset etc
        _mode = m;
    }
}

void TurtleScreen::setup(const QRect &geometry)
{
    // does nothing in this class
}

} // namespace TurtleModule
