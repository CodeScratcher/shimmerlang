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
    pretty_print(pa, is_exploring, depth + 1, param_id == (st.get_params().size() - 1));
    param_id++;
  }
  is_exploring.at(depth) = true;
}

std::string pretty_print(ShimmerParam param) {
  switch(param.get_param_type()) {
    case NONETYPE:
      std::cout << "<untyped param>\n";
      break;

    case LITERAL:
      pretty_print(param.get_literal_val());
      break;

    case IDENTIFIER:
      std::cout << param.get_identifier_val().get_contents();
      break;

    default:
      throw std::runtime_error("Illegal param type!");
  }
}

std::string pretty_print(DotLiteral lit) {
  int literal_type = lit.get_type();
  //std::cout << "literal_type: " << std::to_string(literal_type) << "\n";
  return "";

  switch (literal_type) {
    case TypeString:
      std::cout << lit.get_str();
      break;

    case TypeInt:
      std::cout << std::to_string(lit.get_int());
      break;

    case TypeBool:
      std::cout << (lit.get_bool() ? "true" : "false");
      break;

    case TypeFunc:
      throw std::runtime_error("Hasn't been implemented yet: pretty printing funcs");
      break;

    case TypeId:
      std::cout << lit.get_id().get_contents();
      break;
  }
}

void test_pretty_print_literal() {
  DotLiteral str_lit_1(-1, "Hi there");
  DotLiteral str_lit_2(-1, "Hello world!");
  DotLiteral int_lit_1(-1, 3);
  DotLiteral int_lit_2(-1, 42);
  DotLiteral bool_lit_1(-1, true);
  DotLiteral bool_lit_2(-1, false);

  // TODO
  //DotLiteral id_lit_1(...);
  //DotLiteral id_lit_2(...);

  std::cout << "Pretty printing string literal <\"Hi there\">...\n";
  pretty_print(str_lit_1);

  std::cout << "Pretty printing string literal <\"Hello, world!\">...\n";
  pretty_print(str_lit_2);

  std::cout << "Pretty printing int literal <3>...\n";
  pretty_print(int_lit_1);

  std::cout << "Pretty printing int literal <42>...\n";
  pretty_print(int_lit_2);

  std::cout << "Pretty printing string literal <true>...\n";
  pretty_print(bool_lit_1);

  std::cout << "Pretty printing string literal <false>...\n";
  pretty_print(bool_lit_2);

  // TODO

  // std::cout << "Pretty printing id literal \"Hi there\"...\n";
  // pretty_print(id_lit_1);

  // std::cout << "Pretty printing id literal \"Hi there\"...\n";
  // pretty_print(id_lit_1);

  // std::cout << "Testing pretty printing a function literal...\n";
  // bool caught = false;

  // try {
  //   pretty_print(func_lit);
  // }
  // catch (std::runtime_error e) {
  //   std::cout << "Caught error:\n" << e.what() << "\n";
  //   std::cout << "PASSED!\n";
  //   caught = true;
  // }

  // if (!caught) {
  //   std::cout << "FAILED!\n";
  // }
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
