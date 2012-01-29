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

static const int INDENT = 4;

EditorWidget::EditorWidget(QWidget *parent) :
    QPlainTextEdit(parent),
    _currentLine(-1),
    _errorLine(-1)
{
    _sidebar = new SidebarWidget(this);

    setViewportMargins(sidebarWidth()+2, 0, 0, 0);

    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateSidebar(QRect,int)));

    new PythonHighlighter(document());
}

void EditorWidget::setCurrentlyExecutedLine(int line)
{
    _currentLine = line;
    _sidebar->update();
    ensureLineVisible(line);
}

void EditorWidget::setErrorLine(int line)
{
    _errorLine = line;
    _sidebar->update();
    ensureLineVisible(line);
}

void EditorWidget::clearMarkers()
{
    setErrorLine(0);
    setCurrentlyExecutedLine(0);
}

void EditorWidget::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    _sidebar->setGeometry(QRect(cr.left(), cr.top(), sidebarWidth(), cr.height()));

    setViewportMargins(sidebarWidth()+2, 0, 0, 0);
}

void EditorWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Tab && handleTab(e->modifiers()))
    {
        e->setAccepted(true);
        return;
    }
    else if (e->key() == Qt::Key_Backtab && handleBacktab(e->modifiers()))
    {
        e->setAccepted(true);
        return;
    }
    else if (e->key() == Qt::Key_Backspace && handleBackspace(e->modifiers()))
    {
        e->setAccepted(true);
        return;
    }

    QPlainTextEdit::keyPressEvent(e);
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

void EditorWidget::updateSidebar(const QRect &rect, int dy)
 {
     if (dy)
         _sidebar->scroll(0, dy);
     else
         _sidebar->update(0, rect.y(), _sidebar->width(), rect.height());

//     if (rect.contains(viewport()->rect()))
//         updateLineNumberAreaWidth(0);
}

void EditorWidget::ensureLineVisible(int line)
{
    QTextCursor cursor = textCursor();
    QTextBlock block = document()->findBlockByLineNumber(line);
    cursor.setPosition(block.position());
    setTextCursor(cursor);
    ensureCursorVisible();
}

bool EditorWidget::handleTab(Qt::KeyboardModifiers modifiers)
{
    QTextCursor cursor = textCursor();
    int selectionStart = cursor.selectionStart();
    int selectionEnd = cursor.selectionEnd();

    // no selection, simple 4-chart ident
    if (selectionStart == selectionEnd)
    {
        int spacesToInsert = INDENT - cursor.columnNumber()%INDENT;
        cursor.insertText(QString(" ").repeated(spacesToInsert));
    }
    // selection, indent all blocks
    else
    {
        int firstSelectedBlockNumber = document()->findBlock(selectionStart).blockNumber();
        int pastEndBlockNumber = document()->findBlock(selectionEnd).blockNumber() + 1;

        cursor.beginEditBlock();
        for(;firstSelectedBlockNumber < pastEndBlockNumber; ++firstSelectedBlockNumber)
        {
            QTextBlock block = document()->findBlockByNumber(firstSelectedBlockNumber);
            cursor.setPosition(block.position());
            cursor.insertText(QString(" ").repeated(INDENT));
        }
        cursor.endEditBlock();

    }
    return true;
}

bool EditorWidget::handleBacktab(Qt::KeyboardModifiers modifiers)
{
    QTextCursor cursor = textCursor();
    int selectionStart = cursor.selectionStart();
    int selectionEnd = cursor.selectionEnd();

    // unindent blockl
    if (selectionStart != selectionEnd)
    {
        int firstSelectedBlockNumber = document()->findBlock(selectionStart).blockNumber();
        int pastEndBlockNumber = document()->findBlock(selectionEnd).blockNumber() + 1;

        cursor.beginEditBlock();
        for(;firstSelectedBlockNumber < pastEndBlockNumber; ++firstSelectedBlockNumber)
        {
            QTextBlock block = document()->findBlockByNumber(firstSelectedBlockNumber);
            QString text = block.text();
            int spacesToRemove = 0;
            while(spacesToRemove < INDENT && text[spacesToRemove] == ' ')
                ++spacesToRemove;
            cursor.setPosition(block.position() + spacesToRemove);
            while(spacesToRemove > 0)
            {
                --spacesToRemove;
                cursor.deletePreviousChar();
            }
        }
        cursor.endEditBlock();

    }
    return true;
}

bool EditorWidget::handleBackspace(Qt::KeyboardModifiers modifiers)
{
    // unidnednt where appropriate
    QTextCursor cursor = textCursor();
    QString lineBeforeCursor = cursor.block().text().left(cursor.columnNumber());

    // remove indent if there are only spaces before cursor
    if (!lineBeforeCursor.isEmpty() && lineBeforeCursor == QString(" ").repeated(lineBeforeCursor.length()))
    {
        int charsToRemove = cursor.columnNumber() % INDENT;
        if (charsToRemove == 0)
            charsToRemove = INDENT;

        cursor.beginEditBlock();
        for(int i = 0; i < charsToRemove; ++i)
            cursor.deletePreviousChar();
        cursor.endEditBlock();

        return true;
    }

    return false;
}

} // namespace Turtle
