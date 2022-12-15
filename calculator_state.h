/*
 * (C) Copyright 2022 Larry Milne (https://www.larrycloud.ca)
 *
 * This code is distributed on "AS IS" BASIS,
 * WITHOUT WARRANTINES OR CONDITIONS OF ANY KIND.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author: qawse3dr a.k.a Larry Milne
 */
#pragma once

#include <vector>

// #include "larrys_pico/LCD1602.h"
#include "math/expr_tree.h"
class LCD1602;

struct CalculatorState {
  // Holds a pointer to the current equation (should be index 0)
  // might be able to remove
  std::vector<picolator::math::ExprTree::ExprVec> history;
  int history_cursor;
  picolator::math::Literals ans = {0};

  // Holds all the equations in memory
  picolator::math::ExprTree::ExprVec equation;
  int cursor;

  // Hardware functions might make use of
  LCD1602 lcd;
};
