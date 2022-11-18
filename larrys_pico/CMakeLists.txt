

add_library(larryspico
  OBJECT
  src/LCD1602.cpp
  src/button.cpp
  )

target_include_directories(larryspico PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
$<TARGET_PROPERTY:pico_stdlib,INTERFACE_INCLUDE_DIRECTORIES>
$<TARGET_PROPERTY:hardware_i2c,INTERFACE_INCLUDE_DIRECTORIES>
)