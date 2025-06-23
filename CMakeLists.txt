cmake_minimum_required(VERSION 3.30)
project(P3_ABlanco_PMendez_FPicado_JMora)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

# -- Ruta a Qt6 para find_package
set(CMAKE_PREFIX_PATH "E:/Qt/6.9.1/mingw_64/lib/cmake/Qt6")

# -- Habilitar moc/uic automáticos
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# -- Copia de configs XML
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/config")
file(GLOB CONFIG_FILES "${CMAKE_SOURCE_DIR}/config/*.xml")
foreach(_cfg ${CONFIG_FILES})
    get_filename_component(_name ${_cfg} NAME)
    configure_file("${_cfg}" "${CMAKE_BINARY_DIR}/config/${_name}" COPYONLY)
endforeach()

# -- Código común (DiskNode + tinyxml2)
set(COMMON_SOURCES
        DiskNode.cpp
        tinyxml2.cpp
)

# -------------------------------------------------------------------
# EJECUTABLE PRINCIPAL: servidor REST + GUI Qt
# -------------------------------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

set(P3_SOURCES
        main.cpp
        ControllerNode.cpp
        HttpServer.cpp
        RemoteDiskClient.cpp
        MainWindow.cpp
        MainWindow.ui
        ${COMMON_SOURCES}
)

add_executable(P3_ABlanco_PMendez_FPicado_JMora
        ${P3_SOURCES}
)

target_link_libraries(P3_ABlanco_PMendez_FPicado_JMora
        PRIVATE
        Qt6::Widgets
        Qt6::Network
        ws2_32
)


target_include_directories(P3_ABlanco_PMendez_FPicado_JMora
        PRIVATE ${CMAKE_SOURCE_DIR}/extern
)

# -------------------------------------------------------------------
# EJECUTABLE SECUNDARIO: DiskNodeServer (sin GUI)
# -------------------------------------------------------------------
set(NODE_SOURCES
        DiskNodeServer.cpp
        ${COMMON_SOURCES}
)

add_executable(DiskNodeServer
        ${NODE_SOURCES}
)

target_link_libraries(DiskNodeServer
        PRIVATE ws2_32
)

target_include_directories(DiskNodeServer
        PRIVATE ${CMAKE_SOURCE_DIR}/extern
)
