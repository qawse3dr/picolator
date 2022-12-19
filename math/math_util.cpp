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
#include "math_util.h"

#include <cmath>
#include <cstdint>
#include <limits>

namespace picolator::math {

/**
 * @brief Returns special values of sine cos and tan
 * because std libs aren't good enough
 *
 * @param res
 * @return double
 */
Literals specialSineValues(double res) {
  if (std::fabs(1 - res) < std::numeric_limits<double>::epsilon() * 2) {
    return 1;
  } else if (std::fabs(-1 - res) < std::numeric_limits<double>::epsilon() * 2) {
    return -1;
  } else if (std::fabs(res) < std::numeric_limits<double>::epsilon() * 2) {
    return 0;
  }
  // todo add special angles
  return res;
}

Literals pcos(const Literals& radian) {
  return specialSineValues(cos(radian.getValue()));
}

Literals psin(const Literals& radian) {
  return specialSineValues(sin(radian.getValue()));
}

Literals ptan(const Literals& radian) {
  // Check if its
  // pi/2
  return specialSineValues(tan(radian.getValue()));
}

Literals parccos(const Literals& x) {
  return specialSineValues(acos(x.getValue()));
}

Literals parcsin(const Literals& x) {
  return specialSineValues(asin(x.getValue()));
}

Literals parctan(const Literals& x) {
  // Check if its
  // pi/2
  return specialSineValues(atan(x.getValue()));
}

}  // namespace picolator::math