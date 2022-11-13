cmake_minimum_required(VERSION 3.14)
project(picolator_test)


include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(
  picolator_test
  test_expr_tree.cpp
  )



target_link_libraries(
  picolator_test
  gtest_main
  picolator_objlib
)