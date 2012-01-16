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
* Copyright (C) 2010 by dvmorris@gmail.com from TopMod project
* Copyright (C) 2012 by Maciej Gajewski <maciej.gajewski0@gmail.com>
* All rights reserved.
*
*/
#include "PythonHighlighter.hpp"

#include <QDebug>

namespace Turtle {

static QStringList pythonKeywords(
        QStringList()
        << "and" << "assert" << "break" << "class" << "continue" << "def"
        << "del" << "elif" << "else" << "except" << "exec" << "finally"
        << "for" << "from" << "global" << "if" << "import" << "in"
        << "is" << "lambda" << "not" << "or" << "pass" << "print" << "raise"
        << "return" << "try" << "while" << "yield" << "True" << "False");

PythonHighlighter::PythonHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {
    /*
    pythonKeywords << "and" << "assert" << "break" << "class" << "continue" << "def"
                   << "del" << "elif" << "else" << "except" << "exec" << "finally"
                   << "for" << "from" << "global" << "if" << "import" << "in"
                   << "is" << "lambda" << "not" << "or" << "pass" << "print" << "raise"
                   << "return" << "try" << "while" << "yield" << "True" << "False";
                   */

    // formats

    QTextCharFormat functionFormat;
    functionFormat.setForeground(QColor("royalblue"));
    functionFormat.setFontWeight(QFont::Bold);

    QTextCharFormat tupleFormat;
    tupleFormat.setForeground(QColor("lightskyblue"));
    tupleFormat.setFontWeight(QFont::Bold);

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor("coral"));
    keywordFormat.setFontWeight(QFont::Bold);

    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor("crimson"));

    QTextCharFormat commentFormat;
    commentFormat.setForeground(QColor("orangered"));
    commentFormat.setFontItalic(true);

    // expresions

    QRegExp stringPattern("(\".*\")|'.*'");
    QRegExp functionPattern("\\b[A-Za-z0-9_]+(?=\\()");
    QRegExp tuplePattern("(\\((-?\\d+(\\.\\d*)?,[ ]*)+-?\\d+(\\.\\d*)?\\))|(\\[(-?\\d+(\\.\\d*)?,[ ]*)+-?\\d+(\\.\\d*)?\\])");
    QRegExp commentPattern("#.*");

    // build rules
    _rules.append(qMakePair(stringPattern, stringFormat));
    _rules.append(qMakePair(functionPattern, stringFormat));
    _rules.append(qMakePair(tuplePattern, tupleFormat));

    QString delim("\\b");
    Q_FOREACH(const QString& keyword, pythonKeywords)
    {
        QRegExp pattern(delim + keyword + delim);
        _rules.append(qMakePair(pattern, keywordFormat));
    }

    // must be last
    _rules.append(qMakePair(commentPattern, commentFormat));

}

PythonHighlighter::~PythonHighlighter( ) {

}

void PythonHighlighter::highlightBlock(const QString& text) {
    if (text.startsWith(">")) return;

    Q_FOREACH(const Rule& rule, _rules)
    {
        const QRegExp& pattern = rule.first;
        const QTextCharFormat& format = rule.second;

        int index = pattern.indexIn(text);
        while (index >= 0)
        {
            int length = pattern.matchedLength();
            qDebug() << "found" << pattern.pattern() << "in" << text << "length is" << length;
            setFormat(index, length, format);
            index = pattern.indexIn(text, index+length);
        }
    }

}


} // namespace Turtle
