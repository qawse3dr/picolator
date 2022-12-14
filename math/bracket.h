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
  enum class Type { OPEN, CLOSED, EQUATION };
  using LetterVec = std::vector<std::shared_ptr<Letter>>;
 private:
  Type type_;

 public:
  
  LetterVec letters_;
  Bracket(const Type& type)
      : Letter((type == Type::OPEN) ? "(" : ")",
               Letter::Classification::BRACKET, 1),
        type_(type) {}

  Bracket(LetterVec&& letters)
      : Letter("Todo Impl", Letter::Classification::BRACKET, 1),
        type_(Type::EQUATION),
        letters_(std::move(letters)) {}

  const Type& getType() const { return type_; };
};
}  // namespace picolator::math