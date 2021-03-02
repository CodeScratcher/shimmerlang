#include <stdexcept>
#include <iostream>
#include <string>
#include "tree_print.h"
#include "ShimmerClasses.h"

void pretty_print(DotTree tree) {
  for (DotStatement st : tree.get_tree()) {
    pretty_print(st, 0);
  }
}

void pretty_print(DotStatement statement, int depth) {
  pretty_print(statement.get_expr(), depth);

  for (ShimmerExpr expr : statement.get_params()) {
    pretty_print(expr, depth + 1);
  }
}

void pretty_print(ShimmerExpr expr, int depth) {
  if (expr.is_of_type(STATEMENT)) {
    pretty_print(expr.get_statement_val(), depth);
  }
  else {
    for (int i = 0; i < depth; ++i) {
      std::cout << "\t";
    }

    switch(expr.get_param_type()) {
      case NONETYPE:
        std::cout << "<untyped expr>\n";
        break;

      case LITERAL:
        pretty_print(expr.get_literal_val(), depth + 1);
        break;

      case IDENTIFIER:
        std::cout << expr.get_identifier_val().get_contents();
        break;

      default:
        throw std::runtime_error("Illegal expr type!");
    }

    std::cout << "\n";
  }
}

void pretty_print(DotLiteral lit, int depth) {
  int literal_type = lit.get_type();

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
  std::cout << "\n";

  std::cout << "Pretty printing string literal <\"Hello, world!\">...\n";
  pretty_print(str_lit_2);
  std::cout << "\n";

  std::cout << "Pretty printing int literal <3>...\n";
  pretty_print(int_lit_1);
  std::cout << "\n";

  std::cout << "Pretty printing int literal <42>...\n";
  pretty_print(int_lit_2);
  std::cout << "\n";

  std::cout << "Pretty printing bool literal <true>...\n";
  pretty_print(bool_lit_1);
  std::cout << "\n";

  std::cout << "Pretty printing bool literal <false>...\n";
  pretty_print(bool_lit_2);
  std::cout << "\n";

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
