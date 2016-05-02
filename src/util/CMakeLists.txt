add_library(
  util

  atomic_counter.h
  atomic_counter.cpp
  configuration.h
  configuration.cpp
  dynamic_library.h
  dynamic_library.cpp
  once.h
  synchronized.h
  variant.h
  variant.cpp)

target_link_libraries(
  util

  dl
  hybris-common)
