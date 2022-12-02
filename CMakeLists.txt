cmake_minimum_required(VERSION 3.23)

project(
  cpprun
  LANGUAGES CXX
  VERSION 0.1.0)

option(CPPRUN_ENABLE_TESTING "Enable test of cpprun or not"
       ${PROJECT_IS_TOP_LEVEL})

include(./cmake/CPM.cmake)

CPMFindPackage(
  NAME docopt
  VERSION 0.6.1
  GITHUB_REPOSITORY docopt/docopt.cpp
  GIT_TAG v0.6.3
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL ON)

CPMFindPackage(
  NAME expected
  VERSION 1.0.0
  GITHUB_REPOSITORY TartanLlama/expected
  GIT_TAG v1.1.0
  GIT_SHALLOW ON
  OPTIONS "EXPECTED_BUILD_TESTS OFF"
  FIND_PACKAGE_ARGUMENTS "NAMES tl-expected"
  EXCLUDE_FROM_ALL ON)

if(NOT TARGET tl::expected)
  # NOTE: v1.0.0 version of tl::expected doesn't have a namespaced alias so we
  # have to add it here.
  add_library(tl::expected ALIAS expected)
endif()

CPMFindPackage(
  NAME semver
  VERSION 0.3.0
  GITHUB_REPOSITORY Neargye/Semver
  GIT_TAG v0.3.0
  GIT_SHALLOW ON
  GIT_PROGRESS ON
  EXCLUDE_FROM_ALL ON)

CPMFindPackage(
  NAME nlohmann_json
  VERSION 3.11.2
  URL "https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz"
  EXCLUDE_FROM_ALL ON
  OPTIONS "JSON_Install ON")

CPMFindPackage(
  NAME fmt
  VERSION 9.1.0
  GITHUB_REPOSITORY "fmtlib/fmt"
  GIT_TAG "10.2.1"
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL ON
  OPTIONS "CMAKE_POSITION_INDEPENDENT_CODE ON")

CPMFindPackage(
  NAME spdlog
  VERSION 1.10.0
  GITHUB_REPOSITORY "gabime/spdlog"
  GIT_TAG "v1.13.0"
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL ON
  OPTIONS
    "CMAKE_POSITION_INDEPENDENT_CODE ON" # NOTE:
                                         # https://github.com/gabime/spdlog/issues/1190
    "SPDLOG_FMT_EXTERNAL ON")

CPMFindPackage(
  NAME ocppi
  VERSION 0.2.2
  GITHUB_REPOSITORY black-desk/ocppi
  GIT_TAG v0.2.2
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL ON)

include(./cmake/GitSemver.cmake)

set(CPPRUN_SEMVER ${PROJECT_VERSION})
gitsemver(CPPRUN_SEMVER)

set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE")

include(./cmake/PFL.cmake)

pfl_init(ENABLE_TESTING ${CPPRUN_ENABLE_TESTING})

file(READ ./README.md CPPRUN_READMEMD_CONTENT)
file(READ ./docs/create.md CPPRUN_DOCS_CREATEMD_CONTENT)
file(READ ./docs/delete.md CPPRUN_DOCS_DELETEMD_CONTENT)
file(READ ./docs/exec.md CPPRUN_DOCS_EXECMD_CONTENT)
file(READ ./docs/features.md CPPRUN_DOCS_FEATURESMD_CONTENT)
file(READ ./docs/kill.md CPPRUN_DOCS_KILLMD_CONTENT)
file(READ ./docs/list.md CPPRUN_DOCS_LISTMD_CONTENT)
file(READ ./docs/run.md CPPRUN_DOCS_RUNMD_CONTENT)
file(READ ./docs/start.md CPPRUN_DOCS_STARTMD_CONTENT)
file(READ ./docs/state.md CPPRUN_DOCS_STATEMD_CONTENT)

pfl_add_library(
  INTERNAL
  VERSION
  ${CPPRUN_SEMVER}
  INS
  # find -regex '\.\/src\/.*\.in' | sort
  ./src/cpprun/configure.hpp.in
  ./src/cpprun/documents.hpp.in
  SOURCES
  # find -regex '\.\/src\/.*\.\(h\|c\)pp' | sort
  ./src/cpprun/cli.cpp
  ./src/cpprun/cli.hpp
  ./src/cpprun/cpprun.cpp
  ./src/cpprun/cpprun.hpp
  ./src/cpprun/exception.cpp
  ./src/cpprun/exception.hpp
  ./src/cpprun/json_printer.cpp
  ./src/cpprun/json_printer.hpp
  ./src/cpprun/printer.cpp
  ./src/cpprun/printer.hpp
  ./src/cpprun/text_printer.cpp
  ./src/cpprun/text_printer.hpp
  APPS
  cpprun
  LINK_LIBRARIES
  PUBLIC
  nlohmann_json::nlohmann_json
  tl::expected
  spdlog::spdlog
  docopt
  ocppi::ocppi
  semver::semver
  COMPILE_FEATURES
  PUBLIC
  cxx_std_20)
