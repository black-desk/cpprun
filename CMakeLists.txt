cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(cpprun VERSION 0.1.0)

include(cmake/dependencies.cmake)

if(NOT DEFINED CPPRUN_MASTER_PROJECT)
  if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(CPPRUN_MASTER_PROJECT ON)
  else()
    set(CPPRUN_MASTER_PROJECT OFF)
  endif()
endif()

if(CPPRUN_MASTER_PROJECT)
  if(NOT DEFINED CPPRUN_BUILD_TEST)
    set(CPPRUN_BUILD_TEST ON)
  endif()
  if(NOT DEFINED CMAKE_CXX_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
  endif()
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCPPRUN_DEBUG -DSPDLOG_ACTIVE_LEVEL=0")

add_subdirectory(lib)

add_subdirectory(src)

if(CPPRUN_MASTER_PROJECT)
  enable_testing()
  add_subdirectory(test)
endif()
