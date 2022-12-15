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

#include <string>
#include <utility>
#include <vector>

void lcd_clear(void);
void lcd_set_cursor(int line, int position);
void lcd_string(const char* s);
void lcd_init();
void lcd_char(char val);

const int MAX_LINES = 2;
const int MAX_CHARS = 16;

// TODO update so it can handle custom chars
class LCD1602 {
 private:
  // Holds the info of the entire screen
  std::vector<std::string> screen_buffer_ = {};

  // What will actually be displayed on the screen
  int screen_[MAX_LINES][MAX_CHARS] = {' '};

  int screen_y_ = 0;
  int screen_x_ = 0;

  // where input will be placed
  int cursor_x_ = 0;
  int cursor_y_ = 0;

 public:
  LCD1602();

  // Place char or string wherever the cursor is
  void put(char val, bool scroll = false);
  void put(const std::string& string, bool scroll = false);

  // Inserts char or string wherever the cursor is (doesn't overwrite was is
  // there)
  void insert(char val, bool scroll = false);
  void insert(const std::string&, bool scroll = false);

  // Changes the view to a new y and x (0,0) is the top
  // Not this cant go negative
  void setView(int y, int x) {
    screen_x_ = x;
    screen_y_ = y;
  }

  std::pair<int, int> getView() { return std::make_pair(screen_y_, screen_x_); }

  // Scrolls a delta-y and delta-x can't go into the negatives
  // scrolls cursor will only move screen if cursor would go off the screen
  void scroll(int dy, int dx) {
    screen_y_ += dy;
    screen_x_ += dx;
  }

  // Clears everything on the screen
  void clear();
  void clear(int y);

  /* updates the screen_ and displays changes to the lcd module*/
  void update();

  void setCursor(int y, int x) {
    cursor_x_ = x;
    cursor_y_ = y;
    while (screen_buffer_.size() - 1 < cursor_y_) {
      screen_buffer_.emplace_back("");
    }

    if (screen_x_ > screen_buffer_[cursor_y_].length()) {
      screen_buffer_[cursor_y_].append(
          screen_buffer_[cursor_y_].length() - cursor_x_, ' ');
    }
  }

  bool moveCursor(int dy, int dx, bool scroll = true) {
    if (cursor_x_ + dx < 0 || cursor_y_ + dy < 0) {
      return false;
    }

    // valid move todo this check should be done by client not us
    setCursor(cursor_y_ + dy, cursor_x_ + dx);
    if (scroll) {
      printf("cursor x %d screen x %d\n", cursor_x_, screen_x_);
      if (cursor_x_ < screen_x_) {
        screen_x_ = cursor_x_;
      } else if (cursor_x_ >= screen_x_ + MAX_CHARS) {
        screen_x_ = cursor_x_ - MAX_CHARS + 1;
      }

      if (cursor_y_ < screen_y_) {
        screen_y_ = cursor_y_;
      } else if (cursor_y_ >= screen_y_ + MAX_LINES) {
        screen_y_ = cursor_y_ - MAX_LINES + 1;
      }
    }
    return true;
  }

  void createChar(uint8_t location, uint8_t charmap[]);
};
