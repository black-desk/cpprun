find_package(Git)
execute_process(
  COMMAND ${GIT_EXECUTABLE} describe --abbrev=6 --dirty --always --tags
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file(config.hpp.in config.hpp)

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/../doc/create.md CPPRUN_CREATE_DOC)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/../doc/delete.md CPPRUN_DELETE_DOC)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/../doc/exec.md CPPRUN_EXEC_DOC)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/../doc/kill.md CPPRUN_KILL_DOC)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/../doc/list.md CPPRUN_LIST_DOC)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/../doc/start.md CPPRUN_START_DOC)
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/../doc/state.md CPPRUN_STATE_DOC)

configure_file(help.hpp.in help.hpp)

add_executable(cpprun main.cpp commands.cpp reexec.cpp default_logger.cpp help.cpp)
target_link_libraries(
  cpprun
  blackdesk_cpplib_core blackdesk_cpplib_linux docopt
)
target_include_directories(
  cpprun
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/
)
target_compile_features(
  cpprun
  PUBLIC cxx_std_17
)
set_property(TARGET cpprun PROPERTY CXX_STANDARD 17)
