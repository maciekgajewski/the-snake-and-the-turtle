#include "TurtleScreen.hpp"

#include <QApplication>
#include <QDebug>

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

    QApplication::processEvents();
}

void TurtleScreen::mode(TurtleScreen::Mode m)
{
    QMutexLocker l(&_mutex);
    if (m != _mode)
    {
        _mode = m;
        reset();
    }
}

void TurtleScreen::setworldcoordinates(const QRectF& coords)
{
    mode(MODE_WORLD);
    _worldCoords = coords;
    fitInView(coords);

    QApplication::processEvents();
}

void TurtleScreen::reset()
{
    // TODO
}

void TurtleScreen::setup(const QRect &geometry)
{
    qDebug() << "TurtleScreen: setup";
    // does nothing in this class
}

void TurtleScreen::mainloop()
{
    qDebug() << "TurtleScreen: mainloop";
    // does nothing in this class
}

void TurtleScreen::bye()
{
    qDebug() << "TurtleScreen: bye";
    // does nothing in this class
}

void TurtleScreen::exitonclick()
{
    qDebug() << "TurtleScreen: exitonclick";
    // does nothing in this class
}

} // namespace TurtleModule
