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
#include <cmath>

#include "literals.h"
#include "math_util.h"
#include "unary_operator.h"

using picolator::math::Literals;
using picolator::math::UnaryOperator;

Literals UnaryOperator::solve(const Literals& input) {
  switch (op_) {
    case UnaryOperatorType::MINUS:
      return -input;
    case UnaryOperatorType::SIN:
      return picolator::math::psin(input);
    case UnaryOperatorType::COS:
      return picolator::math::pcos(input);
    case UnaryOperatorType::TAN:
      return picolator::math::ptan(input);
    default:
      return 0;
      break;
  }
}