cmake_minimum_required(VERSION 3.30)
project(P3_ABlanco_PMendez_FPicado_JMora)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

# ======== OpenSSL manual config ========
# Ajusta las rutas si tu instalación está en otro lugar
set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libcrypto.lib")
set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libssl.lib")

include_directories(${OPENSSL_INCLUDE_DIR})
link_directories("${OPENSSL_ROOT_DIR}/lib")
# =======================================

include_directories(${CMAKE_SOURCE_DIR})

# Crear build/config si no existe
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/config")

# Copiar XMLs de configuración
file(GLOB CONFIG_FILES "${CMAKE_SOURCE_DIR}/config/*.xml")
foreach(_cfg ${CONFIG_FILES})
    get_filename_component(_name ${_cfg} NAME)
    configure_file(
            "${_cfg}"
            "${CMAKE_BINARY_DIR}/config/${_name}"
            COPYONLY
    )
endforeach()

# Archivos fuente
set(SOURCES
        main.cpp
        DiskNode.cpp
        ControllerNode.cpp
        tinyxml2.cpp
        DiskNodeServer.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/extern)

set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libcrypto.lib")
set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libssl.lib")

# Agrega explícitamente la ruta a los headers
include_directories("${OPENSSL_INCLUDE_DIR}")
link_directories("${OPENSSL_ROOT_DIR}/lib")

add_definitions(-DCPPHTTPLIB_OPENSSL_SUPPORT)

add_executable(P3_ABlanco_PMendez_FPicado_JMora ${SOURCES})
target_link_libraries(P3_ABlanco_PMendez_FPicado_JMora

)
target_link_libraries(P3_ABlanco_PMendez_FPicado_JMora
        "${OPENSSL_SSL_LIBRARY}"
        "${OPENSSL_CRYPTO_LIBRARY}"
)