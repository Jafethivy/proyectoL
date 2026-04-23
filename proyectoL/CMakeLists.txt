cmake_minimum_required(VERSION 3.16)
project(proyectoL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_NAME "proyectoL")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "" FORCE)

if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()


set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ${CMAKE_CURRENT_SOURCE_DIR}/gui
)

# ==========================================================
# RUTAS DE MYSQL CONNECTOR/C++ 9.6
# ==========================================================

if(WIN32)
    if(EXISTS "C:/Program Files/MySQL/MySQL Connector C++ 9.6")
        set(MYSQL_CONNECTOR_PATH "C:/Program Files/MySQL/MySQL Connector C++ 9.6" CACHE PATH "MySQL Connector C++ path")
    elseif(EXISTS "C:/Program Files (x86)/MySQL/MySQL Connector C++ 9.6")
        set(MYSQL_CONNECTOR_PATH "C:/Program Files (x86)/MySQL/MySQL Connector C++ 9.6" CACHE PATH "MySQL Connector C++ path")
    elseif(EXISTS "C:/MySQL/MySQL Connector C++ 9.6")
        set(MYSQL_CONNECTOR_PATH "C:/MySQL/MySQL Connector C++ 9.6" CACHE PATH "MySQL Connector C++ path")
    elseif(EXISTS "C:/mysql-connector-c++-9.6.0-winx64")
        set(MYSQL_CONNECTOR_PATH "C:/mysql-connector-c++-9.6.0-winx64" CACHE PATH "MySQL Connector C++ path")
    else()
        message(WARNING "MySQL Connector C++ 9.6 not found. Please set MYSQL_CONNECTOR_PATH manually.")
    endif()
endif()

# ==========================================================
# CONFIGURACIÓN DE QT
# ==========================================================

if(WIN32)
    if(EXISTS "C:/Qt/6.9.3/msvc2022_64")
        set(CMAKE_PREFIX_PATH "C:/Qt/6.9.3/msvc2022_64" CACHE PATH "Qt installation path")
    elseif(EXISTS "D:/Qt/6.9.3/msvc2022_64")
        set(CMAKE_PREFIX_PATH "D:/Qt/6.9.3/msvc2022_64" CACHE PATH "Qt installation path")
    else()
        message(WARNING "Qt not found in standard locations. Please set CMAKE_PREFIX_PATH manually.")
    endif()
endif()

# ==========================================================
# BUSCAR PAQUETES
# ==========================================================

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
        Quick
        QuickWidgets
        Qml
)
qt_standard_project_setup()

# ==========================================================
# CONFIGURACIÓN DE MYSQL CONNECTOR/C++ 9.6
# ==========================================================

if(WIN32)
    # Headers de MySQL
    set(MYSQL_INCLUDE_DIRS
        "${MYSQL_CONNECTOR_PATH}/include"
    )
    
    # Carpeta de librerias (con subcarpeta vs14)
    set(MYSQL_LIB_DIR "${MYSQL_CONNECTOR_PATH}/lib64/debug/vs14")
    
    # Verificar que existe la carpeta vs14
    if(NOT EXISTS ${MYSQL_LIB_DIR})
        message(FATAL_ERROR "No se encontro la carpeta vs14 en ${MYSQL_CONNECTOR_PATH}/lib64/debug")
    endif()
    
    # Usar siempre librerias Debug compiladas
    # Nombres exactos segun la imagen que mostraste
    set(MYSQL_CONN_LIB "${MYSQL_LIB_DIR}/mysqlcppconn.lib")
    set(MYSQL_CONNX_LIB "${MYSQL_LIB_DIR}/mysqlcppconnx.lib")
    
    message(STATUS "Usando librerias MySQL Release para todas las configuraciones")
    message(STATUS "MySQL Connector Path: ${MYSQL_CONNECTOR_PATH}")
    message(STATUS "MySQL Lib Dir: ${MYSQL_LIB_DIR}")
    
    # Verificar que existen los archivos .lib
    set(MYSQL_LIBRARIES "")
    
    if(EXISTS ${MYSQL_CONN_LIB})
        list(APPEND MYSQL_LIBRARIES ${MYSQL_CONN_LIB})
        message(STATUS "MySQL library found: ${MYSQL_CONN_LIB}")
    else()
        message(FATAL_ERROR "Libreria de MySQL no encontrada: ${MYSQL_CONN_LIB}")
    endif()
    
    if(EXISTS ${MYSQL_CONNX_LIB})
        list(APPEND MYSQL_LIBRARIES ${MYSQL_CONNX_LIB})
        message(STATUS "MySQL library found: ${MYSQL_CONNX_LIB}")
    else()
        message(FATAL_ERROR "Libreria de MySQL no encontrada: ${MYSQL_CONNX_LIB}")
    endif()
    
    # Carpeta de DLLs (estan en lib64, no en vs14)
    set(MYSQL_DLL_DIR "${MYSQL_CONNECTOR_PATH}/lib64")
    
    message(STATUS "MySQL DLL Dir: ${MYSQL_DLL_DIR}")
endif()

# ==========================================================
# FUENTES DEL PROYECTO
# ==========================================================
set(MODELS_UTILS
    src/models/utils/argon2id.h
    src/models/utils/argon2id.cpp

    src/models/utils/db.h
    src/models/utils/db.cpp
)

set (MODELS
    src/models/Login/auth.h
    src/models/Login/auth.cpp
)

set (CONTROLLERS
    src/view/login/controller.h
    src/view/login/controller.cpp

    src/view/reception/reception_controller.h
    src/view/reception/reception_controller.cpp
)

set (VIEW_SOURCES
    gui/utils/error.ui

    #Main WIndow
    gui/proyectoL.ui
    src/view/proyectoL.h
    src/view/proyectoL.cpp

    #Login
    gui/login/Login.ui
    src/view/login/Login.h
    src/view/login/Login.cpp


    #Recepcion
    gui/reception/reception.ui
    src/view/reception/reception.h
    src/view/reception/reception.cpp
)

set (SOURCE_INIT
    src/main.cpp

    src/AppContext.h
    src/AppContext.cpp
)

set(PROJECT_SOURCES
    ${MODELS_UTILS}
    ${MODELS}
    ${CONTROLLERS}
    ${VIEW_SOURCES}
    ${SOURCE_INIT}
)

if(WIN32)
    qt_add_executable(proyectoL
        WIN32
        ${PROJECT_SOURCES}
    )
endif()

# ==========================================================
# MODULO QML (Reemplaza al .qrc para archivos QML)
# ==========================================================

qt_policy(SET QTP0004 NEW)
qt_policy(SET QTP0001 NEW)

qt_add_qml_module(proyectoL
    URI ProyectoL
    VERSION 1.0
    
    QML_FILES
        qml/reception/reservations.qml
        qml/reception/reception.qml
        qml/reception/config.qml
        qml/reception/tables.qml
        qml/reception/filterPanel.qml

        qml/reception/filterComponents/SectionLabel.qml
        qml/reception/filterComponents/ToggleFilterButton.qml
        qml/reception/filterComponents/TimeRangeField.qml
        qml/reception/filterComponents/DateRangeField.qml
        qml/reception/filterComponents/StyledSpinBox.qml
        qml/reception/filterComponents/MiniToggle.qml
        
        qml/reception/generalComponents/CloseButton.qml
)

    
    # Si tienes imagenes, fonts o JSON en resources.qrc, muevelos aqui:
    # RESOURCES
    #     images/logo.png
    #     fonts/roboto.ttf
# ==========================================================
# ENLAZAR LIBRERÍAS (QT + MYSQL)
# ==========================================================

target_link_libraries(proyectoL PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Quick
    Qt::QuickWidgets
    Qt::Qml
    ${MYSQL_LIBRARIES}
)

# ==========================================================
# DIRECTORIOS DE INCLUDES
# ==========================================================

target_include_directories(proyectoL PRIVATE

    ${CMAKE_CURRENT_SOURCE_DIR}/gui
    ${CMAKE_CURRENT_BINARY_DIR}/gui

    ${CMAKE_CURRENT_SOURCE_DIR}/gui/utils
    ${CMAKE_CURRENT_BINARY_DIR}/gui/utils
    
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/login
    ${CMAKE_CURRENT_BINARY_DIR}/gui/login

    ${CMAKE_CURRENT_SOURCE_DIR}/gui/reception
    ${CMAKE_CURRENT_BINARY_DIR}/gui/reception

    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${MYSQL_INCLUDE_DIRS}
)

# ============================================================
# CONFIGURACION DE SODIUM
# ============================================================

if(WIN32)
    # =========================================================
    # WINDOWS - libsodium pre-compilado
    # =========================================================
    
    # Ruta de libsodium (ajustala segun tu instalacion)
    set(SODIUM_ROOT "D:/vcpkg/installed/x64-windows" CACHE PATH "Ruta de libsodium")
    
    # Buscar headers
    find_path(SODIUM_INCLUDE_DIR 
        NAMES sodium.h 
        PATHS ${SODIUM_ROOT}/include
        REQUIRED
    )
    
    # Buscar libreria (Release por defecto, Debug si esta configurado)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        find_library(SODIUM_LIBRARY 
            NAMES libsodium 
            PATHS ${SODIUM_ROOT}/debug/lib
            REQUIRED
        )
        set(SODIUM_DLL_PATH "${SODIUM_ROOT}/debug/bin/libsodium.dll")
    else()
        find_library(SODIUM_LIBRARY 
            NAMES libsodium 
            PATHS ${SODIUM_ROOT}/x64/Release/v143
            REQUIRED
        )
        set(SODIUM_DLL_PATH "${SODIUM_ROOT}/x64/Release/v143/libsodium.dll")
    endif()
    
    # Aplicar configuracion
    target_include_directories(${PROJECT_NAME} PRIVATE ${SODIUM_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SODIUM_LIBRARY})
    
    # Definicion para libreria estatica (opcional)
    # target_compile_definitions(${PROJECT_NAME} PRIVATE SODIUM_STATIC)
    
    # Copiar DLL automaticamente (solo si es dinamica)
    if(EXISTS ${SODIUM_DLL_PATH})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SODIUM_DLL_PATH}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copiando libsodium.dll..."
        )
    endif()    
endif()

# ==========================================================
# DESPLIEGUE DE DLLs - WINDOWS
# ==========================================================

if(WIN32)
    set(QT_BIN_DIR "${CMAKE_PREFIX_PATH}/bin")

    # Usar windeployqt para Qt
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
                --qmldir "${CMAKE_CURRENT_SOURCE_DIR}"
                \"$<TARGET_FILE:proyectoL>\"
            COMMENT "Deploying Qt dependencies with windeployqt..."
        )
    else()
        message(WARNING "windeployqt not found, using manual DLL copy")
        add_custom_command(TARGET proyectoL POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT_BIN_DIR}/Qt6Cored.dll"
                "${QT_BIN_DIR}/Qt6Guid.dll"
                "${QT_BIN_DIR}/Qt6Widgetsd.dll"
                $<TARGET_FILE_DIR:proyectoL>
            COMMENT "Copying Qt DLLs manually..."
        )
    endif()

    # ==========================================================
    # DESPLIEGUE DE DLLs DE MYSQL CONNECTOR
    # ==========================================================
    
    # Las DLLs estan en lib64 (no en vs14)
    # Nombres segun tu descripcion anterior
    set(MYSQL_DLLS
        "${MYSQL_DLL_DIR}/debug/mysqlcppconn-10-vs14.dll"
        "${MYSQL_DLL_DIR}/debug/mysqlcppconnx-2-vs14.dll"
        "${MYSQL_DLL_DIR}/libssl-3-x64.dll"
        "${MYSQL_DLL_DIR}/libcrypto-3-x64.dll"
    )
    
    # Copiar cada DLL si existe
    foreach(dll ${MYSQL_DLLS})
        if(EXISTS ${dll})
            get_filename_component(dll_name ${dll} NAME)
            add_custom_command(TARGET proyectoL POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${dll}
                    $<TARGET_FILE_DIR:proyectoL>
                COMMENT "Copying MySQL DLL: ${dll_name}"
            )
        else()
            message(WARNING "DLL de MySQL no encontrada: ${dll}")
        endif()
    endforeach()
    
    # Copiar plugins de Qt si es necesario
    if(EXISTS "${QT_BIN_DIR}/../plugins/platforms/qwindowsd.dll")
        add_custom_command(TARGET proyectoL POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:proyectoL>/platforms
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT_BIN_DIR}/../plugins/platforms/qwindowsd.dll"
                $<TARGET_FILE_DIR:proyectoL>/platforms/
        )
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