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

#include <larrys_pico/LCD1602.h>
#include <larrys_pico/button.h>

#include <cmath>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <vector>

// needed for stdout
// #include "bsp/board.h"
#include "calculator_state.h"
#include "callbacks.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "math/binary_operator.h"
#include "math/bracket.h"
#include "math/function.h"
#include "math/literals.h"
#include "math/literals_piece.h"
#include "math/math_util.h"
#include "math/unary_operator.h"
#include "pico/binary_info.h"
#include "pico/bootrom.h"
#include "pico/multicore.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

using picolator::math::BinaryOperator;
using picolator::math::Bracket;
using picolator::math::ExprTree;
using picolator::math::Function;
using picolator::math::Letter;
using picolator::math::Literals;
using picolator::math::LiteralsPiece;
using picolator::math::UnaryOperator;

using LP = ExprTree::LetterPtr;

LP reflash(new Function(reflash_cb));

// No Operation just a placeholder for blank keys
LP noop(new Function(noOp));

LP moveLeft(new Function(moveLeft_cb));
LP moveRight(new Function(moveRight_cb));
LP moveUp(new Function(moveUp_cb));
LP moveDown(new Function(moveDown_cb));
LP calc(new Function(calculate_cb));
LP clear(new Function(clear_cb));
LP backspace(new Function(backspace_cb));
LP convertDouble(new Function(convertDouble_cb));
LP saveVar(new Function(saveVar_cb));
LP getVar(new Function(getVar_cb));

LP layer2(new Function(layer2_cb));

// Operators
// Binary ops
LP op_div(new BinaryOperator("/", BinaryOperator::Type::DIVISION));
LP op_add(new BinaryOperator("+", BinaryOperator::Type::ADDITION));
LP op_sub(new BinaryOperator("-", BinaryOperator::Type::SUBTRACTION));
LP op_mul(new BinaryOperator("*", BinaryOperator::Type::MULTIPLICATION));
LP op_mod(new BinaryOperator("%", BinaryOperator::Type::MODULUS));
LP op_exp(new BinaryOperator("^", BinaryOperator::Type::EXPONENT));
LP op_n_sqrt(new BinaryOperator("^\xE8", BinaryOperator::Type::N_TH_ROOT));

// Unary ops
LP op_minus(new UnaryOperator("-", UnaryOperator::Type::MINUS));
LP op_sin(new UnaryOperator("sin", UnaryOperator::Type::SIN));
LP op_cos(new UnaryOperator("cos", UnaryOperator::Type::COS));
LP op_tan(new UnaryOperator("tan", UnaryOperator::Type::TAN));
LP op_asin(new UnaryOperator("asin", UnaryOperator::Type::ARCSIN));
LP op_acos(new UnaryOperator("acos", UnaryOperator::Type::ARCCOS));
LP op_atan(new UnaryOperator("atan", UnaryOperator::Type::ARCTAN));
LP op_ln(new UnaryOperator("ln", UnaryOperator::Type::LN));
LP op_sqrt(new UnaryOperator("\xE8", UnaryOperator::Type::SQUARE_ROOT));

// Numbers
#define LITP(a) (LP(new LiteralsPiece(a)))

// Literals
LP l_pi(new Literals(Literals::Type::PI));
LP l_e(new Literals(Literals::Type::E));
LP ANS(new Literals(Literals::Type::ANS));

// Brackets
LP b_open(new Bracket(Bracket::Type::OPEN));
LP b_clos(new Bracket(Bracket::Type::CLOSED));

LP button_mapping[MATRIX_COL_SIZE][MATRIX_ROW_SIZE]{
    {noop, moveUp, noop, layer2, reflash},
    {noop, moveLeft, moveRight, noop, noop},
    {convertDouble, moveDown, noop, noop, noop},
    {l_pi, l_e, op_sin, op_cos, op_tan},
    {op_exp, op_mod, b_open, b_clos, op_div},
    {op_sqrt, LITP('7'), LITP('8'), LITP('9'), op_mul},
    {getVar, LITP('4'), LITP('5'), LITP('6'), op_sub},
    {backspace, LITP('1'), LITP('2'), LITP('3'), op_add},
    {clear, LITP('0'), LITP('.'), op_minus, calc}};

// null means fallback to button_mapping
LP button_mapping2[MATRIX_COL_SIZE][MATRIX_ROW_SIZE]{
    {nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, op_ln, op_asin, op_acos, op_atan},
    {nullptr, nullptr, nullptr, nullptr, nullptr},
    {op_n_sqrt, nullptr, nullptr, nullptr, nullptr},
    {saveVar, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, nullptr, ANS, nullptr}};

// smile
uint8_t smile[] = {0x00, 0x00, 0x0A, 0x00, 0x11, 0x0E, 0x00, 0x00};

int main() {
  CalculatorState state;

  sleep_ms(200);
  state.lcd.createChar(1, smile);
  state.lcd.clear();
  state.lcd.setCursor(0, 3);
  state.lcd.put(1);
  state.lcd.put("Picolator");
  state.lcd.put(1);
  state.lcd.update();

  sleep_ms(500);

  // add custom chars
  state.lcd.clear();
  state.lcd.update();

  state.cursor = 0;
  state.equation.clear();
  while (1) {
    sleep_ms(10);
    auto but = state.buttons.getPressed(true);

    if (but) {
      // state.lcd.clear();
      // state.lcd.put(std::to_string(but->first) + " " +
      // std::to_string(but->second)); state.lcd.update();

      // Clear on next button press if the flag is set
      if (state.clear) {
        state.lcd.clear();
        state.clear = false;
        state.cleared = true;
      }

      // dont use ref or
      auto mapping = button_mapping[but->second][but->first];

      // if second layer is active change layer
      if (state.layer2 == true) {
        const auto& mapping2 = button_mapping2[but->second][but->first];
        if (mapping2) {
          mapping = button_mapping2[but->second][but->first];
        }
        state.layer2 = false;
      }

      switch (mapping->getClassification()) {
        case Letter::Classification::FUNCTION:
          reinterpret_cast<Function&>(*mapping).invoke(state);
          break;
        case Letter::Classification::UNARY: {
          auto op = reinterpret_cast<UnaryOperator&>(*mapping);
          if (op.getOp() != UnaryOperator::Type::MINUS) {
            state.equation.emplace_back(mapping);
            state.equation.emplace_back(b_open);

            state.lcd.put(mapping->getSymbol(), true);
            state.lcd.put(b_open->getSymbol(), true);

            state.lcd.update();
            state.cursor += 2;
            break;
          }
        }  // fall through
        default:
          insertEquation(state, mapping);
          break;
      }
      // Set the clear flag to false
      state.cleared = false;
    }
  }
  return 0;
}
