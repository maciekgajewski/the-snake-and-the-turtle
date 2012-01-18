#include "TurtleScreen.hpp"

namespace TurtleModule {

TurtleScreen::TurtleScreen(QWidget *parent) :
    QGraphicsView(&_scene, parent)
{
}

void TurtleScreen::bgcolor(const QColor &color)
{
    _scene.setBackgroundBrush(color);
}

void TurtleScreen::setup(const QRect &geometry)
{
    // does nothing in this class
}

} // namespace TurtleModule
