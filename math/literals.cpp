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
#include "literals.h"

#include <limits>

#include "math_util.h"

using picolator::math::Constant;
using picolator::math::Literals;

std::string typeToString(Literals::Type type) {
  switch (type) {
    case Literals::Type::PI:
      return "\xF7";
    case Literals::Type::E:
      return "e";
    case Literals::Type::ANS:
      return "ANS";
    default:
      return "X";
  }
}
Literals::Literals(double d)
    : Letter(std::to_string(d), Letter::Classification::LITERAL, 0),
      num_(d),
      type_(Type::DOUBLE) {}
Literals::Literals(long l)
    : Letter(std::to_string(l), Letter::Classification::LITERAL, 0),
      num_(l),
      type_(Type::LONG) {}
Literals::Literals(int l)
    : Letter(std::to_string(l), Letter::Classification::LITERAL, 0),
      num_(l),
      type_(Type::LONG) {}
Literals::Literals(char c)
    : Letter(std::string(1, c), Letter::Classification::LITERAL, 0),
      variable_(c),
      type_(Type::VARIABLE) {}

static std::unique_ptr<Constant> createConstant(Literals::Type type,
                                                const Literals& x = 1,
                                                const Literals& pow = 1) {
  switch (type) {
    case Literals::Type::PI:  // valid values
      return std::unique_ptr<Constant>(new picolator::math::PI(x, pow));
    case Literals::Type::E:
      return std::unique_ptr<Constant>(new picolator::math::E(x, pow));
    default:  // not a const ignore
      return nullptr;
  }
}

Literals::Literals(Type type)
    : Letter(typeToString(type), Letter::Classification::LITERAL, 0),
      type_(type),
      constant_(createConstant(type, Literals(1L), Literals(1L))) {
}  // todo add error checking

Literals::Literals(Type type, const Literals& x, const Literals& pow)
    : Letter(typeToString(type), Letter::Classification::LITERAL, 0),
      type_(type),
      constant_(createConstant(type, x, pow)) {}

Literals::Literals(const Literals& numerator, const Literals& denominator)
    : Letter(numerator.getSymbol() + "/" + denominator.getSymbol(),
             Letter::Classification::LITERAL, 0),
      num_(std::move(Fraction(numerator, denominator))),
      type_(Type::FRACTION) {}

Literals::Literals(const Literals& rhs)
    : Letter(rhs.getSymbol(), rhs.getClassification(), 0),
      type_(rhs.type_),
      variable_(rhs.variable_),
      constant_(
          createConstant(rhs.type_, rhs.constant_->x_, rhs.constant_->pow_)) {
  switch (getType()) {
    case Type::DOUBLE:
      num_ = rhs.getDouble();
      break;
    case Type::LONG:
      num_ = rhs.getLong();
      break;
    case Type::FRACTION: {
      Fraction frac(rhs.getFraction());
      num_.emplace<Fraction>(std::move(frac));
      break;
    }
    default:
      break;
  }
}

Literals Literals::operator=(const Literals& rhs) {
  // Copies for Letter
  symbol_ = rhs.symbol_;
  classification_ = rhs.classification_;
  priority_ = rhs.priority_;

  // Copies for LiteralS
  type_ = rhs.type_;
  variable_ = rhs.variable_;
  constant_ = createConstant(rhs.type_, rhs.constant_->x_, rhs.constant_->pow_);
  // copy the data rom variant if needed
  switch (rhs.getType()) {
    case Type::DOUBLE:
      num_ = rhs.getDouble();
      break;
    case Type::LONG:
      num_ = rhs.getLong();
      break;
    case Type::FRACTION: {
      Fraction frac(rhs.getFraction());
      num_.emplace<Fraction>(std::move(frac));
      break;
    }
    default:
      break;
  }

  return *this;
}

double Literals::getValue() const {
  switch (getType()) {
    case Type::DOUBLE:
      return getDouble();
    case Type::LONG:
      return getLong();
    case Type::FRACTION:
      return getNumerator().getValue() / getDenominator().getValue();
    case Type::PI:
      return constant_->x_.getValue() *
             pow(PI::value, constant_->pow_.getValue());
    case Type::E:
      return constant_->x_.getValue() *
             pow(E::value, constant_->pow_.getValue());
    default:
      throw NotImplementedError(__func__);
  }
}

std::string Literals::toString() const {
  switch (getType()) {
    case Type::LONG:
      return std::to_string(getLong());
    case Type::FRACTION:
      return getNumerator().toString() + "/" + getDenominator().toString();
    case Type::E:
    case Type::PI: {
      auto x = constant_->x_.toString();
      auto p = constant_->pow_.toString();
      return (x != "1" ? x : "") + getSymbol() +
             (p != "1" ? "^" + constant_->pow_.toString() : "");
    }
    default:
      return std::to_string(getValue());
  }
}

long Literals::getLong() const {
  if (getType() != Type::LONG) {
    throw TypeError(__func__, "int");
  }
  return std::get<long>(getLiteral().num_);
}

const Literals::Fraction& Literals::getFraction() const {
  if (getType() != Type::FRACTION) {
    throw TypeError(__func__, "Frac");
  }
  return std::get<Fraction>(getLiteral().num_);
}

Literals& Literals::getNumerator() const {
  if (getType() != Type::FRACTION) {
    throw TypeError(__func__, "Frac");
  }
  return *std::get<Fraction>(getLiteral().num_).numerator;
}

Literals& Literals::getDenominator() const {
  if (getType() != Type::FRACTION) {
    throw TypeError(__func__, "Frac");
  }
  return *std::get<Fraction>(getLiteral().num_).denominator;
}

double Literals::getDouble() const {
  if (getType() != Type::DOUBLE) {
    throw TypeError(__func__, "Frac");
  }
  return std::get<double>(getLiteral().num_);
}

Constant& Literals::getConstant() const {
  if (getType() != Type::E && getType() != Type::PI) {
    throw TypeError(__func__, "Frac");
  }
  return *getLiteral().constant_;
}

Literals& Literals::getAnswer() {
  static Literals ans(0);
  return ans;
}

Literals& Literals::getVariable(uint8_t var) {
  static Literals variables[6] = {0, 0, 0, 0, 0, 0};
  if (var > 'F' || var < 'A') {
    throw picolator::math::DomainError("Var doesn't exist");
  }
  return variables[static_cast<int>(var - 'A')];
}

Literals& Literals::getLiteral() {
  switch (type_) {
    case Type::ANS:
      return getAnswer();
    case Type::VARIABLE:
      return getVariable(variable_);
    default:
      return *this;
  }
}
const Literals& Literals::getLiteral() const {
  switch (type_) {
    case Type::ANS:
      return getAnswer();
    case Type::VARIABLE:
      return getVariable(variable_);
    default:
      return *this;
  }
}

Literals Literals::reduce() const {
  if (getType() == Type::FRACTION) {
    const auto& numerator = getNumerator();
    const auto& denominator = getDenominator();

    // Check to make sure that the numerator and denominators aren't doubles
    if (numerator.getType() == Type::DOUBLE ||
        denominator.getType() == Type::DOUBLE) {
      return numerator.getValue() / denominator.getValue();
    }

    int num_value = 0;
    int den_value = 0;

    if (numerator == denominator) {
      return 1;
    }
    if (numerator.getValue() == 0) {
      return 0;
    }
    // get Numerator value
    if (numerator.getType() == Type::LONG) {
      num_value = numerator.getLong();
    } else if (numerator.isConstant()) {
      const auto& con = numerator.getConstant();
      num_value = con.x_.getLong();
    } else {
      throw TypeError(__func__, "Long/Const");
    }

    // get denominator value
    if (denominator.getType() == Type::LONG) {
      den_value = denominator.getLong();
    } else if (denominator.isConstant()) {
      const auto& con = denominator.getConstant();
      den_value = con.x_.getLong();
    } else {
      throw TypeError(__func__, "Long/Const");
    }

    // Uses euclidean gcd to reduce
    int tmp_a;
    while (den_value != 0) {
      tmp_a = num_value;
      num_value = den_value;
      den_value = tmp_a % den_value;
    }

    Literals new_num(numerator), new_den(denominator);
    if (numerator.isConstant()) {
      new_num = Literals(numerator.getType(),
                         numerator.getConstant().x_.getLong() / num_value,
                         numerator.getConstant().pow_);
    } else {
      new_num = numerator.getLong() / num_value;
    }

    if (denominator.isConstant()) {
      new_den = Literals(new_den.getType(),
                         new_den.getConstant().x_.getLong() / num_value,
                         new_den.getConstant().pow_);
    } else {
      new_den = new_den.getLong() / num_value;
    }

    // divide out constants
    if (numerator.isConstant() &&
        numerator.getType() == denominator.getType()) {
      if (numerator.getConstant().pow_ == denominator.getConstant().pow_) {
        // they cancel
        new_num = numerator.getConstant().x_;
        new_den = new_den.getConstant().x_;
      } else if (numerator.getConstant().pow_.getLong() >
                 denominator.getConstant().pow_.getLong()) {
        // bottom cancels
        new_num = Literals(new_num.getType(), new_num.getConstant().x_,
                           new_num.getConstant().pow_.getLong() -
                               new_den.getConstant().pow_.getLong());
        new_den = new_den.getConstant().x_;
      } else {
        // top cancels
        new_num = new_num.getConstant().x_;
        new_den = Literals(new_den.getType(), new_den.getConstant().x_,
                           new_den.getConstant().pow_.getLong() -
                               new_num.getConstant().pow_.getLong());
      }
    }

    if (new_den == Literals(1)) return new_num;
    return Literals(new_num, new_den);

  } else {
    return *this;
  }
}

Literals Literals::operator+(const Literals& rhs) const {
  if (rhs.getType() == getType()) {
    switch (getType()) {
      case Type::LONG:
        return Literals(rhs.getLong() + getLong());
      case Type::FRACTION: {
        const Fraction& left = std::get<Fraction>(num_);
        const Fraction& right = std::get<Fraction>(rhs.num_);
        if (*left.denominator == *right.denominator) {
          return Literals(*left.numerator + *right.numerator,
                          *right.denominator);
        } else {
          return Literals(*left.numerator * *right.denominator +
                              *right.numerator * *left.denominator,
                          *left.denominator * *right.denominator);
        }
      }
      case Type::PI:
      case Type::E:
        if (rhs.constant_->pow_ == constant_->pow_) {
          return Literals(getType(), rhs.constant_->x_ + constant_->x_,
                          constant_->pow_);
        }
    }
  } else if (rhs.getType() == Type::FRACTION && getType() == Type::LONG) {
    const Fraction& frac = std::get<Fraction>(rhs.num_);
    return Literals(*frac.denominator * std::get<long>(num_) + *frac.numerator,
                    *frac.denominator);
  } else if (getType() == Type::FRACTION && rhs.getType() == Type::LONG) {
    const Fraction& frac = std::get<Fraction>(num_);
    return Literals(
        *frac.denominator * std::get<long>(rhs.num_) + *frac.numerator,
        *frac.denominator);
  }
  return Literals(rhs.getValue() + getValue());
}

Literals Literals::operator*(const Literals& rhs) const {
  if (rhs.getType() == getType()) {
    switch (getType()) {
      case Type::LONG:
        return Literals(rhs.getLong() * getLong());
      case Type::FRACTION: {
        const Fraction& left = std::get<Fraction>(num_);
        const Fraction& right = std::get<Fraction>(rhs.num_);

        return Literals(*left.numerator * *right.numerator,
                        *left.denominator * *right.denominator);
      }
      case Type::PI:
      case Type::E:
        return Literals(getType(), rhs.constant_->x_ * constant_->x_,
                        rhs.constant_->pow_ + constant_->pow_);
    }
  } else if (rhs.getType() == Type::FRACTION &&
             (getType() == Type::LONG || isConstant())) {
    const Fraction& frac = rhs.getFraction();
    return Literals(*this * getNumerator(), getDenominator());
  } else if (getType() == Type::FRACTION &&
             (rhs.getType() == Type::LONG || rhs.isConstant())) {
    const Fraction& frac = std::get<Fraction>(num_);
    return Literals(rhs * getNumerator(), getDenominator());
  } else if (getType() == Type::LONG && (rhs.isConstant())) {
    return Literals(rhs.getType(), rhs.getConstant().x_ * *this,
                    rhs.getConstant().pow_);
  } else if (isConstant() && (rhs.getType() == Type::LONG)) {
    return Literals(getType(), getConstant().x_ * rhs, getConstant().pow_);
  }
  return Literals(rhs.getValue() * getValue());
}

Literals Literals::operator/(
    const Literals& rhs) const {  // TODO clean up this function
  if (rhs.getValue() < std::numeric_limits<double>::epsilon()) {
    throw picolator::math::DivideByZero();
  }
  // return fraction
  if (rhs.getType() == getType() && getType() == Type::LONG) {
    if (*this % rhs == 0) {
      return getLong() / rhs.getLong();
    }
    return Literals(*this, rhs).reduce();
  } else if (getType() == Type::LONG && rhs.isConstant() ||
             isConstant() && rhs.getType() == Type::LONG ||
             isConstant() && rhs.isConstant()) {
    return Literals(*this, rhs).reduce();
  }
  return getValue() / rhs.getValue();
}

Literals Literals::operator%(const Literals& rhs) const {
  if (rhs.getType() != Type::LONG || getType() != Type::LONG) {
    throw picolator::math::TypeError("%", "Int");
  }
  return getLong() % rhs.getLong();
}

bool Literals::operator==(const Literals& rhs) const {
  // return fraction
  return (std::fabs(rhs.getValue() - getValue()) <=
          std::numeric_limits<double>::epsilon() * 2);
}

Literals Literals::operator-() const {
  switch (getType()) {
    case Type::DOUBLE:
      return Literals(-getDouble());
    case Type::LONG:
      return Literals(-getLong());
    case Type::FRACTION:
      return Literals(-(getNumerator()), getDenominator());
    case Type::PI:
    case Type::E:
      return Literals(getType(), -(getConstant().x_), getConstant().pow_);
    default:
      // TODO throw err
      throw NotImplementedError(__func__);
  }
}

Literals Literals::operator^(const Literals& rhs) const {
  if (rhs.getType() == getType()) {
    switch (getType()) {
      case Type::LONG:
        return Literals(pow(rhs.getLong(), getLong()));
    }
  }
  if (isConstant()) {
    if (rhs.getType() == Type::LONG) {
      return Literals(getType(), getConstant().x_, getConstant().pow_ + rhs);
    }
  }
  return Literals(pow(getValue(), rhs.getValue()));
}