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
#include <exception>

#include "literals.h"

namespace picolator::math {

// NOT will throw Exceptions
// if invalid values are given

class DomainError : public std::exception {
 private:
  std::string msg_;

 public:
  const char* what() const throw() { return msg_.c_str(); }
  DomainError(const std::string& ftn_name) : msg_(ftn_name + " Domain error") {}
};

class DivideByZero : public std::exception {
 public:
  const char* what() const throw() { return "/ By 0"; }
};

class SyntaxError : public std::exception {
 private:
  std::string msg_;

 public:
  const char* what() const throw() { return msg_.c_str(); }
  SyntaxError(const std::string& msg, int idx)
      : msg_("Syntax E " + msg), idx_(idx) {}
  const int idx_;
};

class TypeError : public std::exception {
 private:
  std::string msg_;

 public:
  const char* what() const throw() { return msg_.c_str(); }
  TypeError(const std::string& ftn, const std::string& type)
      : msg_("Type E " + ftn + "-" + type) {}
};

class NotImplementedError : public std::exception {
 private:
  std::string msg_;

 public:
  const char* what() const throw() { return msg_.c_str(); }
  NotImplementedError(const std::string& func) : msg_("NIMP " + func) {}
};

Literals pcos(const Literals& radian);
Literals psin(const Literals& radian);
Literals ptan(const Literals& radian);

Literals parcsin(const Literals& radian);
Literals parccos(const Literals& radian);
Literals parctan(const Literals& radian);
}  // namespace picolator::math