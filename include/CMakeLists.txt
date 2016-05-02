add_subdirectory(biometry)

install(
  DIRECTORY biometry
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
