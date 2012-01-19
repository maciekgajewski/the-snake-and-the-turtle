#ifndef TURTLEMODULE_TURTLESCREEN_HPP
#define TURTLEMODULE_TURTLESCREEN_HPP

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMutex>


namespace TurtleModule {

/// The TurtleScreen implementation.
/// By turtle specification it is supposed to be a widget representing turtles arena.
class TurtleScreen : public QGraphicsView
{
    Q_OBJECT
    Q_ENUMS(Mode);

public:
    explicit TurtleScreen(QWidget *parent = 0);
    
    // types
    enum Mode
    {
        MODE_STANDARD,
        MODE_LOGO,
        MODE_WORLD
    };

    // inter-thread getters
    QColor bgcolor();
    Mode mode();

public Q_SLOTS:

    // window control
    void bgcolor(const QColor& color);
    void mode(Mode m);

    // (not implemented here) Screen methods
    virtual void setup(const QRect& geometry);

private:

    QGraphicsScene _scene;
    QMutex _mutex;

    // props
    QColor _bgcolor;
    Mode _mode;
};

} // namespace TurtleModule

#endif // TURTLEMODULE_TURTLESCREEN_HPP
