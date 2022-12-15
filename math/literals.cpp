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

using picolator::math::Constant;
using picolator::math::Literals;

double picolator::math::Literals::ans_ = 0;

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
      return std::make_unique<picolator::math::PI>(
          std::make_unique<Literals>(x), std::make_unique<Literals>(pow));
    case Literals::Type::E:
      return std::make_unique<picolator::math::E>(
          std::make_unique<Literals>(x), std::make_unique<Literals>(pow));
    default:
      // todo throw error
      return std::make_unique<picolator::math::PI>(
          std::make_unique<Literals>(0), std::make_unique<Literals>(0));
  }
}

Literals::Literals(Type type)
    : Letter(typeToString(type), Letter::Classification::LITERAL, 0),
      type_(type),
      constant_(createConstant(type, Literals(1L), Literals(1L))) {
}  // todo add erro checking

Literals::Literals(Type type, const Literals& x, const Literals& pow)
    : Letter(typeToString(type), Letter::Classification::LITERAL, 0),
      type_(type),
      constant_(createConstant(type, x, pow)) {}

Literals::Literals(const Literals& numerator, const Literals& denominator)
    : Letter(numerator.getValue() + "/" + denominator.getValue(),
             Letter::Classification::LITERAL, 0),
      num_(std::move(Fraction(numerator, denominator))),
      type_(Type::FRACTION) {}

Literals::Literals(const Literals& rhs)
    : Letter(rhs.getValue(), rhs.getClassification(), 0),
      type_(rhs.type_),
      variable_(rhs.variable_) {
  switch (type_) {
    case Type::DOUBLE:
      num_ = std::get<double>(rhs.num_);
      break;
    case Type::LONG:
      num_ = std::get<long>(rhs.num_);
      break;
    case Type::FRACTION: {
      Fraction frac(std::get<Fraction>(rhs.num_));
      num_.emplace<Fraction>(std::move(frac));
      break;
    }
  }
}

double Literals::getDouble() const {
  switch (type_) {
    case Type::VARIABLE:
      return 0;  // TODO IMPL
    case Type::DOUBLE:
      return std::get<double>(num_);
    case Type::LONG:
      return std::get<long>(num_);
    case Type::FRACTION:
      return std::get<Fraction>(num_).numerator->getDouble() /
             std::get<Fraction>(num_).denominator->getDouble();
    case Type::PI:
      return constant_->x_->getDouble() *
             pow(PI::value, constant_->pow_->getDouble());
    case Type::E:
      return constant_->x_->getDouble() *
             pow(E::value, constant_->pow_->getDouble());
    case Type::ANS:
      return ans_;
    default:
      // TODO throw err
      return 0;
  }
}

long Literals::getLong() const {
  switch (type_) {
    case Type::LONG:
      return std::get<long>(num_);
    default:
      // TODO throw error
      return 0;
  }
}

const Literals::Fraction& Literals::getFraction() {
  if (type_ == Type::FRACTION) {
    return std::get<Fraction>(num_);
  } else {
    // TODO throw error
  }
}

Literals Literals::operator+(const Literals& rhs) const {
  if (rhs.type_ == type_) {
    switch (type_) {
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
        if (*rhs.constant_->pow_ == *constant_->pow_) {
          return Literals(type_, *rhs.constant_->x_ + *constant_->x_,
                          *constant_->pow_);
        }
    }
  } else if (rhs.type_ == Type::FRACTION && type_ == Type::LONG) {
    const Fraction& frac = std::get<Fraction>(rhs.num_);
    return Literals(*frac.denominator * std::get<long>(num_) + *frac.numerator,
                    *frac.denominator);
  } else if (type_ == Type::FRACTION && rhs.type_ == Type::LONG) {
    const Fraction& frac = std::get<Fraction>(num_);
    return Literals(
        *frac.denominator * std::get<long>(rhs.num_) + *frac.numerator,
        *frac.denominator);
  }
  return Literals(rhs.getDouble() + getDouble());
}

Literals Literals::operator*(const Literals& rhs) const {
  if (rhs.type_ == type_) {
    switch (type_) {
      case Type::LONG:
        return Literals(rhs.getLong() * getLong());
      case Type::FRACTION: {
        const Fraction& left = std::get<Fraction>(num_);
        const Fraction& right = std::get<Fraction>(rhs.num_);

        return Literals(*left.numerator + *right.numerator,
                        *left.denominator + *right.denominator);
      }
      case Type::PI:
        return Literals(Type::PI, *rhs.constant_->x_ * *constant_->x_,
                        *rhs.constant_->pow_ + *constant_->pow_);
      case Type::E:
        return Literals(Type::E, *rhs.constant_->x_ * *constant_->x_,
                        *rhs.constant_->pow_ + *constant_->pow_);
    }
  } else if (rhs.type_ == Type::FRACTION &&
             (type_ == Type::LONG || type_ == Type::PI)) {
    const Fraction& frac = std::get<Fraction>(rhs.num_);
    return Literals(*this * *frac.numerator, *frac.denominator);
  } else if (type_ == Type::FRACTION &&
             (rhs.type_ == Type::LONG || rhs.type_ == Type::PI)) {
    const Fraction& frac = std::get<Fraction>(num_);
    return Literals(rhs * *frac.numerator, *frac.denominator);
  }
  return Literals(rhs.getDouble() * getDouble());
}

Literals Literals::operator/(const Literals& rhs) const {
  // return fraction
  if (rhs.type_ == type_ && type_ == Type::LONG) {
    return Literals(*this, rhs);
  }
  return getDouble() / rhs.getDouble();
}

Literals Literals::operator%(const Literals& rhs) const {
  if (rhs.type_ != Type::LONG || type_ != Type::LONG) {
    throw picolator::math::TypeError("%", "Int");
  }
  return getLong() % rhs.getLong();
}

bool Literals::operator==(const Literals& rhs) const {
  // return fraction
  return (std::fabs(rhs.getDouble() - getDouble()) <=
          std::numeric_limits<double>::epsilon() * 2);
}

Literals Literals::operator-() const {
  switch (type_) {
    case Type::VARIABLE:
      return 0;  // TODO IMPL
    case Type::DOUBLE:
      return Literals(-std::get<double>(num_));
    case Type::LONG:
      return Literals(-std::get<long>(num_));
    case Type::FRACTION:
      return Literals(-(*std::get<Fraction>(num_).numerator),
                      *std::get<Fraction>(num_).denominator);
    case Type::PI:
    case Type::E:
      return Literals(type_, -(*constant_->x_), *constant_->pow_);
    case Type::ANS:
      return ans_;
    default:
      // TODO throw err
      return 0;
  }
}
