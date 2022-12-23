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

#include <larrys_pico/LCD1602.h>
#include <larrys_pico/button_matrix.h>

#include <vector>

#include "math/expr_tree.h"

#define MATRIX_ROW_SIZE 5
#define MATRIX_COL_SIZE 9

struct CalculatorState {
  // Holds a pointer to the current equation (should be index 0)
  // might be able to remove
  std::vector<picolator::math::ExprTree::ExprVec> history;
  int history_cursor;
  picolator::math::ExprTree::LiteralPtr ans = {0};

  // Holds all the equations in memory
  picolator::math::ExprTree::ExprVec equation;
  int cursor;

  // if the screen should get  cleared on next button press
  bool clear = false;
  bool cleared = false;  // If the screen got cleared this frame

  // if the new character should overwrite or insert into place
  bool insert_mode = false;

  // 2nd layer toggle
  bool layer2 = false;

  // Hardware functions might make use of
  LCD1602 lcd;

  ButtonMatrix<MATRIX_ROW_SIZE, MATRIX_COL_SIZE> buttons = {
      {15, 11, 14, 13, 12}, {27, 26, 22, 21, 20, 19, 18, 17, 16}};
};
