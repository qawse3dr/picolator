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
#include <cmath>
#include <cstdint>
#include <limits>

#include "math_util.h"

// todo remove
#include <iostream>

namespace picolator::math {

/**
 * @brief Returns special values of sine cos and tan
 * because std libs aren't good enough
 *
 * @param res
 * @return double
 */
double specialSineValues(double res) {
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
  return specialSineValues(cos(radian.getDouble()));
  // double result = 1;

  // // Keeps track of the negative in the taylor poylinomial
  // int negate = -1;

  // // keeps track of the current raidan power in taylor
  // double radiansPower = radian * radian;

  // int64_t fact = 2;
  // for (int i = 2; i < TAYLOR_ITERATIONS; i += 2) {
  //   result += (negate * radiansPower / fact);

  //   // update taylor info
  //   fact *= (i + 1) * (i + 2);
  //   negate *= -1;
  //   radiansPower *= (radian * radian);
  // }
  // return result;
}

Literals psin(const Literals& radian) {
  return specialSineValues(sin(radian.getDouble()));
  // double result = 0;

  // // Keeps track of the negative in the taylor poylinomial
  // int negate = 1;

  // // keeps track of the current raidan power in taylor
  // double radiansPower = radian;

  // int64_t fact = 1;
  // for (int i = 1; i < TAYLOR_ITERATIONS; i += 2) {
  //   result += (negate * radiansPower / fact);

  //   // update taylor info
  //   fact *= (i + 1) * (i + 2);
  //   negate *= -1;
  //   radiansPower *= (radian * radian);
  // }
  // return result;
}

Literals ptan(const Literals& radian) {
  // Check if its
  // pi/2
  return specialSineValues(tan(radian.getDouble()));
}

}  // namespace picolator::math