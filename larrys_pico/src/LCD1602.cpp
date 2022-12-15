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

#include "larrys_pico/LCD1602.h"

#include <string.h>

#include <cstring>

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

/**
 * I Did not create the rest of the code
 * all credit goes to raspberry pi LTD
 */

/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Example code to drive a 16x2 LCD panel via a I2C bridge chip (e.g.
   PCF8574) NOTE: The panel must be capable of being driven at 3.3v NOT 5v.
   The Pico GPIO (and therefor I2C) cannot be used at 5v. You will need to
   use a level shifter on the I2C lines if you want to run the board at 5v.
   Connections on Raspberry Pi Pico board, other boards may vary.
   GPIO 4 (pin 6)-> SDA on LCD bridge board
   GPIO 5 (pin 7)-> SCL on LCD bridge board
   3.3v (pin 36) -> VCC on LCD bridge board
   GND (pin 38)  -> GND on LCD bridge board
*/
// commands
const int LCD_CLEARDISPLAY = 0x01;
const int LCD_RETURNHOME = 0x02;
const int LCD_ENTRYMODESET = 0x04;
const int LCD_DISPLAYCONTROL = 0x08;
const int LCD_CURSORSHIFT = 0x10;
const int LCD_FUNCTIONSET = 0x20;
const int LCD_SETCGRAMADDR = 0x40;
const int LCD_SETDDRAMADDR = 0x80;

// flags for display entry mode
const int LCD_ENTRYSHIFTINCREMENT = 0x01;
const int LCD_ENTRYLEFT = 0x02;

// flags for display and cursor control
const int LCD_BLINKON = 0x01;
const int LCD_CURSORON = 0x02;
const int LCD_DISPLAYON = 0x04;

// flags for display and cursor shift
const int LCD_MOVERIGHT = 0x04;
const int LCD_DISPLAYMOVE = 0x08;

// flags for function set
const int LCD_5x10DOTS = 0x04;
const int LCD_2LINE = 0x08;
const int LCD_8BITMODE = 0x10;

// flag for backlight control
const int LCD_BACKLIGHT = 0x08;

const int LCD_ENABLE_BIT = 0x04;

// By default these LCD display drivers are on bus address 0x27
const int addr = 0x27;

// Modes for lcd_send_byte
#define LCD_CHARACTER 1
#define LCD_COMMAND 0

#define MAX_LINES 2
#define MAX_CHARS 16

/* Quick helper function for single byte transfers */
void i2c_write_byte(uint8_t val) {
#ifdef i2c_default
  i2c_write_blocking(i2c_default, addr, &val, 1, false);
#endif
}

void lcd_toggle_enable(uint8_t val) {
  // Toggle enable pin on LCD display
  // We cannot do this too quickly or things don't work
#define DELAY_US 600
  sleep_us(DELAY_US);
  i2c_write_byte(val | LCD_ENABLE_BIT);
  sleep_us(DELAY_US);
  i2c_write_byte(val & ~LCD_ENABLE_BIT);
  sleep_us(DELAY_US);
}

// The display is sent a byte as two separate nibble transfers
void lcd_send_byte(uint8_t val, int mode) {
  uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
  uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

  i2c_write_byte(high);
  lcd_toggle_enable(high);
  i2c_write_byte(low);
  lcd_toggle_enable(low);
}

void lcd_clear(void) { lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND); }

// go to location on LCD
void lcd_set_cursor(int line, int position) {
  int val = (line == 0) ? 0x80 + position : 0xC0 + position;
  lcd_send_byte(val, LCD_COMMAND);
}

void lcd_char(char val) { lcd_send_byte(val, LCD_CHARACTER); }

void lcd_string(const char* s) {
  while (*s) {
    lcd_char(*s++);
  }
}

void lcd_init() {
  // This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
  i2c_init(i2c_default, 100 * 1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
  // Make the I2C pins available to picotool
  bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN,
                             GPIO_FUNC_I2C));

  lcd_send_byte(0x03, LCD_COMMAND);
  lcd_send_byte(0x03, LCD_COMMAND);
  lcd_send_byte(0x03, LCD_COMMAND);
  lcd_send_byte(0x02, LCD_COMMAND);

  lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
  lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
  lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_BLINKON, LCD_COMMAND);
  lcd_clear();
}

/**
 * Starting here is code I wrote
 *
 */

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LCD1602::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7;  // we only have 8 locations 0-7
  lcd_send_byte(LCD_SETCGRAMADDR | (location << 3), LCD_COMMAND);
  for (int i = 0; i < 8; i++) {
    lcd_send_byte(charmap[i], LCD_CHARACTER);
  }
}

LCD1602::LCD1602() {
  i2c_init(i2c_default, 100 * 1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

  lcd_init();
}

void LCD1602::put(char val, bool scroll) {
  // check if put would go off the screen by one and if it does scroll in
  // that direction

  // newline
  if (val == '\n') {
    if (scroll && cursor_y_ + 1 == screen_y_) {
      screen_y_++;
      cursor_x_ = 0;
      // need to scroll down for view
    }
    cursor_y_++;
    // insert a new line
    screen_buffer_.insert(screen_buffer_.begin() + screen_y_, "");

  } else {
    auto& row = screen_buffer_[cursor_y_];
    while (row.length() <= cursor_x_) {
      row += ' ';
    }
    row[cursor_x_++] = val;

    if (scroll && cursor_x_ - MAX_CHARS >= screen_x_) {
      screen_x_++;
    }
  }
}

void LCD1602::put(const std::string& string, bool scroll) {
  for (auto c : string) {
    put(c, scroll);
  }
}

void LCD1602::clear() {
  screen_y_ = 0;
  screen_x_ = 0;
  cursor_x_ = 0;
  cursor_y_ = 0;
  memset(screen_, ' ', MAX_LINES * MAX_CHARS);
  screen_buffer_.clear();
  screen_buffer_.emplace_back(std::string(MAX_CHARS, ' '));
  screen_buffer_.emplace_back(std::string(MAX_CHARS, ' '));
}

void LCD1602::update() {
  for (int i = 0; i < MAX_LINES && (i + screen_y_ < screen_buffer_.size());
       i++) {
    printf("%s\n", screen_buffer_[i].c_str());
    for (int j = 0; j < MAX_CHARS; j++) {
      if (j + screen_x_ < screen_buffer_[i + screen_y_].length()) {
        if (screen_buffer_[i + screen_y_][j + screen_x_] != screen_[i][j]) {
          screen_[i][j] = screen_buffer_[i + screen_y_][j + screen_x_];
          lcd_set_cursor(i, j);
          lcd_char(screen_[i][j]);
        }

      } else if (screen_[i][j] != ' ') {  // check to make sure its zero'd out
        lcd_set_cursor(i, j);
        lcd_char(' ');
      }
    }
  }
  // set cursor to the correct position
  lcd_set_cursor(cursor_y_ - screen_y_, cursor_x_ - screen_x_);
  printf("\n");
}

void LCD1602::insert(char val, bool scroll) {
  if (val == '\n') {
    // insert should not be used for newline if it is use
    // put instead
    put(val, scroll);
    return;
  }

  auto& row = screen_buffer_[cursor_y_];
  if (row.length() < cursor_x_) {
    row.append(cursor_x_ - row.length(), ' ');
  }
  row[cursor_x_] = val;
  if (scroll && cursor_x_ + MAX_CHARS - 1 == screen_x_) {
    screen_x_++;
  }
  cursor_x_++;
}

void LCD1602::insert(const std::string& string, bool scroll) {
  for (auto c : string) {
    insert(c, scroll);
  }
}
