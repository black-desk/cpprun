add_executable(tests ./oci_spec/config_test.cpp)

target_link_libraries(
  tests
  PRIVATE GTest::gtest_main
  PUBLIC cxx_oci_spec
)
target_compile_features(
  tests
  PUBLIC cxx_std_17
)
set_property(TARGET tests PROPERTY CXX_STANDARD 17)

include(GoogleTest)
gtest_discover_tests(tests)
