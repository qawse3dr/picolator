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

namespace picolator::math {

// NOT will throw Exceptions
// if invalid values are given

class DomainError : public std::exception {
  const char* what() const throw() { return "Domain error"; }
};

const int TAYLOR_ITERATIONS = 20;

double pcos(double radian);
double psin(double radian);
double ptan(double radian);

double parcsin(double radian);
double parccos(double radian);
double parctan(double radian);
}  // namespace picolator::math