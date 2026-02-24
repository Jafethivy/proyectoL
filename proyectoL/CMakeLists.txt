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
    
elseif(UNIX AND NOT APPLE)
    if(EXISTS "/usr/include/mysql-cppconn-9")
        set(MYSQL_CONNECTOR_PATH "/usr" CACHE PATH "MySQL Connector C++ path")
    elseif(EXISTS "/usr/local/mysql-connector-c++-9.6")
        set(MYSQL_CONNECTOR_PATH "/usr/local/mysql-connector-c++-9.6" CACHE PATH "MySQL Connector C++ path")
    else()
        message(WARNING "MySQL Connector C++ not found. Install libmysqlcppconn-dev or set MYSQL_CONNECTOR_PATH.")
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
elseif(UNIX AND NOT APPLE)
    if(EXISTS "$ENV{HOME}/Qt/6.9.3/gcc_64")
        set(CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.9.3/gcc_64" CACHE PATH "Qt installation path")
    elseif(EXISTS "/opt/Qt/6.9.3/gcc_64")
        set(CMAKE_PREFIX_PATH "/opt/Qt/6.9.3/gcc_64" CACHE PATH "Qt installation path")
    else()
        message(WARNING "Qt not found in standard locations. Please set CMAKE_PREFIX_PATH manually.")
    endif()
endif()

# ==========================================================
# BUSCAR PAQUETES
# ==========================================================

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    COMPONENTS
        Core
        Gui
        Widgets
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
    
    # Carpeta de librerías (con subcarpeta vs14)
    set(MYSQL_LIB_DIR "${MYSQL_CONNECTOR_PATH}/lib64/debug/vs14")
    
    # Verificar que existe la carpeta vs14
    if(NOT EXISTS ${MYSQL_LIB_DIR})
        message(FATAL_ERROR "No se encontró la carpeta vs14 en ${MYSQL_CONNECTOR_PATH}/lib64/debug")
    endif()
    
    # Usar siempre librerías Debug compiladas
    # Nombres exactos según la imagen que mostraste
    set(MYSQL_CONN_LIB "${MYSQL_LIB_DIR}/mysqlcppconn.lib")
    set(MYSQL_CONNX_LIB "${MYSQL_LIB_DIR}/mysqlcppconnx.lib")
    
    message(STATUS "Usando librerías MySQL Release para todas las configuraciones")
    message(STATUS "MySQL Connector Path: ${MYSQL_CONNECTOR_PATH}")
    message(STATUS "MySQL Lib Dir: ${MYSQL_LIB_DIR}")
    
    # Verificar que existen los archivos .lib
    set(MYSQL_LIBRARIES "")
    
    if(EXISTS ${MYSQL_CONN_LIB})
        list(APPEND MYSQL_LIBRARIES ${MYSQL_CONN_LIB})
        message(STATUS "MySQL library found: ${MYSQL_CONN_LIB}")
    else()
        message(FATAL_ERROR "Librería de MySQL no encontrada: ${MYSQL_CONN_LIB}")
    endif()
    
    if(EXISTS ${MYSQL_CONNX_LIB})
        list(APPEND MYSQL_LIBRARIES ${MYSQL_CONNX_LIB})
        message(STATUS "MySQL library found: ${MYSQL_CONNX_LIB}")
    else()
        message(FATAL_ERROR "Librería de MySQL no encontrada: ${MYSQL_CONNX_LIB}")
    endif()
    
    # Carpeta de DLLs (están en lib64, no en vs14)
    set(MYSQL_DLL_DIR "${MYSQL_CONNECTOR_PATH}/lib64")
    
    message(STATUS "MySQL DLL Dir: ${MYSQL_DLL_DIR}")
    
else()
    # Linux
    find_package(mysql-concpp QUIET)
    
    if(mysql-concpp_FOUND)
        set(MYSQL_LIBRARIES mysql::concpp)
    else()
        find_package(PkgConfig QUIET)
        if(PkgConfig_FOUND)
            pkg_check_modules(MYSQL_CPPconn mysqlcppconn)
        endif()
        
        if(NOT MYSQL_CPPconn_FOUND)
            set(MYSQL_INCLUDE_DIRS "/usr/include/mysql-cppconn-9")
            set(MYSQL_LIBRARIES 
                "/usr/lib/x86_64-linux-gnu/libmysqlcppconn.so"
                "/usr/lib/x86_64-linux-gnu/libmysqlcppconnx.so"
            )
        endif()
    endif()
endif()

# ==========================================================
# FUENTES DEL PROYECTO
# ==========================================================

set(PROJECT_SOURCES
    # Models (lógica de negocio)
    src/models/argon2id.h
    src/models/argon2id.cpp
    src/models/db.h
    src/models/db.cpp
    
    # Controllers (orquestación)
    src/controllers/controller.h
    src/controllers/controller.cpp
    
    # View (interfaz gráfica)
    src/view/proyectoL.h
    src/view/proyectoL.cpp
    # UI
    gui/proyectoL.ui

    # Entry point
    src/main.cpp
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

# ==========================================================
# ENLAZAR LIBRERÍAS (QT + MYSQL)
# ==========================================================

target_link_libraries(proyectoL PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    ${MYSQL_LIBRARIES}
    $
)

# ==========================================================
# DIRECTORIOS DE INCLUDES
# ==========================================================

target_include_directories(proyectoL PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/src

    ${CMAKE_CURRENT_BINARY_DIR}/gui
    ${MYSQL_INCLUDE_DIRS}
)

# ============================================================
# CONFIGURACION DE SODIUM
# ============================================================

if(WIN32)
    # =========================================================
    # WINDOWS - libsodium pre-compilado
    # =========================================================
    
    # Ruta de libsodium (ajústala según tu instalación)
    set(SODIUM_ROOT "D:/vcpkg/installed/x64-windows" CACHE PATH "Ruta de libsodium")
    
    # Buscar headers
    find_path(SODIUM_INCLUDE_DIR 
        NAMES sodium.h 
        PATHS ${SODIUM_ROOT}/include
        REQUIRED
    )
    
    # Buscar librería (Release por defecto, Debug si está configurado)
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
    
    # Aplicar configuración
    target_include_directories(${PROJECT_NAME} PRIVATE ${SODIUM_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SODIUM_LIBRARY})
    
    # Definición para librería estática (opcional)
    # target_compile_definitions(${PROJECT_NAME} PRIVATE SODIUM_STATIC)
    
    # Copiar DLL automáticamente (solo si es dinámica)
    if(EXISTS ${SODIUM_DLL_PATH})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SODIUM_DLL_PATH}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copiando libsodium.dll..."
        )
    endif()

else()
    # =========================================================
    # LINUX / UNIX - libsodium vía pkg-config o sistema
    # =========================================================
    
    # Buscar usando pkg-config (método estándar en Linux)
    find_package(PkgConfig QUIET)
    
    if(PkgConfig_FOUND)
        pkg_check_modules(SODIUM QUIET libsodium)
        
        if(SODIUM_FOUND)
            # Usar configuración de pkg-config
            target_include_directories(tu_app PRIVATE ${SODIUM_INCLUDE_DIRS})
            target_link_libraries(tu_app PRIVATE ${SODIUM_LIBRARIES})
            target_compile_options(tu_app PRIVATE ${SODIUM_CFLAGS_OTHER})
            
            message(STATUS "libsodium encontrado vía pkg-config")
        endif()
    endif()
    
    # Si no se encontró vía pkg-config, buscar manualmente
    if(NOT SODIUM_FOUND)
        find_path(SODIUM_INCLUDE_DIR 
            NAMES sodium.h 
            PATHS /usr/include /usr/local/include
            REQUIRED
        )
        
        find_library(SODIUM_LIBRARY 
            NAMES sodium libsodium 
            PATHS /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu
            REQUIRED
        )
        
        target_include_directories(tu_app PRIVATE ${SODIUM_INCLUDE_DIR})
        target_link_libraries(tu_app PRIVATE ${SODIUM_LIBRARY})
        
        message(STATUS "libsodium encontrado manualmente")
    endif()
    
    # En Linux, vincular pthread y posiblemente rt (requerido por libsodium)
    find_package(Threads REQUIRED)
    target_link_libraries(tu_app PRIVATE Threads::Threads)
    
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
    
    # Las DLLs están en lib64 (no en vs14)
    # Nombres según tu descripción anterior
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