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

class PI {
 public:
  static constexpr double value = 3.141592653589793;
};

class E {
 public:
  static constexpr double value = 2.718281828459045;
};

class Literals : public Letter {
 public:
  enum class LiteralsType {
    DOUBLE,
    LONG,
    FRACTION,
    PI,
    E,
    VARIABLE,
    ANS,
    SQRT_NUM,  // not sure if i'll use this
  };

  struct Fraction {
    std::unique_ptr<Literals> numerator;
    std::unique_ptr<Literals> denominator;

    Fraction(const Literals& num, const Literals& den) {
      numerator = std::make_unique<Literals>(num);
      denominator = std::make_unique<Literals>(den);
    }
    Fraction(const Fraction& rhs) {
      numerator = std::make_unique<Literals>(*rhs.numerator);
      denominator = std::make_unique<Literals>(*rhs.denominator);
    }
  };

  static double ans_;

 private:
  LiteralsType type_;
  std::variant<Fraction, double, long> num_ = 0L;
  char variable_ = ' ';

 public:
  Literals(double d);
  Literals(long d);
  Literals(int d);
  Literals(char c);
  Literals(const Literals& numerator, const Literals& denominator);
  Literals(LiteralsType type);
  Literals(const Literals&);

  double getDouble() const {
    switch (type_) {
      case LiteralsType::VARIABLE:
        return 0;  // TODO IMPL
      case LiteralsType::DOUBLE:
        return std::get<double>(num_);
      case LiteralsType::LONG:
        return std::get<long>(num_);
      case LiteralsType::FRACTION:
        return std::get<Fraction>(num_).numerator->getDouble() /
               std::get<Fraction>(num_).denominator->getDouble();
      case LiteralsType::PI:
        return PI::value;
      case LiteralsType::E:
        return E::value;
      case LiteralsType::ANS:
        return ans_;
      default:
        // TODO throw err
        return 0;
    }
  }

  long getLong() const {
    switch (type_) {
      case LiteralsType::LONG:
        return std::get<long>(num_);
      default:
        // TODO throw error
        return 0;
    }
  }

  const Fraction& getFraction() {
    if (type_ == LiteralsType::FRACTION) {
      return std::get<Fraction>(num_);
    } else {
      // TODO throw error
    }
  }

  const LiteralsType& getType() const { return type_; }

  // I need this because I am crazy
  Literals operator+(const Literals& rhs) const;
  Literals operator*(const Literals& rhs) const;
  Literals operator/(const Literals& rhs) const;
  bool operator==(const Literals& rhs) const;
};
}  // namespace picolator::math