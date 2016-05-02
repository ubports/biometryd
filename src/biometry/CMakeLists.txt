file(GLOB_RECURSE BIOMETRYD_PUBLIC_HEADERS ${CMAKE_SOURCE_DIR}/include/*.h)

set(symbol_map "${CMAKE_SOURCE_DIR}/symbols.map")

add_subdirectory(qml)

add_library(
  biometry SHARED

  application.cpp
  daemon.h
  daemon.cpp
  device_registry.h
  device_registry.cpp
  geometry.cpp
  percent.cpp
  progress.cpp
  reason.cpp
  runtime.h
  runtime.cpp
  tracing_operation_observer.h
  user.cpp
  variant.cpp
  version.cpp
  
  cmds/enroll.h
  cmds/enroll.cpp
  cmds/identify.h
  cmds/identify.cpp
  cmds/list_devices.h
  cmds/list_devices.cpp
  cmds/help.h
  cmds/help.cpp
  cmds/version.h
  cmds/version.cpp

  dbus/codec.h
  dbus/interface.h
  dbus/service.cpp
  dbus/stub/service.h
  dbus/stub/service.cpp
  dbus/stub/device.h
  dbus/stub/device.cpp
  dbus/stub/template_store.h
  dbus/stub/template_store.cpp
  dbus/stub/identifier.h
  dbus/stub/identifier.cpp
  dbus/stub/observer.h
  dbus/stub/operation.h

  dbus/skeleton/service.h
  dbus/skeleton/service.cpp
  dbus/skeleton/device.h
  dbus/skeleton/device.cpp
  dbus/skeleton/template_store.h
  dbus/skeleton/template_store.cpp
  dbus/skeleton/identifier.h
  dbus/skeleton/identifier.cpp
  dbus/skeleton/observer.h
  dbus/skeleton/operation.h

  devices/fingerprint_reader.cpp
  devices/forwarding.h
  devices/forwarding.cpp

  devices/plugin/device.h
  devices/plugin/device.cpp
  devices/plugin/loader.h
  devices/plugin/loader.cpp

  ${BIOMETRYD_PUBLIC_HEADERS})

set_target_properties(
  biometry

  PROPERTIES
  LINK_FLAGS "${ldflags} -Wl,--version-script,${symbol_map}"
  LINK_DEPENDS ${symbol_map}
  VERSION ${BIOMETRYD_VERSION_MAJOR}.${BIOMETRYD_VERSION_MINOR}.${BIOMETRYD_SERVICE_VERSION_PATCH}
  SOVERSION ${BIOMETRYD_VERSION_MAJOR}
)

target_link_libraries(
  biometry

  util

  elf

  ${Boost_LIBRARIES}
  ${DBUS_CPP_LIBRARIES}
  ${PROCESS_CPP_LIBRARIES}
  ${SQLITE3_LIBRARIES})

add_executable(
  biometryd
  daemon_main.cpp)

target_link_libraries(
  biometryd

  biometry)

install(
  TARGETS biometry
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  TARGETS biometryd
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
