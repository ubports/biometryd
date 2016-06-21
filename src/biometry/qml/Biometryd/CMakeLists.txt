set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -pedantic -Wextra -fPIC -fvisibility=hidden -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Werror -Wall -fno-strict-aliasing -pedantic -Wextra -fPIC -pthread")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")

find_package(Qt5Core 5.4 REQUIRED)
find_package(Qt5Qml 5.4 REQUIRED)
find_package(Qt5Quick 5.4 REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(
    biometryd-qml SHARED

    device.h
    device.cpp
    converter.h
    converter.cpp
    fingerprint_reader.h
    fingerprint_reader.cpp
    identifier.h
    identifier.cpp
    operation.h
    operation.cpp
    service.h
    service.cpp
    template_store.h
    template_store.cpp
    user.h
    user.cpp

    plugin.h
    plugin.cpp)

qt5_use_modules(biometryd-qml Core Qml Quick)
target_link_libraries(biometryd-qml biometry)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plugins.qmltypes
    COMMAND qmlplugindump -notrelocatable Biometryd 0.0 ../ > ${CMAKE_CURRENT_BINARY_DIR}/plugins.qmltypes
    DEPENDS biometryd-qml
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

# We make the module available to qml tests without
# requiring installation of the module. With that, we
# can test the module within the build env.
configure_file(qmldir qmldir COPYONLY)

set(PLUGIN_DIR ${CMAKE_INSTALL_LIBDIR}/qt5/qml/Biometryd)

install(
    TARGETS biometryd-qml
    DESTINATION ${PLUGIN_DIR})

install(
    FILES qmldir
    DESTINATION ${PLUGIN_DIR})

add_custom_target(
    Content_generated_files ALL
    SOURCES ${CMAKE_CURRENT_BINARY_DIR}/plugins.qmltypes)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/plugins.qmltypes
    DESTINATION ${PLUGIN_DIR})
