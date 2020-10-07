cmake_minimum_required(VERSION 2.8)

project(biometryd)

# We haven't received version information via the packaging setup.
# For that, we try to determine sensible values on our own, ensuring
# plain old invocations to cmake still work as expected.
if (NOT DEFINED BIOMETRYD_VERSION_MAJOR)
  find_program(LSB_RELEASE lsb_release)
  execute_process(
    COMMAND ${LSB_RELEASE} -c -s
    OUTPUT_VARIABLE DISTRO_CODENAME
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  # We explicitly ignore errors and only check if we are building for vivid.
  # For all other cases:
  #   - releases other than vivid
  #   - other distros
  #   - errors
  # we define the version to be 0.0.1
  if ("${DISTRO_CODENAME}" STREQUAL "vivid")
    set(BIOMETRYD_VERSION_MAJOR 0)
    set(BIOMETRYD_VERSION_MINOR 0)
    set(BIOMETRYD_VERSION_PATCH 1)
  else ()
    set(BIOMETRYD_VERSION_MAJOR 1)
    set(BIOMETRYD_VERSION_MINOR 0)
    set(BIOMETRYD_VERSION_PATCH 1)
  endif()
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -pedantic -Wextra -fPIC -fvisibility=hidden -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Werror -Wall -fno-strict-aliasing -fvisibility=hidden -fvisibility-inlines-hidden -pedantic -Wextra -fPIC -pthread")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")

include(GNUInstallDirs)

set(
    BIOMETRYD_DEFAULT_PLUGIN_DIRECTORY "${CMAKE_INSTALL_FULL_LIBDIR}/biometryd/plugins"
    CACHE STRING "Default plugin installation directory")

set(
    BIOMETRYD_CUSTOM_PLUGIN_DIRECTORY "/custom/vendor/biometryd/plugins"
    CACHE STRING "Custom plugin installation directory")

enable_testing()

find_package(PkgConfig)
find_package(Boost COMPONENTS filesystem program_options system REQUIRED)

pkg_check_modules(DBUS_CPP dbus-cpp REQUIRED)
pkg_check_modules(DBUS dbus-1 REQUIRED)
pkg_check_modules(LIBAPPARMOR libapparmor REQUIRED)
pkg_check_modules(PROCESS_CPP process-cpp REQUIRED)
pkg_check_modules(SQLITE3 sqlite3 REQUIRED)

include_directories(
    include
    src

    # Make sure that files generated during build are available for compilation.
    # Most notably, this refers to include/biometry/version.h
    ${CMAKE_BINARY_DIR}/include

    ${Boost_INCLUDE_DIRS}
    ${DBUS_CPP_INCLUDE_DIRS}
    ${DBUS_INCLUDE_DIRS}
    ${LIBAPPARMOR_INCLUDE_DIRS}
    ${PROCESS_CPP_INCLUDE_DIRS}
    ${SQLITE3_INCLUDE_DIRS})

add_subdirectory(data)
add_subdirectory(doc)
add_subdirectory(include)
add_subdirectory(src)
#add_subdirectory(tests)
