find_package(
  PkgConfig REQUIRED)
pkg_check_modules(systemd REQUIRED IMPORTED_TARGET libsystemd)

pfl_add_executable(
  OUTPUT_NAME
  cpprun
  SOURCES
  # find -regex '\.\/src\/.*\.\(h\|c\)pp'
  ./src/main.cpp
  LINK_LIBRARIES
  PUBLIC
  cpprun::cpprun
  PkgConfig::systemd
)
