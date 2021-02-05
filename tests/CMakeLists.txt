find_package(Qt5Core 5.4 REQUIRED)
find_package(Qt5Qml 5.4 REQUIRED)
find_package(Qt5Quick 5.4 REQUIRED)
find_package(Qt5QuickTest 5.4 REQUIRED)

configure_file(config.cpp.in config.cpp)
include_directories(.)
# Build with system gmock and embedded gtest
find_package(GMock)

# Ensure that we have a ready-made fixture maintaining a private bus instance
# readily available in unit- and integration tests.
add_definitions(-DCORE_DBUS_ENABLE_GOOGLE_TEST_FIXTURE)

macro(BIOMETRYD_ADD_TEST test_name src)
  add_executable(
    ${test_name}
    ${src}
    ${CMAKE_CURRENT_SOURCE_DIR}/did_finish_successfully.h
    ${CMAKE_CURRENT_SOURCE_DIR}/echo_service.h
    ${CMAKE_CURRENT_SOURCE_DIR}/echo_service.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h
    ${CMAKE_CURRENT_BINARY_DIR}/config.cpp)

  target_link_libraries(
    ${test_name}

    biometry

    ${Boost_LIBRARIES}
    ${DBUS_CPP_LIBRARIES}
    ${PROCESS_CPP_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}

    ${ARGN}

    ${GMOCK_LIBRARIES}
    ${GTEST_BOTH_LIBRARIES})

  add_test(${test_name} ${CMAKE_CURRENT_BINARY_DIR}/${test_name} --gtest_filter=*-*requires*)
  set_tests_properties(${test_name} PROPERTIES ENVIRONMENT "BIOMETRYD_DBUS_SKELETON_IS_RUNNING_UNDER_TESTING=1")
endmacro(BIOMETRYD_ADD_TEST)

add_library(biometryd_devices_plugin_dl SHARED biometryd_devices_plugin_dl.cpp)
target_link_libraries(biometryd_devices_plugin_dl gtest gmock)

add_library(biometryd_devices_plugin_dl_version_mismatch SHARED biometryd_devices_plugin_dl_version_mismatch.cpp)
target_link_libraries(biometryd_devices_plugin_dl_version_mismatch gtest gmock)

BIOMETRYD_ADD_TEST(test_atomic_counter test_atomic_counter.cpp)
BIOMETRYD_ADD_TEST(test_configuration test_configuration.cpp)
BIOMETRYD_ADD_TEST(test_daemon test_daemon.cpp)
BIOMETRYD_ADD_TEST(test_device_registrar test_device_registrar.cpp)
BIOMETRYD_ADD_TEST(test_dispatching_device_and_service test_dispatching_service_and_device.cpp)
BIOMETRYD_ADD_TEST(test_dbus_codec test_dbus_codec.cpp)
BIOMETRYD_ADD_TEST(test_dbus_stub_skeleton test_dbus_stub_skeleton.cpp)
BIOMETRYD_ADD_TEST(test_dictionary test_dictionary.cpp)
BIOMETRYD_ADD_TEST(test_fingerprint_reader test_fingerprint_reader.cpp)
BIOMETRYD_ADD_TEST(test_forwarding test_forwarding.cpp)
BIOMETRYD_ADD_TEST(test_geometry test_geometry.cpp)
BIOMETRYD_ADD_TEST(test_operation test_operation.cpp)
BIOMETRYD_ADD_TEST(test_percent test_percent.cpp)
BIOMETRYD_ADD_TEST(test_plugin_device test_plugin_device.cpp)
BIOMETRYD_ADD_TEST(test_progress test_progress.cpp)
BIOMETRYD_ADD_TEST(test_user test_user.cpp)
BIOMETRYD_ADD_TEST(test_verifier test_verifier.cpp)

BIOMETRYD_ADD_TEST(test_cmds_config cmds/test_config.cpp)
BIOMETRYD_ADD_TEST(test_cmds_run cmds/test_run.cpp)
BIOMETRYD_ADD_TEST(test_cmds_test cmds/test_test.cpp)

if ("DEB_TARGET_ARCH" STREQUAL "powerpc")
else()
add_executable(test_qml_plugin test_qml_plugin.cpp)
qt5_use_modules(test_qml_plugin Core Qml Quick QuickTest)
add_test(test_qml_plugin test_qml_plugin)
set_tests_properties(test_qml_plugin PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=minimal;QML2_IMPORT_PATH=${CMAKE_BINARY_DIR}/src/biometry/qml;BIOMETRYD_QML_ENABLE_TESTING=1")
file(GLOB BIOMETRYD_QML_TEST_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} tst_*.qml)

foreach (QML_FILE ${BIOMETRYD_QML_TEST_FILES})
    message(INFO  ${QML_FILE})
    configure_file(${QML_FILE} ${QML_FILE} COPYONLY)
  endforeach(QML_FILE)
endif()
