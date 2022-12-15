#
# (C) Copyright 2022 Larry Milne (https://www.larrycloud.ca)
#
# This code is distributed on "AS IS" BASIS,
# WITHOUT WARRANTINES OR CONDITIONS OF ANY KIND.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# @author: qawse3dr a.k.a Larry Milne
#
cmake_minimum_required(VERSION 3.12)

# Check if we are running tests
# and fetch sdk if we aren't.
# Pull in SDK (must be before project)
if(NOT DEFINED PICOLATOR_TEST)
  set(PICO_SDK_FETCH_FROM_GIT 1)
  include(pico_sdk_import.cmake)
endif()

project(Picolator C CXX ASM)

# Do not set this when building for pico or else it won't load
# set(CMAKE_CXX_FLAGS '-g')


# any platform independant code
add_library(picolator_objlib
  OBJECT 
  #Math
  "math/literals.cpp"
  "math/expr_tree.cpp"
  "math/unary_operator.cpp"
  "math/math_util.cpp"
)
target_link_libraries(picolator_objlib PUBLIC m)
target_include_directories(picolator_objlib PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

# Check if we are running tests
if(NOT DEFINED PICOLATOR_TEST)

  # Enable exception
  set(PICO_CXX_ENABLE_EXCEPTIONS 1)

  # Initialize the SDK
  pico_sdk_init()

  # Define anything that requires pico sdk here
  add_subdirectory(larrys_pico)

  add_executable(picolator
  larrys_calculator.cpp
  )
  # pull in common dependencies
  target_link_libraries(picolator PRIVATE larryspico picolator_objlib pico_stdlib pico_multicore hardware_i2c)
  target_include_directories(picolator PUBLIC  ${CMAKE_CURRENT_LIST_DIR} )

  # create map/bin/hex/uf2 file etc.
  pico_add_extra_outputs(picolator)

  pico_enable_stdio_usb(picolator 1)
else()
  add_subdirectory(test)
endif()
