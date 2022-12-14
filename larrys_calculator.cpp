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
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "larrys_pico/button.h"
#include "larrys_pico/button_matrix.h"
#include "pico/binary_info.h"
#include "pico/bootrom.h"
#include "pico/multicore.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

#define MATRIX_ROW_SIZE 5
#define MATRIX_COL_SIZE 9

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

  LCD1602 lcd;

  lcd.createChar(1, smile);

  lcd.clear();
  lcd.setCursor(0, 3);
  lcd.put(1);
  lcd.put("Picolator");
  lcd.put(1);
  lcd.update();

  sleep_ms(500);

  // add custom chars
  lcd.clear();
  lcd.update();

  int cursor = 0;
  std::string equation = "";
  while (1) {
    sleep_ms(10);
    auto but = buttons.getPressed(true);

    if (but) {
      // lcd.clear();
      // lcd.put(std::to_string(but->first) + " " +
      // std::to_string(but->second)); lcd.update();

      // reflash button for when i
      //  am dev TODO remove and make a dedicated one
      if (but->first == 4 && but->second == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.put("re-flash mode");
        lcd.setCursor(1, 0);
        lcd.put("unplug battery!");
        lcd.update();

        reset_usb_boot(0, 0);
      }
      switch (button_mapping[but->second][but->first]) {
        case 'N':
          break;   // do nothing
        case 'L':  // directions
          if (lcd.moveCursor(0, -1)) {
            cursor--;
            lcd.update();
          }
          break;
        case 'R':
          printf("equation len %d, cursor %d\n", equation.length(), cursor + 1);
          if (cursor < equation.length()) {
            lcd.moveCursor(0, 1);
            cursor++;
            lcd.update();
          }
          break;

        case 'D':
        case 'U':
          break;
        case '=':
          cursor = 0;
          // lcd_clear();
          lcd.setCursor(1, 0);
          lcd.put("ans 5");
          lcd.setCursor(0, 0);
          lcd.setView(0, 0);
          lcd.update();
          equation.clear();

          break;
        case 'X':
          cursor = 0;
          lcd.clear();
          lcd.update();
          equation.clear();
          break;
        case '<':
          if (cursor > 0) {
            lcd.setCursor(0, --cursor);
            lcd.put(' ');
            lcd.setCursor(0, cursor);
            equation.erase(equation.begin() + cursor);
            lcd.update();
          }
          break;
        default:
          lcd.put(button_mapping[but->second][but->first], true);
          lcd.update();
          equation.append(1, button_mapping[but->second][but->first]);
          printf("equation: %s\n", equation.c_str());
          cursor++;
          break;
      }
    }
  }
  return 0;
}
