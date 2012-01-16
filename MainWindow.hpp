/*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software  Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* Copyright (C) 2012 by Maciej Gajewski <maciej.gajewski0@gmail.com>
* All rights reserved.
*
* The Original Code is: all of this file.
*/
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QLabel>

namespace Ui {
    class MainWindow;
}

namespace Turtle {

class PythonBridge;
class Turtle;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private Q_SLOTS:
    void onTurtleChanged();
    void ensureTurtleVisible();
    void lineAboutToBeExecuted(int line);
    void scriptEnded();
    void scriptError(int line, const QString& text);

    void on_startButton_clicked();
    void on_executeAllButton_clicked();

    void on_stepButton_clicked();

protected:

    virtual void resizeEvent(QResizeEvent* event);

private:

    Ui::MainWindow *ui;
    PythonBridge* _python;
    Turtle* _turtle;

    QLabel* _display;

    bool _executeAll;
};

}

#endif // MAINWINDOW_HPP
