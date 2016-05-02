configure_file(version.h.in version.h)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/version.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/biometry
)
