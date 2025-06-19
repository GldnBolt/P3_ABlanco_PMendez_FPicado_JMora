cmake_minimum_required(VERSION 3.30)
project(P3_ABlanco_PMendez_FPicado_JMora)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Forzar C++20
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/config")

file(GLOB CONFIG_FILES "${CMAKE_SOURCE_DIR}/config/*.xml")
foreach(_cfg ${CONFIG_FILES})
    get_filename_component(_name ${_cfg} NAME)
    configure_file("${_cfg}" "${CMAKE_BINARY_DIR}/config/${_name}" COPYONLY)
endforeach()

set(SOURCES
    main.cpp
    DiskNode.cpp
    ControllerNode.cpp
    HttpServer.cpp
    tinyxml2.cpp
)
add_executable(P3_ABlanco_PMendez_FPicado_JMora ${SOURCES})

if(WIN32)
    target_link_libraries(P3_ABlanco_PMendez_FPicado_JMora ws2_32)
endif()