#include <stdexcept>
#include <iostream>
#include <string>
#include "tree_print.h"
#include "ShimmerClasses.h"

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
  std::cout << "Printing a DotTree\n";

  for (DotStatement st : tree.get_tree()) {
    pretty_print(st);
  }
}

void pretty_print(DotStatement statement) {
  std::cout << "Printing a DotStatement\n";

  for (ShimmerParam pa : statement.get_params()) {
    std::vector<bool> flags(100, true);
    pretty_print(pa, flags, 0, false);
  }
}

void pretty_print(ShimmerParam param, std::vector<bool> is_exploring,
                  int depth, bool is_last) {
  std::cout << "Printing a ShimmerParam\n";

  if (param.not_of_type(STATEMENT)) {
    if (depth == 0) {
      pretty_print(param);
    }
    else if (is_last) { 
      std::cout << "\u2514\u2500\u2500\u2500 ";
      pretty_print(param);
      std::cout << "\n";
      is_exploring.at(depth) = false;
    } 
    else { 
      std::cout << "\u251c\u2500\u2500\u2500 ";
      pretty_print(param);
      std::cout << "\n"; 
    }

    return;
  }

  for (int i = 1; i < depth; i++) {
    if (is_exploring.at(i)) {
      std::cout << "\u2502\t\t\t";
    }
    else {
      std::cout << " \t\t\t";
    }
  }

  int param_id = 0;

  DotStatement st = param.get_statement_val();

  for (ShimmerParam pa : st.get_params()) {
    pretty_print(pa, is_exploring, depth + 1, param_id++ == (st.get_params().size()) - 1);
  }

  is_exploring.at(depth) = true;
}

void pretty_print(ShimmerParam param) {
  std::cout << "Printing a ShimmerParam VALUE\n";

  switch(param.get_param_type()) {
    case NONETYPE:
      std::cout << "<untyped param>\n";
      break;

    case LITERAL:
      std::cout << "There was a LITERAL\n";
      pretty_print(param.get_literal_val());
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
  std::cout << "Printing a DotLiteral\n";

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

/*
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
*/
