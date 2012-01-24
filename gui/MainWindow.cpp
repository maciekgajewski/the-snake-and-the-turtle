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
#include "gui/MainWindow.hpp"
#include "gui/ui_MainWindow.h"

#include "gui/PythonBridge.hpp"
#include "gui/LibraryBrowser.hpp"

#include "turtleModuleLib/Module.hpp"

#include <QTimer>
#include <QMessageBox>

namespace Turtle {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _executeAll(false),
    _executeFast(false)
{
    ui->setupUi(this);

    _python = new PythonBridge(this);

    connect(_python, SIGNAL(currentLineChanged(int)), SLOT(lineAboutToBeExecuted(int)));
    connect(_python, SIGNAL(scriptEnded()), SLOT(scriptEnded()));
    connect(_python, SIGNAL(scriptError(int,QString)), SLOT(scriptError(int,QString)));
    connect(_python, SIGNAL(stdoutData(QString)), ui->consoleOutput, SLOT(appendPlainText(QString)));

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

void MainWindow::startScript(bool fast, bool all)
{
    _executeAll = all;
    _executeFast = fast;

    ui->consoleOutput->clear();
    ui->executeAllButton->setEnabled(false);
    ui->interruptButton->setEnabled(true);
    ui->startButton->setEnabled(false);
    ui->codeEditor->clearMarkers();

    QString code = ui->codeEditor->toPlainText();
    _python->executeScript(code);
}


void MainWindow::on_startButton_clicked()
{
    startScript(false, false);
}

void MainWindow::on_executeAllButton_clicked()
{
    startScript(false, true);
}

void MainWindow::on_executeAllFastButton_clicked()
{
    startScript(true, true);
}

void MainWindow::on_examplesButton_clicked()
{
    if (!_examplesBrowser)
    {
        _examplesBrowser = new LibraryBrowser();
        connect(_examplesBrowser, SIGNAL(fileSelected(QString)), SLOT(loadFile(QString)));
    }

    _examplesBrowser->show();
}

void MainWindow::lineAboutToBeExecuted(int line)
{
    ui->codeEditor->setCurrentlyExecutedLine(line);
    if (_executeAll)
    {
        if (_executeFast)
        {
            _python->step();
        }
        else
        {
            QTimer::singleShot(200, _python, SLOT(step()));
        }
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
    ui->interruptButton->setEnabled(false);
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

void MainWindow::loadFile(const QString &path)
{
    QFile file(path);
    file.open(QFile::ReadOnly);

    ui->codeEditor->setPlainText(file.readAll());
}

void MainWindow::on_stepButton_clicked()
{
    _python->step();
    ui->stepButton->setEnabled(false);
}

void MainWindow::on_mButtonNew_clicked()
{
    _python->reset();
    ui->codeEditor->setPlainText("from qturtle import *\n");
}

void MainWindow::on_interruptButton_clicked()
{
    _python->stop();
}

}




