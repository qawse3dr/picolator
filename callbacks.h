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

#include "calculator_state.h"
#include "math/expr_tree.h"

// Util for callback functions
int cursorIndexToLcdIndex(const CalculatorState& state);
void redrawEquation(CalculatorState& state);
void copyExprVec(const picolator::math::ExprTree::ExprVec& src,
                 picolator::math::ExprTree::ExprVec& dst);

// Callbacks
void noOp(CalculatorState&);
void reflash_cb(CalculatorState& state);
void moveLeft_cb(CalculatorState& state);
void moveRight_cb(CalculatorState& state);
void moveUp_cb(CalculatorState& state);
void moveDown_cb(CalculatorState& state);
void calculate_cb(CalculatorState& state);
void clear_cb(CalculatorState& state);
void backspace_cb(CalculatorState& state);
void convertDouble_cb(CalculatorState& state);
void layer2_cb(CalculatorState& state);