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
#include "larrys_pico/button.h"
#include "larrys_pico/button_matrix.h"
#include "math/binary_operator.h"
#include "math/bracket.h"
#include "math/funciton.h"
#include "math/literals.h"
#include "math/unary_operator.h"
#include "pico/binary_info.h"
#include "pico/bootrom.h"
#include "pico/multicore.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

#define MATRIX_ROW_SIZE 5
#define MATRIX_COL_SIZE 9

// void reflash(CalculatorState& state) {}

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

  sleep_ms(1000);
  printf("hello\n");

  // ButtonMatrix<MATRIX_ROW_SIZE, MATRIX_COL_SIZE> buttons = {
  // {15, 11, 14, 13, 12}, {27, 26, 22, 21, 20, 19, 18, 17, 16}};

  while (1) {
    sleep_ms(10);
    printf("hello\n");
  }
  // CalculatorState state;

  // state.lcd.createChar(1, smile);

  // state.lcd.clear();
  // state.lcd.setCursor(0, 3);
  // state.lcd.put(1);
  // state.lcd.put("Picolator");
  // state.lcd.put(1);
  // state.lcd.update();

  // sleep_ms(500);

  // // add custom chars
  // state.lcd.clear();
  // state.lcd.update();

  // state.cursor = 0;
  // state.equation.clear();
  // while (1) {
  //   sleep_ms(10);
  //   auto but = buttons.getPressed(true);

  //   if (but) {
  //     // state.lcd.clear();
  //     // state.lcd.put(std::to_string(but->first) + " " +
  //     // std::to_string(but->second)); state.lcd.update();

  //     // reflash button for when i
  //     //  am dev TODO remove and make a dedicated one
  //     if (but->first == 4 && but->second == 0) {
  //       state.lcd.clear();
  //       state.lcd.setCursor(0, 0);
  //       state.lcd.put("re-flash mode");
  //       state.lcd.setCursor(1, 0);
  //       state.lcd.put("unplug battery!");
  //       state.lcd.update();

  //       reset_usb_boot(0, 0);
  //     }
  //     switch (button_mapping[but->second][but->first]) {
  //       case 'N':
  //         break;   // do nothing
  //       case 'L':  // directions
  //         if (state.lcd.moveCursor(0, -1)) {
  //           state.cursor--;
  //           state.lcd.update();
  //         }
  //         break;
  //       case 'R':
  //         // // printf("equation len %d, cursor %d\n", equation.length(),
  //         cursor
  //         // +
  //         //     // 1);
  //         if (state.cursor < state.equation.size()) {
  //           state.lcd.moveCursor(0, 1);
  //           state.cursor++;
  //           state.lcd.update();
  //         }
  //         break;

  //       case 'D':
  //       case 'U':
  //         break;
  //       case '=':
  //         state.cursor = 0;
  //         // state.lcd_clear();
  //         state.lcd.setCursor(1, 0);
  //         state.lcd.put("ans 5");
  //         state.lcd.setCursor(0, 0);
  //         state.lcd.setView(0, 0);
  //         state.lcd.update();
  //         state.equation.clear();

  //         break;
  //       case 'X':
  //         state.cursor = 0;
  //         state.lcd.clear();
  //         state.lcd.update();
  //         state.equation.clear();
  //         break;
  //       case '<':
  //         if (state.cursor > 0) {
  //           state.lcd.setCursor(0, --state.cursor);
  //           state.lcd.put(' ');
  //           state.lcd.setCursor(0, state.cursor);
  //           // state.equation.erase(equation.begin() + state.cursor);
  //           state.lcd.update();
  //         }
  //         break;
  //       default:
  //         state.lcd.put(button_mapping[but->second][but->first], true);
  //         state.lcd.update();
  //         // state.equation.append(1,
  //         // button_mapping[but->second][but->first]);
  //         // printf("equation: %s\n", equation.c_str());
  //         state.cursor++;
  //         break;
  //     }
  //   }
  // }
  return 0;
}
