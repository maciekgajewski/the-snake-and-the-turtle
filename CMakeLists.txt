PROJECT(TheSnakeAndTheTurtle)
cmake_minimum_required(VERSION 2.8)

#NOTE: this file is quite speific to my environment, is a far cry from what one would consider
# a proper, portable project description

# Qt config
FIND_PACKAGE(Qt4 4.4 REQUIRED QtCore QtGui QtNetwork)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

# Python config
SET(PYTHON_INCLUDE_DIRS C:/Python32/include)
SET(PYTHON_LIBRARIES C:/Python32/libs/libpython32.a)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})

# general config
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wno-write-strings -Werror")

SET(CMAKE_BUILD_TYPE Debug)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})
SET(CMAKE_VERBOSE_MAKEFILE True)

ADD_SUBDIRECTORY(turtleModuleLib)
ADD_SUBDIRECTORY(gui)
ADD_SUBDIRECTORY(qturtle)
