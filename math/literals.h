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

#include <limits>
#include <memory>
#include <string>
#include <variant>

#include "letter.h"

namespace picolator::math {

// forward declaration for Consts
class Literals;

class Constant {
 public:
  // Represents an x(pi) ie 2(pi) or (3pi)
  std::unique_ptr<Literals> x_;
  // Represents an pi^pow_ ie pi^2 or pi^3
  std::unique_ptr<Literals> pow_;

  Constant(std::unique_ptr<Literals>&& x, std::unique_ptr<Literals>&& pow)
      : x_(std::move(x)), pow_(std::move(pow)) {}
};

class PI : public Constant {
 public:
  static constexpr double value = 3.141592653589793;
  PI(std::unique_ptr<Literals>&& x, std::unique_ptr<Literals>&& pow)
      : Constant(std::move(x), std::move(pow)) {}
};

class E : public Constant {
 public:
  static constexpr double value = 2.718281828459045;
  E(std::unique_ptr<Literals>&& x, std::unique_ptr<Literals>&& pow)
      : Constant(std::move(x), std::move(pow)) {}
};

class Literals : public Letter {
 public:
  enum class Type {
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
  Type type_;
  std::variant<Fraction, double, long> num_ = 0L;
  char variable_ = ' ';
  std::unique_ptr<Constant> constant_ = {};

 public:
  /**
   * @brief Creates double Literal
   *
   * @param d double
   */
  Literals(double d);

  /**
   * @brief Creates Long Literal
   *
   * @param d long
   */
  Literals(long d);

  /**
   * @brief Creates Long Literal
   *
   * @param d long
   */
  Literals(int d);

  /**
   * @brief Creates a variable literal
   * Its value will be grabbed when used from a global static table
   *
   * @param c Variable name
   */
  Literals(char c);

  /**
   * @brief Creates a fraction Literal
   *
   * @param numerator
   * @param denominator
   */
  Literals(const Literals& numerator, const Literals& denominator);

  // Const Constructors
  Literals(Type type);
  Literals(Type type, const Literals& x, const Literals& pow);
  Literals(const Literals&);

  double getDouble() const;
  long getLong() const;
  const Fraction& getFraction();
  const Type& getType() const { return type_; }

  // I need this because I am crazy
  Literals operator+(const Literals& rhs) const;
  Literals operator*(const Literals& rhs) const;
  Literals operator/(const Literals& rhs) const;
  Literals operator%(const Literals& rhs) const;
  bool operator==(const Literals& rhs) const;
  Literals operator-() const;
};
}  // namespace picolator::math