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
#include <array>
#include <optional>
#include <utility>

#include "button.h"
#include "pico/stdlib.h"

template <size_t rows, size_t columns>
class ButtonMatrix {
 private:
  std::array<GpioInputButton, columns> buttons_;
  std::array<GpioOutput, rows> rows_;

 public:
  ButtonMatrix(const std::array<uint8_t, rows>& row_pins,
               const std::array<uint8_t, columns>& column_pins) {
    for (int i = 0; i < rows; i++) {
      rows_[i] = GpioOutput(row_pins[i]);
    }

    for (int i = 0; i < columns; i++) {
      buttons_[i] = GpioInputButton(column_pins[i]);
    }
  }

  /**
   * @brief returns a pair if a button is pressed by index in array.
   * @note Returns the index in the array not by pin so the first button in the
   * first row would be <0,0> instead of the pin values
   * @note Only one button can be pressed at once I am not fancy.
   *
   * @param wait_for_unpress will only return after the button is finished being
   * pressed
   * @return std::optional<std::pair<uint8_t, uint8_t>> index of pin being
   * pressed
   */
  std::optional<std::pair<uint8_t, uint8_t>> getPressed(
      bool wait_for_unpress = false) {
    for (uint8_t i = 0; i < rows; i++) {
      rows_[i].toggle();
      sleep_ms(10);
      for (uint8_t j = 0; j < columns; j++) {
        if (buttons_[j].pressed()) {
          // wait for button to be unpressed
          if (wait_for_unpress) {
            while (buttons_[j].pressed()) {
              sleep_ms(10);
            }
          }
          rows_[i].toggle();
          return {std::make_pair(i, j)};
        }
      }
      rows_[i].toggle();
    }
    return {};
  }
};