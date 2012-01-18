#ifndef TURTLEMODULE_TURTLESCREEN_HPP
#define TURTLEMODULE_TURTLESCREEN_HPP

#include <QGraphicsView>
#include <QGraphicsScene>

namespace TurtleModule {

/// The TurtleScreen implementation.
/// By turtle specification it is supposed to be a widget representing turtles arena.
class TurtleScreen : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TurtleScreen(QWidget *parent = 0);
    


public Q_SLOTS:

    // window control
    void bgcolor(const QColor& color);

    // (not implemented here) Screen methods
    virtual void setup(const QRect& geometry);

private:

    QGraphicsScene _scene;
};

} // namespace TurtleModule

#endif // TURTLEMODULE_TURTLESCREEN_HPP
