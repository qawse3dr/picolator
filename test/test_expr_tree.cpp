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

#include <vector>

#include "math/binary_operator.h"
#include "math/bracket.h"
#include "math/expr_tree.h"
#include "math/function.h"
#include "math/literals.h"
#include "math/literals_piece.h"
#include "math/unary_operator.h"

using picolator::math::BinaryOperator;
using picolator::math::Bracket;
using picolator::math::ExprTree;
using picolator::math::Literals;
using picolator::math::LiteralsPiece;

using picolator::math::UnaryOperator;

namespace picolator::math {
class ExprTreeTester {
 public:
  ExprTree isi;

  ExprTreeTester(const ExprTree::ExprVec& expr) : isi(expr) {}

  const ExprTree::ExprTreeNodePtr& getRoot() { return isi.root_; }
  int countLeafs(const ExprTree::ExprTreeNodePtr& start_node) {
    return isi.countLeafs(start_node);
  }
};
};  // namespace picolator::math

TEST(ExprTree, Constructor) { ASSERT_TRUE(true); }

TEST(ExprTree, BlankTree) {
  ExprTree::ExprVec letters;
  picolator::math::ExprTreeTester tree(letters);
  ASSERT_FALSE(tree.getRoot());
  ASSERT_EQ(0, tree.countLeafs(tree.getRoot()));
}

TEST(ExprTree, MinimizeLong) {
  ExprTree::ExprVec letters;
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('1')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('0')));
  picolator::math::ExprTreeTester tree(letters);

  ASSERT_NE(nullptr, tree.getRoot());
  ASSERT_NE(nullptr, tree.getRoot()->current_value);
  ASSERT_EQ(10, tree.getRoot()->current_value->getValue());
}

TEST(ExprTree, MinimizeDouble) {
  ExprTree::ExprVec letters;
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('1')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('0')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('.')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('5')));

  picolator::math::ExprTreeTester tree(letters);

  ASSERT_NE(nullptr, tree.getRoot());
  ASSERT_NE(nullptr, tree.getRoot()->current_value);
  ASSERT_EQ(10.5, tree.getRoot()->current_value->getValue());
}

TEST(ExprTree, SingleInt) {
  ExprTree::ExprVec letters;
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('1')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('0')));
  picolator::math::ExprTreeTester tree(letters);
  ASSERT_TRUE(tree.getRoot());
  ASSERT_EQ(1, tree.countLeafs(tree.getRoot()));
  ASSERT_EQ(tree.isi.getValue()->getValue(), 10.0f);
}

TEST(ExprTree, Minus) {
  ExprTree::ExprVec letters;
  letters.emplace_back(
      ExprTree::LetterPtr(new UnaryOperator("-", UnaryOperator::Type::MINUS)));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('1')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('0')));

  picolator::math::ExprTreeTester tree(letters);
  ASSERT_NE(nullptr, tree.getRoot());
  ASSERT_EQ(1, tree.countLeafs(tree.getRoot()));
  ASSERT_EQ(tree.isi.getValue()->getValue(), -10.0f);
}

TEST(ExprTree, Addition) {
  ExprTree::ExprVec letters;
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('1')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('0')));
  letters.emplace_back(ExprTree::LetterPtr(
      new BinaryOperator("*", BinaryOperator::Type::ADDITION)));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('1')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('5')));

  picolator::math::ExprTreeTester tree(letters);
  ASSERT_TRUE(tree.getRoot());
  ASSERT_EQ(2, tree.countLeafs(tree.getRoot()));
  ASSERT_EQ(tree.isi.getValue()->getValue(), 25.0f);
}

TEST(ExprTree, Addition_With_Minus) {
  ExprTree::ExprVec letters;
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('1')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('0')));
  letters.emplace_back(ExprTree::LetterPtr(
      new BinaryOperator("*", BinaryOperator::Type::ADDITION)));
  letters.emplace_back(
      ExprTree::LetterPtr(new UnaryOperator("-", UnaryOperator::Type::MINUS)));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('1')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('5')));

  picolator::math::ExprTreeTester tree(letters);
  ASSERT_TRUE(tree.getRoot());
  ASSERT_EQ(2, tree.countLeafs(tree.getRoot()));
  ASSERT_EQ(tree.isi.getValue()->getValue(), -5.0f);
}

TEST(ExprTree, Addition_and_Mult) {
  ExprTree::ExprVec letters;
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('1')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('0')));
  letters.emplace_back(ExprTree::LetterPtr(
      new BinaryOperator("+", BinaryOperator::Type::ADDITION)));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('1')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('2')));
  letters.emplace_back(ExprTree::LetterPtr(
      new BinaryOperator("*", BinaryOperator::Type::MULTIPLICATION)));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('1')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('2')));

  picolator::math::ExprTreeTester tree(letters);
  ASSERT_TRUE(tree.getRoot());
  ASSERT_EQ(3, tree.countLeafs(tree.getRoot()));
  ASSERT_EQ(
      BinaryOperator::Type::ADDITION,
      reinterpret_cast<BinaryOperator&>(*(tree.getRoot()->value)).getType());
  ASSERT_EQ(2, tree.getRoot()->children.size());
  ASSERT_EQ(
      BinaryOperator::Type::MULTIPLICATION,
      reinterpret_cast<BinaryOperator&>(*(tree.getRoot()->children[1]->value))
          .getType());
  ASSERT_EQ(10, tree.getRoot()->children[0]->current_value->getValue());

  ASSERT_EQ(tree.isi.getValue()->getValue(), 154.0f);
}

TEST(ExprTree, Addition_and_Mult_Brackets) {
  ExprTree::ExprVec letters;
  letters.emplace_back(ExprTree::LetterPtr(new Bracket(Bracket::Type::OPEN)));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('1')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('0')));
  letters.emplace_back(ExprTree::LetterPtr(
      new BinaryOperator("+", BinaryOperator::Type::ADDITION)));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('1')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('2')));
  letters.emplace_back(ExprTree::LetterPtr(new Bracket(Bracket::Type::CLOSED)));
  letters.emplace_back(ExprTree::LetterPtr(
      new BinaryOperator("*", BinaryOperator::Type::MULTIPLICATION)));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('2')));

  picolator::math::ExprTreeTester tree(letters);
  ASSERT_TRUE(tree.getRoot());
  ASSERT_EQ(
      BinaryOperator::Type::MULTIPLICATION,
      reinterpret_cast<BinaryOperator&>(*(tree.getRoot()->value)).getType());
  ASSERT_EQ(2, tree.getRoot()->children.size());
  ASSERT_EQ(2,
            reinterpret_cast<Literals&>(*(tree.getRoot()->children[1]->value))
                .getValue());
  ASSERT_EQ(
      BinaryOperator::Type::ADDITION,
      reinterpret_cast<BinaryOperator&>(*(tree.getRoot()->children[0]->value))
          .getType());
  ASSERT_EQ(3, tree.countLeafs(tree.getRoot()));

  ASSERT_EQ(tree.isi.getValue()->getValue(), 44.0f);
}

TEST(ExprTree, Bracket_MULT) {
  ExprTree::ExprVec letters;
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('0')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('.')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('5')));
  letters.emplace_back(ExprTree::LetterPtr(new Bracket(Bracket::Type::OPEN)));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('1')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('0')));
  letters.emplace_back(ExprTree::LetterPtr(
      new BinaryOperator("+", BinaryOperator::Type::ADDITION)));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('1')));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('2')));
  letters.emplace_back(ExprTree::LetterPtr(new Bracket(Bracket::Type::CLOSED)));
  letters.emplace_back(ExprTree::LetterPtr(
      new BinaryOperator("*", BinaryOperator::Type::MULTIPLICATION)));
  letters.emplace_back(ExprTree::LetterPtr(new LiteralsPiece('2')));

  picolator::math::ExprTreeTester tree(letters);

  ASSERT_TRUE(tree.getRoot());
  ASSERT_EQ(
      BinaryOperator::Type::MULTIPLICATION,
      reinterpret_cast<BinaryOperator&>(*(tree.getRoot()->value)).getType());
  ASSERT_EQ(2, tree.getRoot()->children.size());
  ASSERT_EQ(0.5,
            reinterpret_cast<Literals&>(*(tree.getRoot()->children[0]->value))
                .getValue());
  ASSERT_EQ(
      BinaryOperator::Type::ADDITION,
      reinterpret_cast<BinaryOperator&>(*(tree.getRoot()->children[0]->value))
          .getType());

  ASSERT_EQ(tree.isi.getValue()->getValue(), 22.0f);
}

TEST(ExprTree, sin_pi) {
  ExprTree::ExprVec letters;
  letters.emplace_back(
      ExprTree::LetterPtr(new UnaryOperator("sin", UnaryOperator::Type::SIN)));
  letters.emplace_back(ExprTree::LetterPtr(new Bracket(Bracket::Type::OPEN)));
  letters.emplace_back(ExprTree::LetterPtr(new Literals(Literals::Type::PI)));

  letters.emplace_back(ExprTree::LetterPtr(new Bracket(Bracket::Type::CLOSED)));
  picolator::math::ExprTreeTester tree(letters);

  ASSERT_EQ(tree.isi.getValue()->getValue(), 0);
}

TEST(ExprTree, pi) {
  ExprTree::ExprVec letters;
  letters.emplace_back(ExprTree::LetterPtr(new Literals(Literals::Type::PI)));
  picolator::math::ExprTreeTester tree(letters);

  // should be < e but I am lazy
  ASSERT_TRUE(std::abs(tree.isi.getValue()->getValue() -
                       picolator::math::PI::value) < 0.001);
}

TEST(ExprTree, cos_pi) {
  ExprTree::ExprVec letters;
  letters.emplace_back(
      ExprTree::LetterPtr(new UnaryOperator("cos", UnaryOperator::Type::COS)));
  letters.emplace_back(ExprTree::LetterPtr(new Bracket(Bracket::Type::OPEN)));
  letters.emplace_back(
      ExprTree::LetterPtr(new Literals(Literals::Type::PI, 1L, 1L)));

  letters.emplace_back(ExprTree::LetterPtr(new Bracket(Bracket::Type::CLOSED)));
  picolator::math::ExprTreeTester tree(letters);

  ASSERT_EQ(tree.isi.getValue()->getValue(), -1);
}