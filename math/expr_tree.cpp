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

#include <algorithm>
#include <queue>
#include <stack>

#include "binary_operator.h"
#include "bracket.h"
#include "literals.h"
#include "literals_piece.h"
#include "math_util.h"
#include "unary_operator.h"

using picolator::math::BinaryOperator;
using picolator::math::Bracket;
using picolator::math::ExprTree;
using picolator::math::Letter;
using picolator::math::Literals;
using picolator::math::LiteralsPiece;
using picolator::math::UnaryOperator;

ExprTree::ExprTree(const ExprTree::ExprVec& expr) {
  root_ = std::move(createTree(minimizeTreeInput(expr)));
}

// checks if letter has a value
#define LETTER_PTR_HAS_VALUE(l)                                        \
  (l->value->getClassification() == Letter::Classification::LITERAL || \
   l->current_value != nullptr)

#define LETTER_PTR_GET_VALUE(l)        \
  ((l->current_value != nullptr)       \
       ? l->current_value->getDouble() \
       : reinterpret_cast<const Literals&>(*l->value).getDouble())

float ExprTree::getValue() {
  std::stack<ExprTreeNode*> node_stack;
  node_stack.emplace(root_.get());

  while (!node_stack.empty()) {
    // This is a value pop off the stack nothing to do here
    if (LETTER_PTR_HAS_VALUE(node_stack.top())) {
      node_stack.pop();
      continue;
    }
    switch (node_stack.top()->value->getClassification()) {
      case Letter::Classification::LITERAL:
        // Set the current literals to current value
        node_stack.top()->current_value = std::make_shared<Literals>(
            *reinterpret_cast<Literals*>(node_stack.top()->value.get()));
        break;
      case Letter::Classification::BINARY: {
        // TODO create var of state

        // Check if we can calculate binary
        bool has_vals = true;
        if (!LETTER_PTR_HAS_VALUE(node_stack.top()->children[0])) {
          node_stack.push(node_stack.top()->children[0].get());
          has_vals = false;
        }

        if (!LETTER_PTR_HAS_VALUE(node_stack.top()->children[1])) {
          node_stack.push(node_stack.top()->children[1].get());
          has_vals = false;
        }

        if (has_vals) {
          auto& binary_op =
              reinterpret_cast<BinaryOperator&>(*(node_stack.top()->value));
          node_stack.top()->current_value = std::make_shared<Literals>(
              binary_op.solve(*node_stack.top()->children[0]->current_value,
                              *node_stack.top()->children[1]->current_value));
        }
      } break;
      case Letter::Classification::UNARY:
        // TODO create var of state

        // Check if we can calculate binary
        if (!LETTER_PTR_HAS_VALUE(node_stack.top()->children[0])) {
          node_stack.push(node_stack.top()->children[0].get());
        } else {
          auto& unary_op =
              reinterpret_cast<UnaryOperator&>(*(node_stack.top()->value));
          node_stack.top()->current_value = std::make_shared<Literals>(
              unary_op.solve(*node_stack.top()->children[0]->current_value));
        }

        break;
    }
  }
  return LETTER_PTR_GET_VALUE(root_);
}

void ExprTree::print() {
  // do a breath first search  to print the tree out
  // with the width based on how many leaf nodes there are
  printf("Not impled\n");
}

int ExprTree::countLeafs(
    const std::unique_ptr<ExprTreeNode>& start_node) const {
  int count = 0;

  if (start_node) {
    std::queue<const ExprTreeNode*> node_stack;
    node_stack.push(start_node.get());
    while (!node_stack.empty()) {
      if (node_stack.front()->children.empty()) {
        count++;
      } else {
        for (const auto& node : node_stack.front()->children) {
          node_stack.push(node.get());
        }
      }
      node_stack.pop();
    }
  }

  return count;
}

// Init tree putting all literals into 1 object and grouping brackets into 1
// Letter
ExprTree::ExprVec ExprTree::minimizeTreeInput(const ExprTree::ExprVec& expr) {
  ExprVec minimized_literal_input;

  // copy the current vector input into our minimized_literal_input chopping off
  // any literals where we can
  std::string cur_literal = "";
  bool has_decimal = false;
  for (const auto& l : expr) {
    if (l->getClassification() != Letter::Classification::LITERAL_PIECE) {
      if (!cur_literal.empty()) {
        if (has_decimal) {
          minimized_literal_input.emplace_back(
              new Literals(std::stod(cur_literal)));
        } else {
          minimized_literal_input.emplace_back(
              new Literals(std::stol(cur_literal)));
        }
        cur_literal = "";
        has_decimal = false;
        if (l->getClassification() == Letter::Classification::BRACKET &&
                reinterpret_cast<const Bracket&>(*l).getType() ==
                    Bracket::Type::OPEN ||
            l->getClassification() == Letter::Classification::LITERAL) {
          // add a mult symbol
          minimized_literal_input.emplace_back(
              new BinaryOperator("*", BinaryOperator::Type::MULTIPLICATION));
        }
      }
      minimized_literal_input.push_back(l);
    } else {
      const auto& lit = reinterpret_cast<const LiteralsPiece&>(*l);
      if (lit.value_ == '.') {
        has_decimal = true;
      }
      cur_literal += static_cast<char>(lit.value_);
    }
  }
  if (cur_literal != "") {
    if (has_decimal) {
      minimized_literal_input.emplace_back(
          new Literals(std::stod(cur_literal)));
    } else {
      minimized_literal_input.emplace_back(
          new Literals(std::stol(cur_literal)));
    }
  }

  // go through the minimized input again but getting rid of brackets
  ExprVec minimized_input;
  std::stack<ExprVec> bracket_stack;

  for (const auto& l : minimized_literal_input) {
    if (l->getClassification() == Letter ::Classification::BRACKET &&
        reinterpret_cast<const Bracket&>(*l).getType() !=
            Bracket::Type::EQUATION) {
      if (reinterpret_cast<const Bracket&>(*l).getType() ==
          Bracket::Type::OPEN) {
        bracket_stack.emplace(ExprVec());
      } else {
        if (bracket_stack.size() == 1) {
          minimized_input.emplace_back(
              LetterPtr(new Bracket(std::move(bracket_stack.top()))));
          bracket_stack.pop();
        } else {
          Bracket* brack = new Bracket(std::move(bracket_stack.top()));
          bracket_stack.pop();
          bracket_stack.top().emplace_back(std::move(brack));
        }
      }
    } else {
      if (bracket_stack.empty()) {
        minimized_input.push_back(l);
      } else {
        bracket_stack.top().push_back(l);
      }
    }
  }

  if (bracket_stack.size() == 1) {
    minimized_input.emplace_back(
        LetterPtr(new Bracket(std::move(bracket_stack.top()))));
  }

  if (bracket_stack.size() > 2) {
    throw SyntaxError("", 0);
  }

  return minimized_input;
}

ExprTree::ExprTreeNodePtr ExprTree::createTree(const ExprTree::ExprVec& expr) {
  if (expr.empty()) {
    return nullptr;  // no expresion to parse
  }

  if (expr.size() == 1) {
    if (expr[0]->getClassification() == Letter::Classification::LITERAL) {
      ExprTreeNodePtr node(new ExprTreeNode);
      node->value = expr[0];
      node->current_value =
          std::make_shared<Literals>(reinterpret_cast<Literals&>(*expr[0]));
      return node;
    } else if (expr[0]->getClassification() ==
               Letter::Classification::BRACKET) {
      return createTree(reinterpret_cast<Bracket&>(*(expr[0])).letters_);
    }
  }

  // Find the letter with the highest priority then split the tree
  // into 2 vecs one with before it and one after
  auto op_it = std::max_element(expr.begin(), expr.end(),
                                [](const LetterPtr& l1, const LetterPtr& l2) {
                                  return l1->getPriority() < l2->getPriority();
                                });
  size_t op_pos = std::distance(expr.begin(), op_it);

  if (expr[op_pos]->getClassification() == Letter::Classification::BINARY) {
    ExprVec lhs;

    if (op_pos == 0 || op_pos == expr.size() - 1) {
      throw picolator::math::SyntaxError("", 0);
    }

    for (auto it = expr.begin(); it < op_it; it++) {
      lhs.push_back(*it);
    }
    ExprVec rhs;  // std::copy doesn't seem to work
    for (auto it = op_it + 1; it < expr.end(); it++) {
      rhs.push_back(*it);
    }

    // get nodes
    ExprTreeNodePtr node(new ExprTreeNode);
    node->children.emplace_back(createTree(lhs));
    node->children.emplace_back(createTree(rhs));
    node->value = *op_it;
    return node;
  } else if (expr[op_pos]->getClassification() ==
             Letter::Classification::UNARY) {
    if (op_pos != 0) {
      throw picolator::math::SyntaxError("", 0);
    } else {
      ExprVec rhs;
      for (auto it = op_it + 1; it < expr.end(); it++) {
        rhs.push_back(*it);
      }

      ExprTreeNodePtr node(new ExprTreeNode);
      node->children.emplace_back(createTree(rhs));
      node->value = *op_it;
      return node;
    }
  } else if (expr[op_pos]->getClassification() ==
             Letter::Classification::BRACKET) {
    const Bracket& brack = reinterpret_cast<const Bracket&>(**op_it);
    return createTree(brack.letters_);
  } else {  // invalid op not sure what I should do here
    throw std::exception();
  }
}
