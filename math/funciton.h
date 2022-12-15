/*
 * (C) Copyright 2022 Larry Milne (https://www.larrycloud.ca)
 *
 * This code is distributed on "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author: qawse3dr a.k.a Larry Milne
 */
#pragma once
#include <functional>

#include "letter.h"

class CalculatorState;

namespace picolator::math {
class Function : public Letter {
 private:
  std::function<void(CalculatorState&)> callback_;

 public:
  Function(std::function<void(CalculatorState&)> callback)
      : Letter("FN", Letter::Classification::FUNCTION, 1),
        callback_(callback) {}

  void invoke(CalculatorState& state) const { callback_(state); }
};
}  // namespace picolator::math