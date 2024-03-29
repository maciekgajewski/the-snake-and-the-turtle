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
#include <QPointer>


namespace Turtle {

namespace Ui {
    class MainWindow;
}

class PythonBridge;
class LibraryBrowser;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private Q_SLOTS:
    void lineAboutToBeExecuted(int line);
    void scriptEnded();
    void scriptError(int line, const QString& text);
    void loadFile(const QString& path);

    void on_startButton_clicked();
    void on_executeAllButton_clicked();
    void on_stepButton_clicked();
    void on_mButtonNew_clicked();
    void on_interruptButton_clicked();
    void on_executeAllFastButton_clicked();
    void on_examplesButton_clicked();

    void on_stepOverButton_clicked();

    void on_executeSuperfastButton_clicked();

protected:

    virtual void resizeEvent(QResizeEvent* event);

private:

    void startScript(bool superfast, bool fast, bool all);

    Ui::MainWindow *ui;
    PythonBridge* _python;

    bool _executeAll;
    bool _executeFast;

    QPointer<LibraryBrowser> _examplesBrowser;
};

}

#endif // MAINWINDOW_HPP
