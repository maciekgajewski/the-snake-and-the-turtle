#-------------------------------------------------
#
# Project created by QtCreator 2012-01-14T20:54:32
#
#-------------------------------------------------

QT       += core gui
CONFIG += no_keywords

TARGET = turtle
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
        MainWindow.cpp \
    Turtle.cpp \
    PythonBridge.cpp \
    EditorWidget.cpp \
    PythonHighlighter.cpp


HEADERS  += MainWindow.hpp \
    Turtle.hpp \
    PythonBridge.hpp \
    EditorWidget.hpp \
    PythonHighlighter.hpp

FORMS    += MainWindow.ui


# Python config
INCLUDEPATH += C:/Python32/include
LIBS += -LC:/Python32/libs
LIBS += -lpython32
