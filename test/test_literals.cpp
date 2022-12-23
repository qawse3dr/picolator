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

TEST(LiteralsTest, getValue) {
  ASSERT_EQ(Literals(10).getValue(), 10);
  ASSERT_EQ(Literals(2, 5).getValue(), 2.0 / 5.0);
  ASSERT_EQ(Literals(0.5).getValue(), 0.5);
  ASSERT_EQ(Literals(Literals::Type::PI, 3, 1).getValue(),
            3 * picolator::math::PI::value);
  ASSERT_EQ(Literals(Literals::Type::PI, 1, 3).getValue(),
            pow(picolator::math::PI::value, 3));
  ASSERT_EQ(Literals(Literals::Type::PI, 3, 4).getValue(),
            3 * pow(picolator::math::PI::value, 4));
}

TEST(LiteralsTest, Addition) {
  // Long
  ASSERT_EQ((Literals(10) + Literals(5)).getValue(), 15);

  // fraction
  ASSERT_EQ((Literals(2, 5) + Literals(3, 5)).getValue(), 1);
  ASSERT_EQ((Literals(2, 5) + Literals(3)), Literals(17, 5));
  ASSERT_EQ((Literals(3) + Literals(2, 5)), Literals(17, 5));

  // fraction different bases
  ASSERT_EQ((Literals(1, 7) + Literals(-9, 5)), Literals(-58, 35));

  // double
  ASSERT_EQ(Literals(0.5) + Literals(0.5), Literals(1));

  ASSERT_EQ((Literals(42.42069) + Literals(3)).getValue(),
            Literals(42.42069 + 3).getValue());

  // Const
  ASSERT_EQ(
      Literals(Literals::Type::PI, 13, 1) + Literals(Literals::Type::PI, 4, 1),
      Literals(Literals::Type::PI, 17, 1));

  // Const with power
  ASSERT_EQ(
      (Literals(Literals::Type::PI, 1, 3) + Literals(Literals::Type::PI, 1, 5))
          .getValue(),
      pow(picolator::math::PI::value, 3) + pow(picolator::math::PI::value, 5));
  ASSERT_EQ(
      (Literals(Literals::Type::PI) + Literals(Literals::Type::PI)).getValue(),
      2 * picolator::math::PI::value);
}

// Tests that Variables work as expected
TEST(LiteralsTest, Variable) {
  Literals::getVariable('A') = Literals(10);
  ASSERT_EQ(Literals('A').getValue(), 10);
}
