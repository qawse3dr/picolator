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

using picolator::math::Constant;
using picolator::math::Literals;

double picolator::math::Literals::ans_ = 0;

std::string typeToString(Literals::LiteralsType type) {
  switch (type) {
    case Literals::LiteralsType::PI:
      return "\x7F";
    case Literals::LiteralsType::E:
      return "e";
    case Literals::LiteralsType::ANS:
      return "ANS";
    default:
      return "X";
  }
}
Literals::Literals(double d)
    : Letter(std::to_string(d), Letter::Classification::LITERAL),
      num_(d),
      type_(LiteralsType::DOUBLE) {}
Literals::Literals(long l)
    : Letter(std::to_string(l), Letter::Classification::LITERAL),
      num_(l),
      type_(LiteralsType::LONG) {}
Literals::Literals(int l)
    : Letter(std::to_string(l), Letter::Classification::LITERAL),
      num_(l),
      type_(LiteralsType::LONG) {}
Literals::Literals(char c)
    : Letter(std::to_string(c), Letter::Classification::LITERAL),
      variable_(c),
      type_(LiteralsType::VARIABLE) {}

Literals::Literals(LiteralsType type)
    : Letter(typeToString(type), Letter::Classification::LITERAL), type_(type) {
  switch (type_) {
    case LiteralsType::PI:  // valid values
    case LiteralsType::E:
    case LiteralsType::ANS:
      return;
    default:
      return;
      // todo throw error. (maybe this should be done in a factory so I can
      // throw errors)
  }
}

static std::unique_ptr<Constant> createConstant(
    Literals::LiteralsType type, const Literals& x = Literals(1),
    const Literals& pow = Literals(1)) {
  switch (type) {
    case Literals::LiteralsType::PI:  // valid values
      return std::make_unique<picolator::math::PI>(
          std::make_unique<Literals>(x), std::make_unique<Literals>(pow));
    case Literals::LiteralsType::E:
      return std::make_unique<picolator::math::E>(
          std::make_unique<Literals>(x), std::make_unique<Literals>(pow));
    default:
      // todo throw error
      return std::make_unique<picolator::math::PI>(
          std::make_unique<Literals>(0), std::make_unique<Literals>(0));
  }
}

Literals::Literals(LiteralsType type, const Literals& x, const Literals& pow)
    : Letter(typeToString(type), Letter::Classification::LITERAL),
      type_(type),
      constant_(createConstant(type, x, pow)) {}

Literals::Literals(const Literals& numerator, const Literals& denominator)
    : Letter(numerator.getValue() + "/" + denominator.getValue(),
             Letter::Classification::LITERAL),
      num_(std::move(Fraction(numerator, denominator))),
      type_(LiteralsType::FRACTION) {}

Literals::Literals(const Literals& rhs)
    : Letter(rhs.getValue(), rhs.getClassification()),
      type_(rhs.type_),
      variable_(rhs.variable_) {
  switch (type_) {
    case LiteralsType::DOUBLE:
      num_ = std::get<double>(rhs.num_);
      break;
    case LiteralsType::LONG:
      num_ = std::get<long>(rhs.num_);
      break;
    case LiteralsType::FRACTION: {
      Fraction frac(std::get<Fraction>(rhs.num_));
      num_.emplace<Fraction>(std::move(frac));
      break;
    }
  }
}

double Literals::getDouble() const {
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
      return constant_->x_->getDouble() *
             pow(PI::value, constant_->pow_->getDouble());
    case LiteralsType::E:
      return constant_->x_->getDouble() *
             pow(E::value, constant_->pow_->getDouble());
    case LiteralsType::ANS:
      return ans_;
    default:
      // TODO throw err
      return 0;
  }
}

long Literals::getLong() const {
  switch (type_) {
    case LiteralsType::LONG:
      return std::get<long>(num_);
    default:
      // TODO throw error
      return 0;
  }
}

const Literals::Fraction& Literals::getFraction() {
  if (type_ == LiteralsType::FRACTION) {
    return std::get<Fraction>(num_);
  } else {
    // TODO throw error
  }
}

Literals Literals::operator+(const Literals& rhs) const {
  if (rhs.type_ == type_) {
    switch (type_) {
      case LiteralsType::LONG:
        return Literals(rhs.getLong() + getLong());
      case LiteralsType::FRACTION: {
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
      case LiteralsType::PI:
      case LiteralsType::E:
        if (*rhs.constant_->pow_ == *constant_->pow_) {
          return Literals(type_, *rhs.constant_->x_ + *constant_->x_,
                          *constant_->pow_);
        }
    }
  } else if (rhs.type_ == LiteralsType::FRACTION &&
             type_ == LiteralsType::LONG) {
    const Fraction& frac = std::get<Fraction>(rhs.num_);
    return Literals(*frac.denominator * std::get<long>(num_) + *frac.numerator);
  } else if (type_ == LiteralsType::FRACTION &&
             rhs.type_ == LiteralsType::LONG) {
    const Fraction& frac = std::get<Fraction>(num_);
    return Literals(*frac.denominator * std::get<long>(rhs.num_) +
                    *frac.numerator);
  }
  return Literals(rhs.getDouble() + getDouble());
}

Literals Literals::operator*(const Literals& rhs) const {
  if (rhs.type_ == type_) {
    switch (type_) {
      case LiteralsType::LONG:
        return Literals(rhs.getLong() * getLong());
      case LiteralsType::FRACTION: {
        const Fraction& left = std::get<Fraction>(num_);
        const Fraction& right = std::get<Fraction>(rhs.num_);

        return Literals(*left.numerator + *right.numerator,
                        *left.denominator + *right.denominator);
      }
      case LiteralsType::PI:
        return Literals(LiteralsType::PI, *rhs.constant_->x_ * *constant_->x_,
                        *rhs.constant_->pow_ + *constant_->pow_);
      case LiteralsType::E:
        return Literals(LiteralsType::E, *rhs.constant_->x_ * *constant_->x_,
                        *rhs.constant_->pow_ + *constant_->pow_);
    }
  } else if (rhs.type_ == LiteralsType::FRACTION &&
             (type_ == LiteralsType::LONG || type_ == LiteralsType::PI)) {
    const Fraction& frac = std::get<Fraction>(rhs.num_);
    return Literals(*this * *frac.numerator, *frac.denominator);
  } else if (type_ == LiteralsType::FRACTION &&
             (rhs.type_ == LiteralsType::LONG ||
              rhs.type_ == LiteralsType::PI)) {
    const Fraction& frac = std::get<Fraction>(num_);
    return Literals(rhs * *frac.numerator, *frac.denominator);
  }
  return Literals(rhs.getDouble() + rhs.getDouble());
}

Literals Literals::operator/(const Literals& rhs) const {
  // return fraction
  if (rhs.type_ == type_ && type_ == LiteralsType::LONG) {
    return Literals(*this, rhs);
  }
  return getDouble() / rhs.getDouble();
}

bool Literals::operator==(const Literals& rhs) const {
  // return fraction
  return (std::fabs(rhs.getDouble() - getDouble()) <=
          std::numeric_limits<double>::epsilon() * 2);
}

Literals Literals::operator-() const {
  switch (type_) {
    case LiteralsType::VARIABLE:
      return 0;  // TODO IMPL
    case LiteralsType::DOUBLE:
      return Literals(-std::get<double>(num_));
    case LiteralsType::LONG:
      return Literals(-std::get<long>(num_));
    case LiteralsType::FRACTION:
      return Literals(-(*std::get<Fraction>(num_).numerator),
                      *std::get<Fraction>(num_).denominator);
    case LiteralsType::PI:
    case LiteralsType::E:
      return Literals(type_, -(*constant_->x_), *constant_->pow_);
    case LiteralsType::ANS:
      return ans_;
    default:
      // TODO throw err
      return 0;
  }
}
