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

TEST(LiteralsTest, Addition) {
  ASSERT_EQ((Literals(10) + Literals(5)).getLong(), 15);
  ASSERT_EQ((Literals(2, 5) + Literals(3, 5)).getDouble(), 1);
}