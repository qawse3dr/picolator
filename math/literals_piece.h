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

#include <math.h>

#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <variant>

#include "letter.h"

namespace picolator::math {

class LiteralsPiece : public Letter {
 public:
  enum class LiteralsPieceType { NUM };

  const char value_ = ' ';

 public:
  LiteralsPiece(uint8_t value)
      : Letter(std::string(1, static_cast<char>(value)),
               Letter::Classification::LITERAL_PIECE, -1),
        value_(value) {}
};
}  // namespace picolator::math