cmake_minimum_required(VERSION 3.30)
project(P3_ABlanco_PMendez_FPicado_JMora)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

<<<<<<< HEAD
# Incluir directorio para httplib.h
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)
=======
include_directories(${CMAKE_SOURCE_DIR})
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e

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
set(SOURCES
<<<<<<< HEAD
    main.cpp
    DiskNode.cpp
    ControllerNode.cpp
    HttpServer.cpp
    tinyxml2.cpp
)
add_executable(P3_ABlanco_PMendez_FPicado_JMora ${SOURCES})
# En Windows, añadir Winsock para cpp-httplib
if(WIN32)
    target_link_libraries(P3_ABlanco_PMendez_FPicado_JMora ws2_32)
endif()
=======
        main.cpp
        DiskNode.cpp
        ControllerNode.cpp
        tinyxml2.cpp
)

add_executable(P3_ABlanco_PMendez_FPicado_JMora ${SOURCES})
>>>>>>> 5d8529b2b1638fd79eadf79dd08b4da6da5fd95e
