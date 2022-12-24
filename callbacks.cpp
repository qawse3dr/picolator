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
#include "callbacks.h"

#include "math/binary_operator.h"
#include "math/bracket.h"
#include "math/function.h"
#include "math/literals.h"
#include "math/literals_piece.h"
#include "math/math_util.h"
#include "math/unary_operator.h"
#include "pico/bootrom.h"

using picolator::math::BinaryOperator;
using picolator::math::Bracket;
using picolator::math::DomainError;
using picolator::math::ExprTree;
using picolator::math::Letter;
using picolator::math::Literals;
using picolator::math::LiteralsPiece;
using picolator::math::NotImplementedError;
using picolator::math::SyntaxError;
using picolator::math::TypeError;
using picolator::math::UnaryOperator;
using LP = ExprTree::LetterPtr;

int cursorIndexToLcdIndex(const CalculatorState& state) {
  int lcd_cursor = 0;
  int count = 0;
  for (auto& e : state.equation) {
    if (count++ < state.cursor) {
      lcd_cursor += e->getSymbol().length();
    } else {
      break;
    }
  }
  return lcd_cursor;
}

void redrawEquation(CalculatorState& state) {
  state.lcd.clear(0);
  state.lcd.setCursor(0, 0);
  for (const auto& l : state.equation) {
    state.lcd.put(l->getSymbol());
  }
  state.lcd.setCursor(0, cursorIndexToLcdIndex(state));
  state.lcd.update();
}

void copyExprVec(const ExprTree::ExprVec& src, ExprTree::ExprVec& dst) {
  dst.clear();
  for (const auto& l : src) {
    dst.emplace_back(l);
  }
}

void insertEquation(CalculatorState& state,
                    const ExprTree::LetterPtr& mapping) {
  if (state.cursor == state.equation.size()) {
    state.equation.emplace_back(mapping);
    state.lcd.put(mapping->getSymbol(), true);
    state.cursor += 1;
  } else {
    if (state.insert_mode) {
      state.equation.insert(state.equation.begin() + state.cursor, mapping);
    } else {
      state.equation[state.cursor] = mapping;
    }
    state.cursor += 1;
  }
  redrawEquation(state);
}

void reflash_cb(CalculatorState& state) {
  state.lcd.clear();
  state.lcd.setCursor(0, 0);
  state.lcd.put("re-flash mode");
  state.lcd.setCursor(1, 0);
  state.lcd.put("unplug battery!");
  state.lcd.update();

  reset_usb_boot(0, 0);
}

void moveLeft_cb(CalculatorState& state) {
  if (state.cursor > 0) {
    state.cursor -= 1;
    state.lcd.setCursor(0, cursorIndexToLcdIndex(state));
    state.lcd.update();
  }
}

void moveRight_cb(CalculatorState& state) {
  if (state.cursor < state.equation.size()) {
    state.cursor += 1;
  } else if (state.cleared) {  // replace equation
    copyExprVec(state.history.back(), state.equation);
    // state.equation = state.history.back();
    state.cursor = 1;
  }
  redrawEquation(state);
}

void moveUp_cb(CalculatorState& state) {
  printf("moveUp pressed\n");
  if (state.history_cursor < state.history.size()) {
    copyExprVec(state.history[state.history.size() - ++state.history_cursor],
                state.equation);
    redrawEquation(state);
  }
}

void moveDown_cb(CalculatorState& state) {
  printf("moveDown pressed\n");
  if (state.history_cursor > 1) {
    copyExprVec(state.history[state.history.size() - --state.history_cursor],
                state.equation);
    redrawEquation(state);
  }
}

// Calculates the result and prints it on
// the screen. or display error in case of error
void calculate_cb(CalculatorState& state) {
  state.lcd.clear(1);
  Literals::getAnswer() = state.ans->getLiteral();
  if (state.equation.empty()) {
    if (state.history.empty()) return;
    state.equation = state.history.back();
    redrawEquation(state);
  }
  ExprTree::LiteralPtr value = 0;
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
  } catch (const TypeError& e) {
    state.lcd.setCursor(1, 0);
    state.lcd.put(e.what());
    state.lcd.setView(0, 0);
    state.lcd.setCursor(0, 0);
    state.lcd.update();
    return;
  } catch (const NotImplementedError& e) {
    state.lcd.setCursor(1, 0);
    state.lcd.put(e.what());
    state.lcd.setView(0, 0);
    state.lcd.setCursor(0, 0);
    state.lcd.update();
  } catch (std::exception& e) {
    state.lcd.setCursor(1, 0);
    state.lcd.put("caught error");
    state.lcd.setView(0, 0);
    state.lcd.setCursor(0, 0);
    state.lcd.update();
    return;
  }

  state.clear = true;
  state.lcd.setCursor(1, 0);
  state.lcd.put("\x7E" + value->toString());
  state.lcd.setCursor(0, 0);
  state.lcd.setView(0, 0);
  state.lcd.update();
  state.cursor = 0;
  state.history_cursor = 0;

  // Save the equation in the history
  state.history.push_back(state.equation);
  state.equation.clear();

  state.ans = value;
}

// Clears the screen and the result
void clear_cb(CalculatorState& state) {
  state.equation.clear();
  state.cursor = 0;
  state.lcd.clear();
  state.lcd.update();
}

void backspace_cb(CalculatorState& state) {
  if (state.equation.size() < 1) {
    clear_cb(state);
    return;
  }
  state.equation.erase(state.equation.begin() + --state.cursor);
  state.lcd.setCursor(0, 0);
  redrawEquation(state);
}

void convertDouble_cb(CalculatorState& state) {
  if (!state.ans) state.ans = 0;
  state.lcd.clear();
  state.lcd.setCursor(0, 0);
  state.lcd.put("ANS \x7E DOUBLE");
  state.lcd.setCursor(1, 0);
  state.lcd.put("\x7E" + std::to_string(state.ans->getValue()));
  state.lcd.update();
}

static char selectVar(CalculatorState& state) {
  int cursor = 0;
  state.lcd.clear();
  state.lcd.setCursor(0, 0);
  state.lcd.put("A B C D E F");
  state.lcd.setCursor(1, 0);
  state.lcd.put("\x7E" + Literals::getVariable('A').getSymbol());
  state.lcd.setCursor(0, 0);
  state.lcd.update();

  while (1) {
    sleep_ms(10);
    auto but = state.buttons.getPressed(true);
    if (!but) continue;
    if (but->second == 8 && but->first == 4) break;
    if (but->second == 1 && but->first == 2 && cursor < 6) {
      cursor++;
      state.lcd.setCursor(1, 0);
      state.lcd.clear(1);

      state.lcd.put("\x7E" + Literals::getVariable('A' + cursor).getSymbol());
      state.lcd.setCursor(0, cursor * 2);

      state.lcd.update();
    }
    if (but->second == 1 && but->first == 1 && cursor > 0) {
      cursor--;
      state.lcd.setCursor(1, 0);
      state.lcd.clear(1);
      state.lcd.put("\x7E" + Literals::getVariable('A' + cursor).getSymbol());
      state.lcd.setCursor(0, cursor * 2);

      state.lcd.update();
    }
  }
  return 'A' + cursor;
}

void saveVar_cb(CalculatorState& state) {
  if (!state.ans) state.ans = 0;
  char var = selectVar(state);
  Literals::getVariable(var) = state.ans->getLiteral();
  state.lcd.clear();
  state.lcd.setCursor(0, 0);
  state.lcd.put("ANS \x7E " + std::string(1, var));
  state.lcd.setCursor(1, 0);
  state.lcd.put("\x7E" + Literals::getVariable(var).toString());
  state.lcd.setCursor(0, 0);
  state.lcd.update();
}
void getVar_cb(CalculatorState& state) {
  if (!state.ans) state.ans = 0;
  char var = selectVar(state);
  insertEquation(state, LP(new Literals(var)));
  redrawEquation(state);
}

void noOp(CalculatorState&){};

void layer2_cb(CalculatorState& state) { state.layer2 = true; }