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
#include <cstdint>
#include <string>
#include <vector>

#include "calculator_state.h"

namespace calculator::math {

class Letter {
 public:
  enum class Classification { LITERAL, BINARY, UNARY, BRACKET, FUNCTION };

 private:
  std::string value_;
  Classification classification_;

 public:
  Letter(std::string value, Classification c)
      : value_(value), classification_(c){};

  const Classification& getClassification() { return classification_; }

  std::string getValue() { return value_; }

  virtual void doAction(CalculatorState& stats) = 0;
};

}  // namespace calculator::math
