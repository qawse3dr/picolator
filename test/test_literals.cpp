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

#include <gtest/gtest.h>

#include "math/literals.h"

using picolator::math::Literals;

TEST(LiteralsTest, getDouble) {
  ASSERT_EQ(Literals(10).getDouble(), 10);
  ASSERT_EQ(Literals(2, 5).getDouble(), 2.0 / 5.0);
  ASSERT_EQ(Literals(0.5).getDouble(), 0.5);
  ASSERT_EQ(Literals(Literals::Type::PI, 3, 1).getDouble(),
            3 * picolator::math::PI::value);
  ASSERT_EQ(Literals(Literals::Type::PI, 1, 3).getDouble(),
            pow(picolator::math::PI::value, 3));
  ASSERT_EQ(Literals(Literals::Type::PI, 3, 4).getDouble(),
            3 * pow(picolator::math::PI::value, 4));
}

TEST(LiteralsTest, Addition) {
  // Long
  ASSERT_EQ((Literals(10) + Literals(5)).getLong(), 15);

  // fraction
  ASSERT_EQ((Literals(2, 5) + Literals(3, 5)).getDouble(), 1);
  ASSERT_EQ((Literals(2, 5) + Literals(3)), Literals(17, 5));
  ASSERT_EQ((Literals(3) + Literals(2, 5)), Literals(17, 5));

  // fraction different bases
  ASSERT_EQ((Literals(1, 7) + Literals(-9, 5)), Literals(-58, 35));

  // double
  ASSERT_EQ(Literals(0.5) + Literals(0.5), Literals(1));

  ASSERT_EQ((Literals(42.42069) + Literals(3)).getDouble(),
            Literals(42.42069 + 3).getDouble());

  // Const
  ASSERT_EQ(
      Literals(Literals::Type::PI, 13, 1) + Literals(Literals::Type::PI, 4, 1),
      Literals(Literals::Type::PI, 17, 1));

  // Const with power
  ASSERT_EQ(
      (Literals(Literals::Type::PI, 1, 3) + Literals(Literals::Type::PI, 1, 5))
          .getDouble(),
      pow(picolator::math::PI::value, 3) + pow(picolator::math::PI::value, 5));
}
