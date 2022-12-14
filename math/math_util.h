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
  const char* what() const throw() { return "Domain error"; }
};

class SyntaxError : public std::exception {
  const char* what() const throw() { return "Domain error"; }
};

Literals pcos(const Literals& radian);
Literals psin(const Literals& radian);
Literals ptan(const Literals& radian);

Literals parcsin(const Literals& radian);
Literals parccos(const Literals& radian);
Literals parctan(const Literals& radian);
}  // namespace picolator::math