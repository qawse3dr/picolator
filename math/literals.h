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

#include <string>

#include "letter.h"

namespace calculator::math {
class Literals : public Letter {
 public:
  enum class LiteralsType {
    NUMBER,
    PI,
    E,
    VARIABLE,
    ANS,
  };

  static double ans_;

 private:
  LiteralsType type_;
  double num_;
  char variable_ = ' ';

 public:
  Literals(double d)
      : Letter(std::to_string(d), Letter::Classification::LITERAL), num_(d) {}
  Literals(char c);
  Literals(LiteralsType type);

  float returnValue() {
    switch (type_) {
      case LiteralsType::VARIABLE:
        return 0;  // TODO IMPL
      case LiteralsType::NUMBER:
        return num_;
      case LiteralsType::PI:
        return M_PI;
      case LiteralsType::E:
        return M_E;
      case LiteralsType::ANS:
        return ans_;
    }
  }
};
}  // namespace calculator::math