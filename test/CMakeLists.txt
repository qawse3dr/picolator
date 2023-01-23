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
cmake_minimum_required(VERSION 3.14)
project(picolator_test)


find_package(GTest REQUIRED)
include(GoogleTest)
#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  GIT_REPOSITORY https://github.com/google/googletest.git
#  GIT_TAG release-1.12.1
#)
#FetchContent_MakeAvailable(googletest)

add_executable(
  picolator_test
  test_expr_tree.cpp
  test_unary_operator.cpp
  test_literals.cpp
)

target_link_libraries(
  picolator_test
  GTest::GTest GTest::Main
  picolator_objlib
)