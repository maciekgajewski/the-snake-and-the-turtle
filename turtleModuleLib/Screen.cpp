#include "Screen.hpp"

namespace TurtleModule {

Screen::Screen() :
    TurtleScreen()
{
}

void Screen::setup(const QRect &geometry)
{
    move(geometry.topLeft());
    resize(geometry.size());
}

} // namespace TurtleModule
