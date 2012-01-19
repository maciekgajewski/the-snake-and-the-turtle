
ADD_LIBRARY(qturtle SHARED

    qturtle_global.cpp qturtle_global.hpp

    ${mocfiles}
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

TARGET_LINK_LIBRARIES(qturtle
    turtleModuleLib
    ${PYTHON_LIBRARIES}
    ${QT_LIBRARIES}
)
