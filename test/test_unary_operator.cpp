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
using picolator::math::UnaryOperator;

TEST(UnaryOperatorTest, Constructor) {
  UnaryOperator op("sin", UnaryOperator::UnaryOperatorType::SIN);

  ASSERT_EQ("sin", op.getValue());
  ASSERT_EQ(Letter::Classification::UNARY, op.getClassification());
  ASSERT_EQ(UnaryOperator::UnaryOperatorType::SIN, op.getOp());
}

TEST(UnaryOperatorTest, MINUS) {
  UnaryOperator op("-", UnaryOperator::UnaryOperatorType::MINUS);

  ASSERT_EQ(-10, op.solve(10));
  ASSERT_EQ(10, op.solve(-10));
}

TEST(UnaryOperatorTest, SIN) {
  UnaryOperator op("sin", UnaryOperator::UnaryOperatorType::SIN);

  EXPECT_EQ(0, op.solve(0));
  EXPECT_EQ(0, op.solve(2.0 * M_PI));
  EXPECT_EQ(1, op.solve(M_PI / 2.0));
}

TEST(UnaryOperatorTest, COS) {
  UnaryOperator op("cos", UnaryOperator::UnaryOperatorType::COS);

  EXPECT_EQ(1, op.solve(0));
  EXPECT_EQ(1, op.solve(2 * M_PI));
  EXPECT_EQ(0, op.solve(M_PI / 2.0));
}
