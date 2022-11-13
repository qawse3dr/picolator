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

namespace calculator::math {
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
  UnaryOperationType op_;

 public:
  UnaryOperation(std::sting symbol, const UnaryOperationType op)
      : Letter(symbol, Letter::Classification::UNARY), op_(op) {}

  const UnaryOperationType& getOp() { return op_; }
  void doAction(CalculatorState& stats) override;
};
}  // namespace calculator::math