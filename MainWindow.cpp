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
#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "Turtle.hpp"
#include "PythonBridge.hpp"

#include <QTimer>
#include <QMessageBox>

namespace Turtle {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _executeAll(false)
{
    ui->setupUi(this);

    _turtle = new Turtle(this);
    _python = new PythonBridge(_turtle, this);
    _display = new QLabel(ui->displayScrollArea);
    ui->displayScrollArea->setWidget(_display);

    connect(_turtle, SIGNAL(changed()), SLOT(onTurtleChanged()));
    connect(_python, SIGNAL(currentLineChanged(int)), SLOT(lineAboutToBeExecuted(int)));
    connect(_python, SIGNAL(scriptEnded()), SLOT(scriptEnded()));
    connect(_python, SIGNAL(scriptError(int,QString)), SLOT(scriptError(int,QString)));

    onTurtleChanged();

    QTimer::singleShot(0, this, SLOT(ensureTurtleVisible()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTurtleChanged()
{
    QPixmap pic = _turtle->render();
    _display->resize(pic.size());
    _display->setPixmap(pic);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    ensureTurtleVisible();
}

void MainWindow::ensureTurtleVisible()
{
    QPoint turtlePos = _turtle->turtlePos().toPoint();
    ui->displayScrollArea->ensureVisible(
        turtlePos.x(), turtlePos.y(),
        ui->displayScrollArea->width() * 0.5,
         ui->displayScrollArea->height() * 0.5);
}

void MainWindow::on_startButton_clicked()
{
    ui->executeAllButton->setEnabled(false);
    ui->startButton->setEnabled(false);

    QString code = ui->codeEditor->toPlainText();
    _turtle->reset();
    _executeAll = false;
    _python->executeScript(code);
}

void MainWindow::on_executeAllButton_clicked()
{
    ui->executeAllButton->setEnabled(false);
    ui->startButton->setEnabled(false);

    QString code = ui->codeEditor->toPlainText();
    _turtle->reset();
    _executeAll = true;
    _python->executeScript(code);

}

void MainWindow::lineAboutToBeExecuted(int line)
{
    ui->codeEditor->setCurrentlyExecutedLine(line);
    if (_executeAll)
    {
        QTimer::singleShot(200, _python, SLOT(step()));
    }
    else
    {
        ui->stepButton->setEnabled(true);
    }
}

void MainWindow::scriptEnded()
{
    ui->codeEditor->setCurrentlyExecutedLine(0);
    ui->executeAllButton->setEnabled(true);
    ui->startButton->setEnabled(true);
}

void MainWindow::scriptError(int line, const QString &text)
{
    if (line > 0)
        ui->codeEditor->setErrorLine(line);

    QMessageBox::critical(
        this,
        tr("Program error"),
        text);
}

void MainWindow::on_stepButton_clicked()
{
    _python->step();
    ui->stepButton->setEnabled(false);
}


}

