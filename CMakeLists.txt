cmake_minimum_required(VERSION 3.30)
project(P3_ABlanco_PMendez_FPicado_JMora)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR})

# Crear build/config si no existe
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/config")

# Recoger todos los .xml de config/ y copiarlos uno a uno
file(GLOB CONFIG_FILES "${CMAKE_SOURCE_DIR}/config/*.xml")
foreach(_cfg ${CONFIG_FILES})
    # Extraer solo el nombre del archivo (ej. node1.xml)
    get_filename_component(_name ${_cfg} NAME)
    # Copiar al build/config/ manteniendo el nombre
    configure_file(
            "${_cfg}"
            "${CMAKE_BINARY_DIR}/config/${_name}"
            COPYONLY
    )
endforeach()

# Fuentes
set(COMMON_SOURCES
        DiskNode.cpp
        tinyxml2.cpp
)

# Main para ControllerNode con HttpServer
add_executable(P3_ABlanco_PMendez_FPicado_JMora
        main.cpp
        ControllerNode.cpp
        HttpServer.cpp
        RemoteDiskClient.cpp
        ${COMMON_SOURCES}
)

# Main para DiskNodeServer independiente
add_executable(DiskNodeServer
        DiskNodeServer.cpp
        ${COMMON_SOURCES}
)
if (WIN32)
    target_link_libraries(P3_ABlanco_PMendez_FPicado_JMora ws2_32)
    target_link_libraries(DiskNodeServer ws2_32)
endif()