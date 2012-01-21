#ifndef TURTLEMODULE_SCREEN_HPP
#define TURTLEMODULE_SCREEN_HPP

#include "TurtleScreen.hpp"

#include <QEventLoop>

namespace TurtleModule {

class Screen : public TurtleScreen
{
    Q_OBJECT
public:
    explicit Screen();
    
public Q_SLOTS:

    virtual void setup(const QRect &geometry);
    virtual void mainloop();
    virtual void bye();
    virtual void exitonclick();

protected:

    virtual void closeEvent(QCloseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:

    void exitLoop(); // eexits loop if loop is running

    QEventLoop* _loop;
    bool _exitOnClick;
};

} // namespace TurtleModule

#endif // TURTLEMODULE_SCREEN_HPP
