#include <stdexcept>
#include <iostream>
#include <string>
#include "tree_print.h"
#include "ShimmerClasses.h"

// Copied from SO
/*
  void printBT(const std::string& prefix, const BSTNode* node, bool isLeft)
  {
      if( node != nullptr )
      {
          std::cout << prefix;

          std::cout << (isLeft ? "├──" : "└──" );

          // print the value of the node
          std::cout << node->m_val << std::endl;

          // enter the next tree level - left and right branch
          printBT( prefix + (isLeft ? "│   " : "    "), node->m_left, true);
          printBT( prefix + (isLeft ? "│   " : "    "), node->m_right, false);
      }
  }

  void printBT(const BSTNode* node)
  {
      printBT("", node, false);    
  }
*/

// Sample/Expected Tree
/*
 * a
 * |--------- a1
 * |          |------------ a1a
 * |          |             |-------------- a1a1
 * |          |             |-------------- a1a2
 * |          |             |______________ a1a3
 * |          |                             |______________ etc.
 * |          |------------ a1b
 * |          |             |-------------- a1b1
 * |          |             |-------------- a1b2
 * |          |             |______________ a1b3
 * |          |                             |______________ etc.
 * |          |____________ a1c
 * |                        |-------------- a1c1
 * |                        |-------------- a1c2
 * |                        |______________ a1c3
 * |                                        |______________ etc.
 * |--------- a2
 * |          |------------ a2a
 * |          |             |-------------- a2a1
 * |          |             |-------------- a2a2
 * |          |             |______________ a2a3
 * |          |                             |______________ etc.
 * |          |------------ a2b
 * |          |             |-------------- a2b1
 * |          |             |-------------- a2b2
 * |          |             |______________ a2b3
 * |          |                             |______________ etc.
 * |          |____________ a2c
 * |                        |-------------- a2c1
 * |                        |-------------- a2c2
 * |                        |______________ a2c3
 * |                                        |______________ etc.
 * |_________ a3
 *            |------------ a3a
 *            |             |-------------- a3a1
 *            |             |-------------- a3a2
 *            |             |______________ a3a3
 *            |                             |______________ etc.
 *            |------------ a3b
 *            |             |-------------- a3b1
 *            |             |-------------- a3b2
 *            |             |______________ a3b3
 *            |                             |______________ etc.
 *            |____________ a3c
 *                          |-------------- a3c1
 *                          |-------------- a3c2
 *                          |______________ a3c3
 *                                          |______________ etc.
 *
**/

void pretty_print(DotTree tree) {
  if (tree.tree.size() > 0) {
    for (DotStatement statement : tree.get_tree()) {
      for (ShimmerParam param : statement.get_params()) {
        pretty_print(param);
      }
    }
  }
}

void pretty_print(DotStatement statement, int indent) {
  std::cout << "Now printing a Statement\n";
  for (ShimmerParam param : statement.get_params()) {
    pretty_print(param, indent);
  }
}

void pretty_print(ShimmerParam param, int indent) {
  std::cout << "Now printing a Param\n";
  for (int i = 0; i < indent; i++) {
    std::cout << "  ";
  }

  switch(param.get_param_type()) {
    case NONETYPE:
      std::cout << "<untyped param>\n";
      break;

    case LITERAL:
      std::cout << "There was a LITERAL\n";
      pretty_print(param.get_literal_val());
      break;

    case STATEMENT:
      std::cout << "There was a STATEMENT\n";
      pretty_print(param.get_statement_val(), indent);
      break;

    case IDENTIFIER:
      std::cout << "There was a IDENTIFIER\n";
      std::cout << param.get_identifier_val().get_contents() << "\n";
      break;

    default:
      throw std::runtime_error("Illegal param type!");
  }
}

void pretty_print(DotLiteral lit) {
  std::cout << "Now printing a Literal\n";
  int literal_type = lit.get_type();

  switch (literal_type) {
    case TypeString:
      std::cout << lit.get_str() << "\n";
      break;

    case TypeInt:
      std::cout << std::to_string(lit.get_int()) << "\n";
      break;

    case TypeBool:
      throw std::runtime_error("Hasn't been implemented yet: pretty printing bools");
      break;

    case TypeFunc:
      throw std::runtime_error("Hasn't been implemented yet: pretty printing funcs");
      break;

    case TypeId:
      throw std::runtime_error("Hasn't been implemented yet: pretty printing IDs");
      break;
  }
}
