QT4_WRAP_CPP(mocfiles

    EditorWidget.hpp
    MainWindow.hpp
    PythonBridge.hpp
    PythonHighlighter.hpp
)

QT4_WRAP_UI(uifiles

    MainWindow.ui
)

ADD_EXECUTABLE(gui WIN32

    main.cpp
    EditorWidget.cpp EditorWidget.hpp
    MainWindow.cpp MainWindow.hpp
    PythonBridge.cpp PythonBridge.hpp
    PythonHighlighter.cpp PythonHighlighter.hpp

    ${mocfiles}
    ${uifiles}
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

TARGET_LINK_LIBRARIES(gui

    qturtle
    ${QT_LIBRARIES}
    ${PYTHON_LIBRARIES}
)
