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

#include <cmath>

#include "math/unary_operator.h"

using picolator::math::Letter;
using picolator::math::Literals;

using picolator::math::UnaryOperator;

TEST(UnaryOperatorTest, Constructor) {
  UnaryOperator op("sin", UnaryOperator::Type::SIN);

  ASSERT_EQ("sin", op.getSymbol());
  ASSERT_EQ(Letter::Classification::UNARY, op.getClassification());
  ASSERT_EQ(UnaryOperator::Type::SIN, op.getOp());
}

TEST(UnaryOperatorTest, MINUS) {
  UnaryOperator op("-", UnaryOperator::Type::MINUS);

  ASSERT_EQ(Literals(-10), op.solve(10));
  ASSERT_EQ(Literals(10), op.solve(-10));
}

TEST(UnaryOperatorTest, SIN) {
  UnaryOperator op("sin", UnaryOperator::Type::SIN);

  EXPECT_EQ(Literals(0), op.solve(0));
  EXPECT_EQ(Literals(0), op.solve(2.0 * M_PI));
  EXPECT_EQ(Literals(1), op.solve(M_PI / 2.0));
}

TEST(UnaryOperatorTest, COS) {
  UnaryOperator op("cos", UnaryOperator::Type::COS);

  EXPECT_EQ(Literals(1), op.solve(0));
  EXPECT_EQ(Literals(1), op.solve(2 * M_PI));
  EXPECT_EQ(Literals(0), op.solve(M_PI / 2.0));
}
