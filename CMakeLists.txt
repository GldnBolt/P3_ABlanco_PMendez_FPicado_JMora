cmake_minimum_required(VERSION 3.30)
project(P3_ABlanco_PMendez_FPicado_JMora)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

# Incluir encabezados
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/extern)  # Asegúrate de incluir httplib

# Crear carpeta de configuración si no existe
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/config")

# Copiar todos los archivos XML de config/ a build/config/
file(GLOB CONFIG_FILES "${CMAKE_SOURCE_DIR}/config/*.xml")
foreach(_cfg ${CONFIG_FILES})
    get_filename_component(_name ${_cfg} NAME)
    configure_file("${_cfg}" "${CMAKE_BINARY_DIR}/config/${_name}" COPYONLY)
endforeach()

# Archivos fuente compartidos
set(COMMON_SOURCES
        DiskNode.cpp
        tinyxml2.cpp
)

# Ejecutable principal (Controlador)
add_executable(P3_ABlanco_PMendez_FPicado_JMora
        main.cpp
        ControllerNode.cpp
        HttpServer.cpp
        RemoteDiskClient.cpp
        ${COMMON_SOURCES}
)

# Ejecutable para los nodos remotos
add_executable(DiskNodeServer
        DiskNodeServer.cpp
        ${COMMON_SOURCES}
)

# Enlazar con Winsock2 en Windows
if (WIN32)
    target_link_libraries(P3_ABlanco_PMendez_FPicado_JMora ws2_32)
    target_link_libraries(DiskNodeServer ws2_32)
endif()