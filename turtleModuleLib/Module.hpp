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
#ifndef TURTLEMODULE_MODULE_HPP
#define TURTLEMODULE_MODULE_HPP

#include <QObject>
#include <QMutex>
#include <QWidget>

namespace TurtleModule {

class TurtleScreen;

/// A singleton class representing the module loaded into memory.
/// The insatnce of this class is created automatically when needed and performs neccesary initalizations,
// or it can be created manually to control th behaviour of the module (ie to embedd the screen in Qt app)

class Module : public QObject
{
    Q_OBJECT
public:
    /// Singletonm instance
    static Module* instance();

    /// Initializes module for embedding. Creates the default screen as child widget of provided here
    static Module* initialiseEmbedded(QWidget* parent);

    /// The unnamed, default screen. will be created if does not exist
    TurtleScreen* screen();

    /// Reset the module
    void reset();

private:
    Module();
    Module(QWidget* widget);

    // singleton instance
    static QMutex _mutex; // guarding the instance
    static Module* _instance;

    /// Default screen
    TurtleScreen* _screen;

    bool _embedded;
    
};

} // namespace TurtlrModule

#endif // TURTLEMODULE_MODULE_HPP
