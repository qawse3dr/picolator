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

#include "button.h"
#include "hardware/gpio.h"

Gpio::Gpio(int gpio_pin) : gpio_pin_(gpio_pin) {
  if (gpio_pin != -1) gpio_init(gpio_pin_);
}

Gpio::~Gpio() {}

GpioInputButton::GpioInputButton(int gpio_pin, bool pull_up)
    : Gpio(gpio_pin), pull_up_(pull_up) {
  gpio_set_dir(gpio_pin_, GPIO_IN);
  if (pull_up) {
    gpio_pull_up(gpio_pin);
  }
}

bool GpioInputButton::pressed() {
  return (pull_up_) ? !gpio_get(gpio_pin_) : gpio_get(gpio_pin_);
}

GpioOutput::GpioOutput(int gpio_pin, bool start_state)
    : Gpio(gpio_pin), state_(start_state) {
  gpio_set_dir(gpio_pin_, GPIO_OUT);
  put(start_state);
}

bool GpioOutput::put(bool state) {
  state_ = state;
  gpio_put(gpio_pin_, state_);
  return state_;
}
bool GpioOutput::toggle() { return put(!state_); }
