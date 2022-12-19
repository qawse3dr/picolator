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
#include <memory>
#include <vector>

#include "letter.h"
#include "literals.h"

namespace picolator::math {

class ExprTree {
 public:
  using LetterPtr = std::shared_ptr<Letter>;
  using LiteralPtr = std::shared_ptr<Literals>;

  using ExprVec = std::vector<LetterPtr>;

 private:
  struct ExprTreeNode {
    std::vector<std::unique_ptr<ExprTreeNode>> children;
    LetterPtr value;
    LiteralPtr current_value = nullptr;
  };
  using ExprTreeNodePtr = std::unique_ptr<ExprTreeNode>;

  ExprTreeNodePtr root_ = {};

  // Helper for print function simply counts how many leaf nodes
  // there are so printing can be done easier
  int countLeafs(const std::unique_ptr<ExprTreeNode>& start_node) const;

  // takes in a current expanded tree input and minimize it's literals and
  // brackets to make solving easier
  ExprVec minimizeTreeInput(const ExprVec& expr);
  ExprTreeNodePtr createTree(const ExprVec& expr);

 public:
  ExprTree(const ExprVec& expr);

  /**
   * @brief Get the float value of the tree
   *
   * @return float
   */
  LiteralPtr getValue();

  // prints a pretty version of the tree
  void print();

  friend class ExprTreeTester;
};
}  // namespace picolator::math