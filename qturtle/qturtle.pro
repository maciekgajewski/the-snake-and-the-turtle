#-------------------------------------------------
#
# Project created by QtCreator 2012-01-18T19:53:50
#
#-------------------------------------------------

QT       += phonon
CONFIG += no_keywords staticlib
QMAKE_CXXFLAGS += -std=c++0x
TEMPLATE = lib

TARGET = qturtle

# convenience: auto deployment
#TARGET_EXT = .pyd
#DESTDIR = C:/Python32/DLLs

DEFINES += QTURTLE_LIBRARY

SOURCES += Turtle.cpp \
    qturtle_global.cpp \
    Module.cpp \
    TurtleScreen.cpp \
    Screen.cpp \
    globalFunctions.cpp

HEADERS += Turtle.hpp\
        qturtle_global.hpp \
    Module.hpp \
    TurtleScreen.hpp \
    Screen.hpp \
    globalFunctions.hpp

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE2117DDA
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = qturtle.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

# Python config
INCLUDEPATH += C:/Python32/include
LIBS += -LC:/Python32/libs
LIBS += -lpython32
