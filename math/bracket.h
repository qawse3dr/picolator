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
class Bracket : public Letter {
 public:
  enum class BracketType {
    OPEN,
    CLOSED,
  };

 private:
  BracketType type_;

 public:
  Bracket(const BracketType& type)
      : Letter((type == BracketType::OPEN) ? "(" : ")",
               Letter::Classification::BRACKET),
        type_(type) {}

  const BracketType& getType() { return type_; };
};
}  // namespace picolator::math