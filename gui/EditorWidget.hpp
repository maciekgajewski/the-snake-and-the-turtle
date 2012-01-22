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
#ifndef TURTLE_EDITORWIDGET_HPP
#define TURTLE_EDITORWIDGET_HPP

#include <QPlainTextEdit>

namespace Turtle {

class SidebarWidget;

class EditorWidget : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit EditorWidget(QWidget *parent = 0);
    
public Q_SLOTS:

    // sets highlighet lines. 1-based, line<=0 unsets
    void setCurrentlyExecutedLine(int line);
    void setErrorLine(int line);
    void clearMarkers(); // removes all markers
    
protected:

    virtual void resizeEvent(QResizeEvent *e);
    virtual void keyPressEvent(QKeyEvent *e);

private:

    friend class SidebarWidget;

    int sidebarWidth() const;
    void sidebarPaintEvent(QPaintEvent *event);

    // keyboard support
    bool handleTab(Qt::KeyboardModifiers modifiers); // handle tab press, return true if handled
    bool handleBacktab(Qt::KeyboardModifiers modifiers); // handle tab press, return true if handled
    bool handleBackspace(Qt::KeyboardModifiers modifiers); // unidents where appropriate

    int _currentLine;
    int _errorLine;
    SidebarWidget* _sidebar;
};

class SidebarWidget : public QWidget
{
public:

    SidebarWidget(EditorWidget* parent)
        : QWidget(parent), _parent(parent) { }

    QSize sizeHint() const {
        return QSize(_parent->sidebarWidth(), 0);
    }

protected:

    void paintEvent(QPaintEvent *event) {
        _parent->sidebarPaintEvent(event);
    }

private:

    EditorWidget* _parent;
};

} // namespace Turtle

#endif // TURTLE_EDITORWIDGET_HPP
