cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(cpprun)

if(NOT DEFINED CPM_USE_LOCAL_PACKAGES)
  set(CPM_USE_LOCAL_PACKAGES ON)
endif()

include(cmake/CPM.cmake)

CPMAddPackage(
  GITHUB_REPOSITORY nlohmann/json
  NAME nlohmann_json
  VERSION 3.11.2
  GIT_SHALLOW ON
)

CPMAddPackage(
  GITHUB_REPOSITORY fmtlib/fmt
  VERSION 9.1.0
  GIT_TAG 9.1.0
  GIT_SHALLOW ON
)

CPMAddPackage(
  GITHUB_REPOSITORY docopt/docopt.cpp
  NAME docopt
  VERSION 0.6.2
  GIT_TAG master
  GIT_SHALLOW ON
)

include_directories(BEFORE include)

add_subdirectory(lib)

add_subdirectory(src)

enable_testing()
