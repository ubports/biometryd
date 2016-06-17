file(GLOB_RECURSE BIOMETRYD_PUBLIC_HEADERS ${CMAKE_SOURCE_DIR}/include/*.h)

set(symbol_map "${CMAKE_SOURCE_DIR}/symbols.map")

add_subdirectory(qml)

configure_file(daemon_configuration.cpp.in daemon_configuration.cpp)

add_library(
  biometry SHARED

  application.cpp
  daemon.h
  daemon.cpp  
  device_registrar.h
  device_registrar.cpp
  device_registry.h
  device_registry.cpp
  dispatching_service.h
  dispatching_service.cpp
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
  
  "${CMAKE_CURRENT_BINARY_DIR}/daemon_configuration.cpp"

  cmds/config.h
  cmds/config.cpp
  cmds/enroll.h
  cmds/enroll.cpp
  cmds/identify.h
  cmds/identify.cpp
  cmds/list_devices.h
  cmds/list_devices.cpp
  cmds/run.h
  cmds/run.cpp
  cmds/test.h
  cmds/test.cpp
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

  dbus/skeleton/credentials_resolver.h
  dbus/skeleton/daemon_credentials_resolver.h
  dbus/skeleton/daemon_credentials_resolver.cpp
  dbus/skeleton/request_verifier.h
  dbus/skeleton/request_verifier.cpp
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

  devices/dispatching.h
  devices/dispatching.cpp
  devices/dummy.h
  devices/dummy.cpp
  devices/fingerprint_reader.cpp
  devices/forwarding.h
  devices/forwarding.cpp

  devices/plugin/device.h
  devices/plugin/device.cpp
  devices/plugin/enumerator.h
  devices/plugin/enumerator.cpp
  devices/plugin/loader.h
  devices/plugin/loader.cpp
  devices/plugin/verifier.h
  devices/plugin/verifier.cpp

  util/atomic_counter.h
  util/atomic_counter.cpp
  util/benchmark.h
  util/benchmark.cpp
  util/cli.h
  util/cli.cpp
  util/configuration.h
  util/configuration.cpp
  util/dispatcher.h
  util/dispatcher.cpp
  util/dynamic_library.h
  util/dynamic_library.cpp
  util/json_configuration_builder.h
  util/json_configuration_builder.cpp
  util/not_implemented.h
  util/not_implemented.cpp
  util/not_reachable.h
  util/not_reachable.cpp
  util/once.h
  util/statistics.h
  util/statistics.cpp
  util/streaming_configuration_builder.h
  util/synchronized.h

  ${BIOMETRYD_PUBLIC_HEADERS})

set_target_properties(
  biometry

  PROPERTIES
  LINK_FLAGS "${ldflags} -Wl,--version-script,${symbol_map}"
  LINK_DEPENDS ${symbol_map}
  VERSION ${BIOMETRYD_VERSION_MAJOR}.${BIOMETRYD_VERSION_MINOR}.${BIOMETRYD_VERSION_PATCH}
  SOVERSION ${BIOMETRYD_VERSION_MAJOR}
)

target_link_libraries(
  biometry

  util

  dl
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
