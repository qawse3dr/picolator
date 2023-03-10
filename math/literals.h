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

// Forward declaration for Literals
class Constant;

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

  static const Literals* ans_;

 private:
  Type type_;
  std::variant<Fraction, double, long> num_ = 0L;
  char variable_ = ' ';
  std::unique_ptr<Constant> constant_ = {};

  long getLong() const;
  double getDouble() const;
  Literals& getNumerator() const;
  Literals& getDenominator() const;
  Constant& getConstant() const;
  const Fraction& getFraction() const;
  inline bool isConstant() const {
    return (getType() == Type::E || getType() == Type::PI);
  }

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

  // Returns a double value of the Literals
  double getValue() const;
  inline const Type& getType() const { return getLiteral().type_; }
  // finds the reduction of the current literal and returns it.
  Literals reduce() const;

  /**
   * @brief Checks if this literals is a wrapper for
   *        a real literal and returns the correct value
   *        (ie Variable and Answer are just placeholder
   *         and dont contain the actual value
   *        )
   *
   * @return Literals& Ref to actual literal
   */
  const Literals& getLiteral() const;
  Literals& getLiteral();

  // SingletonAnswer
  static Literals& getAnswer();

  // Singleton variables
  static Literals& getVariable(uint8_t var);

  std::string toString() const;

  // I need this because I am crazy
  Literals operator+(const Literals& rhs) const;
  Literals operator*(const Literals& rhs) const;
  Literals operator/(const Literals& rhs) const;
  Literals operator%(const Literals& rhs) const;
  Literals operator^(const Literals& rhs) const;
  bool operator==(const Literals& rhs) const;
  Literals operator-() const;

  // copy operator
  Literals operator=(const Literals& rhs);
};

class Constant {
 public:
  // Represents an x(pi) ie 2(pi) or (3pi)
  Literals x_;
  // Represents an pi^pow_ ie pi^2 or pi^3
  Literals pow_;

  Constant(const Literals& x, const Literals& pow) : x_(x), pow_(pow) {}
};

class PI : public Constant {
 public:
  static constexpr double value = 3.141592653589793;
  PI(const Literals& x, const Literals& pow) : Constant(x, pow) {}
};

class E : public Constant {
 public:
  static constexpr double value = 2.718281828459045;
  E(const Literals& x, const Literals& pow) : Constant(x, pow) {}
};
}  // namespace picolator::math
