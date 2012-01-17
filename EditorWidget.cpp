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
#include "EditorWidget.hpp"
#include "PythonHighlighter.hpp"

#include <QPainter>
#include <QTextBlock>

#include <QDebug>

namespace Turtle {

EditorWidget::EditorWidget(QWidget *parent) :
    QPlainTextEdit(parent),
    _currentLine(-1),
    _errorLine(-1)
{
    _sidebar = new SidebarWidget(this);

    setViewportMargins(sidebarWidth()+2, 0, 0, 0);
    new PythonHighlighter(document());
}

void EditorWidget::setCurrentlyExecutedLine(int line)
{
    _currentLine = line;
    _sidebar->update();
}

void EditorWidget::setErrorLine(int line)
{
    _errorLine = line;
    _sidebar->update();
}

void EditorWidget::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    _sidebar->setGeometry(QRect(cr.left(), cr.top(), sidebarWidth(), cr.height()));

    setViewportMargins(sidebarWidth()+2, 0, 0, 0);

}

int EditorWidget::sidebarWidth() const
{
    return fontMetrics().height() + 4;
}

void EditorWidget::sidebarPaintEvent(QPaintEvent *event)
{
    QPainter painter(_sidebar);
    painter.fillRect(event->rect(), Qt::lightGray);
    painter.setRenderHint(QPainter::Antialiasing);

    // draw the current executed line marker
    if (_currentLine > 0)
    {
        QTextBlock block = document()->findBlockByLineNumber(_currentLine-1);
        QRectF br = blockBoundingGeometry(block);
        int top = (int) br.translated(contentOffset()).top();
        int height = fontMetrics().height();

        QPolygonF marker;
        marker << QPointF(2, top) << QPointF(sidebarWidth() - 2, top + height/2) << QPointF(2, top + height);

        painter.setBrush(Qt::green);
        painter.drawPolygon(marker);
    }
    if (_errorLine > 0)
    {
        QTextBlock block = document()->findBlockByLineNumber(_errorLine-1);
        QRectF br = blockBoundingGeometry(block);
        int top = (int) br.translated(contentOffset()).top();
        int height = fontMetrics().height();

        QRectF marker(2, top+2, sidebarWidth()-4, height-4);

        painter.setBrush(Qt::red);
        painter.drawRect(marker);
    }
}

} // namespace Turtle
