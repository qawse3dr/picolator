cmake_minimum_required(VERSION 3.12)

# Check if we are running tests
# and fetch sdk if we aren't.
# Pull in SDK (must be before project)
if(NOT DEFINED PICOLATOR_TEST)
  set(PICO_SDK_FETCH_FROM_GIT 1)
  include(pico_sdk_import.cmake)
endif()

project(Picolator C CXX ASM)

# any platform independant code
add_library(picolator_objlib
  OBJECT 
  #Math
  "math/literals.cpp"
  "math/expr_tree.cpp"
)
target_include_directories(picolator_objlib PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

# Check if we are running tests
if(NOT DEFINED PICOLATOR_TEST)
  # Define anything that requires pico sdk here
  add_executable(picolator
    larrys_calculator.cpp
    larrys_pico/LCD1602.cpp
    larrys_pico/button.cpp
  )

  # Initialize the SDK
  pico_sdk_init()

  # pull in common dependencies
 target_link_libraries(picolator PRIVATE picolator_objlib pico_stdlib pico_multicore hardware_i2c )
 target_include_directories(picolator PUBLIC  ${CMAKE_CURRENT_LIST_DIR})


  # create map/bin/hex/uf2 file etc.
  pico_add_extra_outputs(picolator)

  pico_enable_stdio_usb(picolator 1)
else()
  add_subdirectory(test)
endif()
