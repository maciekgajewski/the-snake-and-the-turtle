QT4_WRAP_CPP(mocfiles
    Screen.hpp
    TurtleScreen.hpp
    Module.hpp
    Turtle.hpp
)

ADD_LIBRARY(qturtle STATIC

    Screen.cpp Screen.hpp
    TurtleScreen.cpp TurtleScreen.hpp
    qturtle_global.cpp qturtle_global.hpp
    Module.cpp Module.hpp
    Turtle.cpp Turtle.hpp
    globalFunctions.cpp globalFunctions.hpp

    ${mocfiles}
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

TARGET_LINK_LIBRARIES(qturtle
    ${QT_LIBRARIES}
    ${PYTHON_LIBRARIES}
)
