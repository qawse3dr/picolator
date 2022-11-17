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

#include "expr_tree.h"

#include <stack>

using picolator::math::ExprTree;

ExprTree::ExprTree(const std::vector<Letter>& expr) { createTree(expr, root_); }

float ExprTree::getValue() { return 5; }

void ExprTree::print() {
  // do a breath first search  to print the tree out
  // with the width based on how many leaf nodes there are
  printf("Not impled\n");
}

int ExprTree::countLeafs(const std::unique_ptr<ExprTreeNode>& start_node) {
  int count = 0;

  std::stack<const ExprTreeNode*> node_stack;
  node_stack.push(start_node.get());
  while (!node_stack.empty()) {
    if (node_stack.top()->children.empty()) {
      count++;
      node_stack.pop();
      continue;
    }
    for (const auto& node : node_stack.top()->children) {
      node_stack.push(node.get());
    }
  }

  return count;
}

void ExprTree::createTree(const std::vector<Letter>& expr,
                          const std::unique_ptr<ExprTreeNode>& node) {}
