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
#include <larrys_pico/button_matrix.h>

#include <cmath>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "bsp/board.h"
#include "calculator_state.h"
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

#define MATRIX_ROW_SIZE 5
#define MATRIX_COL_SIZE 9

using picolator::math::BinaryOperator;
using picolator::math::Bracket;
using picolator::math::DomainError;
using picolator::math::ExprTree;
using picolator::math::Function;
using picolator::math::Letter;
using picolator::math::Literals;
using picolator::math::LiteralsPiece;
using picolator::math::SyntaxError;
using picolator::math::UnaryOperator;
using LP = ExprTree::LetterPtr;

int cursorIndexToLcdIndex(const CalculatorState& state) {
  int lcd_cursor = 0;
  int count = 0;
  for (auto& e : state.equation) {
    if (count++ < state.cursor) {
      lcd_cursor += e->getValue().length();
    } else {
      break;
    }
  }
  return lcd_cursor;
}

void reflash_callback(CalculatorState& state) {
  state.lcd.clear();
  state.lcd.setCursor(0, 0);
  state.lcd.put("re-flash mode");
  state.lcd.setCursor(1, 0);
  state.lcd.put("unplug battery!");
  state.lcd.update();

  reset_usb_boot(0, 0);
}
LP reflash(new Function(reflash_callback));

// No Operation just a placeholder for blank keys
void noOp(CalculatorState&){};
LP noop(new Function(noOp));

// Moves cursors 1 to the left
void moveLeft(CalculatorState& state) {}

// Calculates the result and prints it on
// the screen. or display error in case of error
void calculate_cb(CalculatorState& state) {
  float value = 0;
  try {
    auto tree = ExprTree(state.equation);
    value = tree.getValue();

  } catch (const DomainError& e) {
    state.lcd.setCursor(1, 0);
    state.lcd.put(e.what());
    state.lcd.setView(0, 0);
    state.lcd.setCursor(0, 0);
    state.lcd.update();
    return;
  } catch (const SyntaxError& e) {
    state.lcd.setCursor(1, 0);
    state.lcd.put(e.what());
    state.lcd.setView(0, 0);
    state.lcd.setCursor(0, cursorIndexToLcdIndex(state));

    state.lcd.update();
    return;
  }
  // state.lcd_clear();
  state.lcd.setCursor(1, 0);
  state.lcd.put("ans " + std::to_string(value));
  state.lcd.setCursor(0, 0);
  state.lcd.setView(0, 0);
  state.lcd.update();
  state.cursor = 0;

  // Save the equation in the history
  state.history.push_back(state.equation);
  state.equation.clear();
}
LP calc(new Function(calculate_cb));

// Clears the screen and the result
void clear_cb(CalculatorState& state) {
  state.equation.clear();
  state.cursor = 0;
  state.lcd.clear();
  state.lcd.update();
}
LP clear(new Function(clear_cb));

void backspace_cb(CalculatorState& state) {
  if (state.equation.size() == 1) {
    clear_cb(state);
    return;
  }
  state.equation.erase(state.equation.begin() + --state.cursor);
  state.lcd.setCursor(0, 0);
  int lcd_cursor = 0;
  int count = 0;
  state.lcd.clear();
  for (auto& e : state.equation) state.lcd.put(e->getValue());

  state.lcd.setCursor(0, cursorIndexToLcdIndex(state));
  state.lcd.update();
}
LP backspace(new Function(backspace_cb));

// Operators
// Binary ops
LP op_div(new BinaryOperator("/", BinaryOperator::Type::DIVISION));
LP op_add(new BinaryOperator("+", BinaryOperator::Type::ADDITION));
LP op_sub(new BinaryOperator("-", BinaryOperator::Type::SUBTRACTION));
LP op_mul(new BinaryOperator("*", BinaryOperator::Type::MULTIPLICATION));
// LP op_mod(new BinaryOperator("%", BinaryOperator::Type::MODULUS));
// LP op_exp(new BinaryOperator("^", BinaryOperator::Type::EXPONENT));

// Unary ops
LP op_minus(new UnaryOperator("-", UnaryOperator::Type::MINUS));
LP op_sin(new UnaryOperator("sin", UnaryOperator::Type::SIN));
LP op_cos(new UnaryOperator("cos", UnaryOperator::Type::COS));
LP op_tan(new UnaryOperator("tan", UnaryOperator::Type::TAN));

// Numbers
#define LITP(a) (LP(new LiteralsPiece(a)))

// Literals
LP l_pi(new Literals(Literals::Type::PI));
LP l_e(new Literals(Literals::Type::E));

// Brackets
LP b_open(new Bracket(Bracket::Type::OPEN));
LP b_clos(new Bracket(Bracket::Type::CLOSED));

LP button_mapping2[MATRIX_COL_SIZE][MATRIX_ROW_SIZE]{
    {noop, noop, noop, backspace, reflash},
    {noop, noop, noop, noop, noop},
    {noop, noop, noop, noop, noop},
    {l_pi, l_e, op_sin, op_cos, op_tan},
    {noop, noop, b_open, b_clos, op_div},
    {noop, LITP('7'), LITP('8'), LITP('9'), op_sub},
    {noop, LITP('4'), LITP('5'), LITP('6'), op_mul},
    {noop, LITP('1'), LITP('2'), LITP('3'), op_add},
    {clear, LITP('0'), LITP('.'), op_minus, calc}};
// todo change to a math mapping
char button_mapping[MATRIX_COL_SIZE][MATRIX_ROW_SIZE]{
    {'N', 'N', 'N', '<', 'F'}, {'U', 'L', 'R', 'A', 'B'},
    {'N', 'D', 'x', 'y', 'z'}, {'p', 'e', 's', 'c', '0'},
    {'2', '-', '(', ')', '/'}, {'^', '7', '8', '9', '-'},
    {'<', '4', '5', '6', '*'}, {'X', '1', '2', '3', '+'},
    {'X', '0', '.', '-', '='}

};

// smile
uint8_t smile[] = {0x00, 0x00, 0x0A, 0x00, 0x11, 0x0E, 0x00, 0x00};

int main() {
  stdio_init_all();

  ButtonMatrix<MATRIX_ROW_SIZE, MATRIX_COL_SIZE> buttons = {
      {15, 11, 14, 13, 12}, {27, 26, 22, 21, 20, 19, 18, 17, 16}};

  CalculatorState state;

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

  int cursor = 0;
  // state.equation.clear();
  while (1) {
    sleep_ms(10);
    auto but = buttons.getPressed(true);

    if (but) {
      // state.lcd.clear();
      // state.lcd.put(std::to_string(but->first) + " " +
      // std::to_string(but->second)); state.lcd.update();

      const auto& mapping = button_mapping2[but->second][but->first];
      switch (mapping->getClassification()) {
        case Letter::Classification::FUNCTION:
          reinterpret_cast<Function&>(*mapping).invoke(state);
          break;
        case Letter::Classification::UNARY: {
          auto op = reinterpret_cast<UnaryOperator&>(*mapping);
          if (op.getOp() != UnaryOperator::Type::MINUS) {
            state.equation.emplace_back(mapping);
            state.equation.emplace_back(b_open);

            state.lcd.put(mapping->getValue(), true);
            state.lcd.put(b_open->getValue(), true);

            state.lcd.update();
            state.cursor += 2;
            break;
          }
        }  // fall through
        default:
          state.equation.emplace_back(mapping);
          state.lcd.put(mapping->getValue(), true);
          state.lcd.update();
          state.cursor += 1;
      }
    }
  }
  return 0;
}
