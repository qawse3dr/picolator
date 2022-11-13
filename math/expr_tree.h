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

#include "letter.h"

namespace calculator::math {

class ExprTree {
 private:
  struct ExprTreeNode {
    std::vector<std::unique_ptr<ExprTreeNode>> children;
    std::shared_ptr<Letter> value;
  };

  std::unique_ptr<ExprTreeNode> root_ = {};

  // Helper for print function simply counts how many leaf nodes
  // there are so printing can be done easier
  int countLeafs(const std::unique_ptr<ExprTreeNode>& start_node);

  void createTree(const std::vector<Letter>& expr,
                  const std::unique_ptr<ExprTreeNode>& node);

 public:
  ExprTree(const std::vector<Letter>& expr);

  // Gets the value of the tree
  float getValue();

  // prints a pretty version of the tree
  void print();
};
}  // namespace calculator::math