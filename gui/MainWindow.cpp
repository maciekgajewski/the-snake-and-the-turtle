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

#include "PythonBridge.hpp"

#include "qturtle/Module.hpp"

#include <QTimer>
#include <QMessageBox>

namespace Turtle {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _executeAll(false)
{
    ui->setupUi(this);

    _python = new PythonBridge(this);

    connect(_python, SIGNAL(currentLineChanged(int)), SLOT(lineAboutToBeExecuted(int)));
    connect(_python, SIGNAL(scriptEnded()), SLOT(scriptEnded()));
    connect(_python, SIGNAL(scriptError(int,QString)), SLOT(scriptError(int,QString)));

    QTimer::singleShot(0, this, SLOT(ensureTurtleVisible()));

    // embedd module
    TurtleModule::Module::initialiseEmbedded(ui->sceneContainer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    // TODO remove if not used
}


void MainWindow::on_startButton_clicked()
{
    ui->executeAllButton->setEnabled(false);
    ui->startButton->setEnabled(false);

    QString code = ui->codeEditor->toPlainText();
    _executeAll = false;
    _python->executeScript(code);
}

void MainWindow::on_executeAllButton_clicked()
{
    ui->executeAllButton->setEnabled(false);
    ui->startButton->setEnabled(false);

    QString code = ui->codeEditor->toPlainText();
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

