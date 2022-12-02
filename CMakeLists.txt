cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(cpprun)

if(NOT DEFINED CPM_LOCAL_PACKAGES_ONLY)
  set(CPM_LOCAL_PACKAGES_ONLY ON)
endif()

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME nlohmann_json
  GIT_TAG v3.11.2
  GITHUB_REPOSITORY nlohmann/json
)

CPMAddPackage(
  NAME fmt
  GIT_TAG 9.1.0
  GITHUB_REPOSITORY fmtlib/fmt
)

CPMAddPackage(
  NAME docopt
  GIT_TAG v0.6.1
  GITHUB_REPOSITORY docopt/docopt.cpp
)

include_directories(BEFORE include)

add_subdirectory(lib)

add_subdirectory(src)
