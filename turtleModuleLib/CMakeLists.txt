QT4_WRAP_CPP(mocfiles
    Screen.hpp
    TurtleScreen.hpp
    Module.hpp
    Turtle.hpp
)

ADD_LIBRARY(turtleModuleLib STATIC

    Screen.cpp Screen.hpp
    TurtleScreen.cpp TurtleScreen.hpp
    Module.cpp Module.hpp
    Turtle.cpp Turtle.hpp
    GlobalFunctions.cpp GlobalFunctions.hpp
    ScreenFunctions.cpp ScreenFunctions.hpp

    ${mocfiles}
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

TARGET_LINK_LIBRARIES(turtleModuleLib
    ${QT_LIBRARIES}
    ${PYTHON_LIBRARIES}
)
