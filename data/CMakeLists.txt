configure_file(
  biometryd.pc.in biometryd.pc @ONLY
)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/biometryd.pc
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
