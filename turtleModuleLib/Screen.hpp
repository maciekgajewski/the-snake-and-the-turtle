#ifndef TURTLEMODULE_SCREEN_HPP
#define TURTLEMODULE_SCREEN_HPP

#include "TurtleScreen.hpp"

namespace TurtleModule {

class Screen : public TurtleScreen
{
    Q_OBJECT
public:
    explicit Screen();
    
public Q_SLOTS:

    virtual void setup(const QRect &geometry);
};

} // namespace TurtleModule

#endif // TURTLEMODULE_SCREEN_HPP
