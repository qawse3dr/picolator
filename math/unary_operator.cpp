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
#include "unary_operator.h"

#include <cmath>

#include "literals.h"
#include "math_util.h"

using picolator::math::Literals;
using picolator::math::UnaryOperator;

Literals UnaryOperator::solve(const Literals& input) {
  switch (op_) {
    case Type::MINUS:
      return -input;
    case Type::SIN:
      return picolator::math::psin(input);
    case Type::COS:
      return picolator::math::pcos(input);
    case Type::TAN:
      return picolator::math::ptan(input);
    case Type::SQUARE_ROOT:
      if (input.getDouble() < 0) throw DomainError("sqrt");
      return sqrt(input.getDouble());
    default:
      return 0;
      break;
  }
}