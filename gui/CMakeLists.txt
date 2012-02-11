QT4_WRAP_CPP(mocfiles

    EditorWidget.hpp
    MainWindow.hpp
    PythonBridge.hpp
    PythonHighlighter.hpp
    LibraryBrowser.hpp
)

QT4_WRAP_UI(uifiles

    MainWindow.ui
    LibraryBrowser.ui
)

ADD_EXECUTABLE(gui WIN32

    main.cpp
    EditorWidget.cpp EditorWidget.hpp
    Directories.cpp Directories.hpp
    MainWindow.cpp MainWindow.hpp
    PythonBridge.cpp PythonBridge.hpp
    PythonHighlighter.cpp PythonHighlighter.hpp
    Emb.cpp Emb.hpp
    Library.cpp Library.hpp
    LibraryBrowser.cpp LibraryBrowser.hpp
    Synchronizer.cpp Synchronizer.hpp

    ${mocfiles}
    ${uifiles}
)

TARGET_LINK_LIBRARIES(gui

    turtleModuleLib
    ${QT_LIBRARIES}
    ${PYTHON_LIBRARIES}
)
