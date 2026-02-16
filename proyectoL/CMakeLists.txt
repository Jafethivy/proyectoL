cmake_minimum_required(VERSION 3.16)
project(proyectoL LANGUAGES CXX)

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    # Windows - ajusta según tu instalación
    if(EXISTS "C:/Qt/6.9.3/msvc2022_64")
        set(CMAKE_PREFIX_PATH "C:/Qt/6.9.3/msvc2022_64" CACHE PATH "Qt installation path")
    elseif(EXISTS "D:/Qt/6.9.3/msvc2022_64")
        set(CMAKE_PREFIX_PATH "D:/Qt/6.9.3/msvc2022_64" CACHE PATH "Qt installation path")
    else()
        message(WARNING "Qt not found in standard locations. Please set CMAKE_PREFIX_PATH manually.")
    endif()
elseif(UNIX AND NOT APPLE)
    # Linux - ruta típica de Qt
    if(EXISTS "$ENV{HOME}/Qt/6.9.3/gcc_64")
        set(CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.9.3/gcc_64" CACHE PATH "Qt installation path")
    elseif(EXISTS "/opt/Qt/6.9.3/gcc_64")
        set(CMAKE_PREFIX_PATH "/opt/Qt/6.9.3/gcc_64" CACHE PATH "Qt installation path")
    else()
        message(WARNING "Qt not found in standard locations. Please set CMAKE_PREFIX_PATH manually.")
    endif()
endif()

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

if(WIN32)
    qt_add_executable(proyectoL
        WIN32
        ${PROJECT_SOURCES}
    )
else()
    qt_add_executable(proyectoL
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(proyectoL PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# ==========================================================
# DESPLIEGUE DE DLLs - SOLO WINDOWS
# ==========================================================

if(WIN32)
    set(QT_BIN_DIR "${CMAKE_PREFIX_PATH}/bin")

    # Usar windeployqt para copiar todas las dependencias automáticamente
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt
        PATHS "${QT_BIN_DIR}"
        NO_DEFAULT_PATH
    )

    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET proyectoL POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --verbose 0
                --no-translations
                --no-compiler-runtime
                \"$<TARGET_FILE:proyectoL>\"
            COMMENT "Deploying Qt dependencies with windeployqt..."
        )
    else()
        # Fallback manual si windeployqt no se encuentra
        message(WARNING "windeployqt not found, using manual DLL copy")

        add_custom_command(TARGET proyectoL POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT_BIN_DIR}/Qt6Cored.dll"
                "${QT_BIN_DIR}/Qt6Guid.dll"
                "${QT_BIN_DIR}/Qt6Widgetsd.dll"
                $<TARGET_FILE_DIR:proyectoL>
            COMMENT "Copying Qt DLLs manually..."
        )

        # Plugins solo si existen
        if(EXISTS "${QT_BIN_DIR}/../plugins/platforms/qwindowsd.dll")
            add_custom_command(TARGET proyectoL POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:proyectoL>/platforms
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${QT_BIN_DIR}/../plugins/platforms/qwindowsd.dll"
                    $<TARGET_FILE_DIR:proyectoL>/platforms/
            )
        endif()
    endif()
endif()

# ==========================================================
# PROPIEDADES MULTIPLATAFORMA
# ==========================================================

if(WIN32)
    set_target_properties(proyectoL PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# MACOSX_BUNDLE solo en macOS
if(APPLE)
    set_target_properties(proyectoL PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()

include(GNUInstallDirs)
install(TARGETS proyectoL
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
