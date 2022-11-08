cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
set(PICO_SDK_FETCH_FROM_GIT 1)
include(pico_sdk_import.cmake)

project(LarrysCalculator C CXX ASM)
add_executable(LarrysCalculator
        larrys_calculator.cpp
        LCD1602.cpp
        button.cpp

        #Math
        "math/literals.cpp"
        )

# Initialize the SDK
pico_sdk_init()


# pull in common dependencies
target_link_libraries(LarrysCalculator pico_stdlib pico_multicore hardware_i2c)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(LarrysCalculator PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(LarrysCalculator)

pico_enable_stdio_usb(LarrysCalculator 1)

