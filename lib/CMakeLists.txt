add_library(
  cxx_oci_spec
  cxx_oci_spec/capabilities.cpp cxx_oci_spec/config.cpp cxx_oci_spec/default_logger.cpp
  cxx_oci_spec/rlimits.cpp cxx_oci_spec/verified_config.cpp cxx_oci_spec/verify_helper.cpp
)
target_link_libraries(
  cxx_oci_spec
  PUBLIC blackdesk_cpplib_linux
  PUBLIC mount
  PUBLIC nlohmann_json::nlohmann_json
  PUBLIC semver
)
target_include_directories(
  cxx_oci_spec
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cxx_oci_spec
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/cxx_oci_spec
)
target_compile_features(
  cxx_oci_spec
  PUBLIC cxx_std_17
)
set_property(TARGET cxx_oci_spec PROPERTY CXX_STANDARD 17)
