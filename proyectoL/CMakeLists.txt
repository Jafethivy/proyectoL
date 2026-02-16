cmake_minimum_required(VERSION 3.16)
project(proyectoL LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
)
qt_standard_project_setup()

set(PROJECT_SOURCES
    main.cpp
    proyectoL.ui
    proyectoL.h
    proyectoL.cpp
)

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

set(QT_BIN_DIR "${Qt6Core_DIR}/../../../bin")

# Copiar DLLs principales de Qt (Debug)
add_custom_command(TARGET proyectoL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${QT_BIN_DIR}/Qt6Cored.dll"
        "${QT_BIN_DIR}/Qt6Guid.dll"
        "${QT_BIN_DIR}/Qt6Widgetsd.dll"
        $<TARGET_FILE_DIR:proyectoL>
    COMMENT "Copying Qt DLLs..."
)

# Crear carpeta platforms y copiar plugin
add_custom_command(TARGET proyectoL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:proyectoL>/platforms
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${QT_BIN_DIR}/../plugins/platforms/qwindowsd.dll"
        $<TARGET_FILE_DIR:proyectoL>/platforms/
    COMMENT "Copying platform plugin..."
)

# Crear carpeta styles y copiar plugin
add_custom_command(TARGET proyectoL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:proyectoL>/styles
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${QT_BIN_DIR}/../plugins/styles/qwindowsvistastyled.dll"
        $<TARGET_FILE_DIR:proyectoL>/styles/
    COMMENT "Copying style plugin..."
)

# ==========================================================

set_target_properties(proyectoL PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

include(GNUInstallDirs)
install(TARGETS proyectoL
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)