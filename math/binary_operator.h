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
#include <cmath>

#include "letter.h"
#include "literals.h"

namespace picolator::math {
class BinaryOperator : public Letter {
 public:
  enum class Type {
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    EXPONENT,
    N_TH_ROOT,
    MODULUS
  };

 private:
  Type op_;

 public:
  BinaryOperator(std::string symbol, const Type& op)
      : Letter(symbol, Letter::Classification::BINARY, 3), op_(op) {
    switch (op_) {
      case Type::ADDITION:
        priority_ = 6;
        break;
      case Type::SUBTRACTION:
        priority_ = 7;
        break;
      case Type::MULTIPLICATION:
        priority_ = 5;
        break;
      case Type::DIVISION:
      case Type::MODULUS:
        priority_ = 4;
        break;
      case Type::EXPONENT:
      case Type::N_TH_ROOT:
        priority_ = 3;
        break;
    }
  }

  Literals solve(const Literals& lhs, const Literals& rhs) {
    switch (op_) {
      case Type::ADDITION:
        return lhs + rhs;
        break;
      case Type::SUBTRACTION:
        return lhs + (-rhs);
        break;
      case Type::MULTIPLICATION:
        return lhs * rhs;
        break;
      case Type::DIVISION:
        return lhs / rhs;
      case Type::EXPONENT:
        return pow(lhs.getDouble(), rhs.getDouble());
      default:  // todo impl mod and nth root
        return 0;
    }
  }

  inline const Type getType() const { return op_; }
};
}  // namespace picolator::math