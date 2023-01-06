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

CPMAddPackage(
  GITHUB_REPOSITORY google/googletest
  NAME GTest
  VERSION 1.12.1
  GIT_TAG release-1.12.1
  GIT_SHALLOW ON
)

CPMAddPackage(
  GITHUB_REPOSITORY black-desk/lib
  GIT_TAG master
  GIT_SHALLOW ON
  SOURCE_SUBDIR cpp
)

CPMAddPackage(
  NAME semver
  GITHUB_REPOSITORY Neargye/semver
  GIT_TAG v0.3.0
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(mount REQUIRED mount IMPORTED_TARGET GLOBAL)
add_library(mount ALIAS PkgConfig::mount)

pkg_check_modules(cap REQUIRED libcap IMPORTED_TARGET GLOBAL)
add_library(cap ALIAS PkgConfig::cap)
