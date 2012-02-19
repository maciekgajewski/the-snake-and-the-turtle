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
#ifndef TURTLEMODULE_TURTLEFUNCTIONS_HPP
#define TURTLEMODULE_TURTLEFUNCTIONS_HPP

#include <Python.h>

namespace TurtleModule {

PyObject* showturtle_global(PyObject *self, PyObject *args);
PyObject* hideturtle_global(PyObject *self, PyObject *args);
PyObject* isvisible_global(PyObject *self, PyObject *args);
PyObject* shapesize_global(PyObject *self, PyObject *args);
PyObject* penup_global(PyObject *self, PyObject *args);
PyObject* pendown_global(PyObject *self, PyObject *args);
PyObject* isdown_global(PyObject *self, PyObject *args);
PyObject* shape_global(PyObject *self, PyObject *args);
PyObject* pencolor_global(PyObject *self, PyObject *args);
PyObject* fillcolor_global(PyObject *self, PyObject *args);
PyObject* color_global(PyObject *self, PyObject *args);
PyObject* forward_global(PyObject *self, PyObject *args);
PyObject* backward_global(PyObject *self, PyObject *args);
PyObject* goto_global(PyObject *self, PyObject *args);
PyObject* stamp_global(PyObject *self, PyObject *args);
PyObject* reset_global(PyObject *self, PyObject *args);
PyObject* left_global(PyObject *self, PyObject *args);
PyObject* right_global(PyObject *self, PyObject *args);
PyObject* setheading_global(PyObject *self, PyObject *args);
PyObject* heading_global(PyObject *self, PyObject *args);
PyObject* width_global(PyObject *self, PyObject *args);
PyObject* begin_fill_global(PyObject *self, PyObject *args);
PyObject* end_fill_global(PyObject *self, PyObject *args);
PyObject* circle_global(PyObject *self, PyObject *args, PyObject* keywords);
PyObject* home_global(PyObject *self, PyObject *args);
PyObject* dot_global(PyObject *self, PyObject *args);

void registerTurtleType(PyObject* module);

} // namespace TurtleModule

#endif // TURTLEMODULE_TURTLEFUNCTIONS_HPP
