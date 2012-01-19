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

TARGET_LINK_LIBRARIES(gui

    turtleModuleLib
    ${QT_LIBRARIES}
    ${PYTHON_LIBRARIES}
)
