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
#include "literals.h"

namespace picolator::math {
class UnaryOperator : public Letter {
 public:
  enum class Type {
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
  Type op_;

 public:
  UnaryOperator(std::string symbol, const Type op)
      : Letter(symbol, Letter::Classification::UNARY, 2), op_(op) {}

  const Type& getOp() { return op_; }
  Literals solve(const Literals& input);
};
}  // namespace picolator::math
