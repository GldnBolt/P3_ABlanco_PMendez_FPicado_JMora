cmake_minimum_required(VERSION 3.30)
project(P3_ABlanco_PMendez_FPicado_JMora)

set(CMAKE_CXX_STANDARD 20)
# Para filesystem en C++20
set(CMAKE_CXX_EXTENSIONS OFF)

# Carpeta raíz como include path
include_directories(${CMAKE_SOURCE_DIR})

# Copiar config.xml al build dir para que el ejecutable lo vea
file(COPY ${CMAKE_SOURCE_DIR}/config/config.xml
        DESTINATION ${CMAKE_BINARY_DIR})

add_executable(P3_ABlanco_PMendez_FPicado_JMora
        main.cpp
        DiskNode.cpp
        tinyxml2.cpp
        ControllerNode.cpp
        ControllerNode.h)
