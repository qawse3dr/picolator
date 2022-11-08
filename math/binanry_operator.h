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
class BinaryOperator : public Letter {
 public:
  enum class BinaryOperatorType {
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    EXPONENT,
    N_TH_ROOT,
    MODULUS
  };

 private:
  // The priority of operation
  int priority;
};
}  // namespace calculator::math