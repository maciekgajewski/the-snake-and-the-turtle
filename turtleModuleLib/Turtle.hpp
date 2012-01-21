#ifndef TURTLEMODULE_TURTLE_HPP
#define TURTLEMODULE_TURTLE_HPP

#include <QObject>

namespace TurtleModule
{

/// The turtle class implementation
class Turtle : public QObject
{
    Q_OBJECT
public:
    Turtle(QObject* parent = NULL);

};


} // namespace TurtleModule
#endif // TURTLEMODULE_TURTLE_HPP
