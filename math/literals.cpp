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

#include "math_util.h"

#define LITERAL_GET_VALUE(func_ans, func_val) \
  ((type_ == Literals::Type::ANS) ? getAnswer().func_ans : func_val)

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
    : Letter(std::to_string(c), Letter::Classification::LITERAL, 0),
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
    case Type::VARIABLE:
      return 0;  // TODO IMPL
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
  return LITERAL_GET_VALUE(getLong(), std::get<long>(num_));
}

const Literals::Fraction& Literals::getFraction() const {
  if (getType() != Type::FRACTION) {
    throw TypeError(__func__, "Frac");
  }
  return LITERAL_GET_VALUE(getFraction(), std::get<Fraction>(num_));
}

Literals& Literals::getNumerator() const {
  if (getType() != Type::FRACTION) {
    throw TypeError(__func__, "Frac");
  }
  return LITERAL_GET_VALUE(getNumerator(), *std::get<Fraction>(num_).numerator);
}

Literals& Literals::getDenominator() const {
  if (getType() != Type::FRACTION) {
    throw TypeError(__func__, "Frac");
  }
  return LITERAL_GET_VALUE(getDenominator(),
                           *std::get<Fraction>(num_).denominator);
}

double Literals::getDouble() const {
  if (getType() != Type::DOUBLE) {
    throw TypeError(__func__, "Frac");
  }
  return LITERAL_GET_VALUE(getDouble(), std::get<double>(num_));
}

Constant& Literals::getConstant() const {
  if (getType() != Type::E && getType() != Type::PI) {
    throw TypeError(__func__, "Frac");
  }
  return LITERAL_GET_VALUE(getConstant(), *constant_);
}

Literals& Literals::getAnswer() {
  static Literals ans(0);
  return ans;
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

        return Literals(*left.numerator + *right.numerator,
                        *left.denominator + *right.denominator);
      }
      case Type::PI:
        return Literals(Type::PI, rhs.constant_->x_ * constant_->x_,
                        rhs.constant_->pow_ + constant_->pow_);
      case Type::E:
        return Literals(Type::E, rhs.constant_->x_ * constant_->x_,
                        rhs.constant_->pow_ + constant_->pow_);
    }
  } else if (rhs.getType() == Type::FRACTION &&
             (getType() == Type::LONG || getType() == Type::PI)) {
    const Fraction& frac = rhs.getFraction();
    return Literals(*this * getNumerator(), getDenominator());
  } else if (getType() == Type::FRACTION &&
             (rhs.getType() == Type::LONG || rhs.getType() == Type::PI)) {
    const Fraction& frac = std::get<Fraction>(num_);
    return Literals(rhs * getNumerator(), getDenominator());
  }
  return Literals(rhs.getValue() * getValue());
}

Literals Literals::operator/(const Literals& rhs) const {
  // return fraction
  if (rhs.getType() == getType() && getType() == Type::LONG) {
    if (*this % rhs == 0) {
      return getLong() / rhs.getLong();
    }
    return Literals(*this, rhs);
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
    case Type::VARIABLE:
      return 0;  // TODO IMPL
    case Type::DOUBLE:
      return Literals(-getDouble());
    case Type::LONG:
      return Literals(-getLong());
    case Type::FRACTION:
      return Literals(-(getNumerator()), getDenominator());
    case Type::PI:
    case Type::E:
      return Literals(getType(), -(getConstant().x_), getConstant().pow_);
    case Type::ANS:
      return -getAnswer();
    default:
      // TODO throw err
      throw std::exception();
  }
}
