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
#include "letter.h"

namespace picolator::math {
class UnaryOperator : public Letter {
 public:
  enum class UnaryOperatorType {
    SIN,
    COS,
    TAN,
    ARCSIN,
    ARCHCOS,
    ARCHTAN,
    LOG,
    LN,
    MINUS,
    SQUARE_ROOT
  };

 private:
  UnaryOperatorType op_;

 public:
  UnaryOperator(std::string symbol, const UnaryOperatorType op)
      : Letter(symbol, Letter::Classification::UNARY), op_(op) {}

  const UnaryOperatorType& getOp() { return op_; }
  double solve(double input);
};
}  // namespace picolator::math
