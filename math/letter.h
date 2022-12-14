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

namespace picolator::math {

class Letter {
 public:
  enum class Classification {
    LITERAL,
    LITERAL_PIECE,
    BINARY,
    UNARY,
    BRACKET,
    FUNCTION
  };

 private:
  std::string value_;
  Classification classification_;

 protected:
  /** used when creating the tree.
   * priority should go
   * 0 Literals
   * 1 brackets // might be 2 not unary
   * 2 UnaryOps
   * 3 exponents
   * 4 division / mod
   * 5 multiplication
   * 6 addition
   * 7 subtraction
   */
  uint8_t priority_;

 public:
  Letter(std::string value, Classification c, uint8_t priority)
      : value_(value), classification_(c), priority_(priority){};

  const Classification& getClassification() const { return classification_; }

  std::string getValue() const { return value_; }

  inline int8_t getPriority() const { return priority_; }
};

}  // namespace picolator::math
