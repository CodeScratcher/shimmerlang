#include <iostream>
#include <stdexcept>
#include <string>

#include "ShimmerClasses.h"
#include "tree_print.h"

void pretty_print(ShimmerTree tree) {
  for (ShimmerExpr st : tree.get_tree()) {
    if (st.is_of_type(STATEMENT)) pretty_print(st.get_statement_val(), 0);
    else {
      pretty_print(st, 0);
    }
  }
}

void pretty_print(ShimmerStatement statement, int depth) {
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

      case FUNCTION:
        pretty_print(expr.get_func_val(), depth);
        break;

      default:
        throw std::runtime_error("Illegal expr type!");
    }

    std::cout << "\n";
  }
}

void pretty_print(ShimmerLiteral lit, int depth) {
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
      pretty_print(lit.get_func(), depth);
      break;

    case TypeId:
      std::cout << lit.get_id().get_contents();
      break;
  }
}

void pretty_print(ShimmerClosedFunc func, int depth) {
  std::cout << "<function>\n";
  pretty_print(func.tree);
}
void pretty_print(ShimmerUnclosedFunc func, int depth) {
  std::cout << "<function>\n";
  pretty_print(func.tree);
}

/*
 *
 * Basically pretty_print(shsc) recursively calls itself on shsc->upper_scope until shsc_upper_scope == nullptr
 * Then it goes back DOWN the call stack again to actually do the printing.
 *
 * The reason for this is that pretty_print needs to be able to print the right indentation, so it needs to know the topmost shsc.
 *
 * Expected output:
 *
 * Begin scope {
     foo: 5
     bar: 10
     baz: -1
     quux: "yeet"
     Begin subscope {
       [OVERRIDES] foo: true
       msg: "Hello, world!"
       fn: [Function]
       Begin subscope {
         ...
       } End subscope
     } End subscope
 * } End scope
 *
 *
 */

void pretty_print(ShimmerScope* shsc, int depth) {
  std::cout << \
    std::string(depth, '\t') << "Begin " << \
    (depth > 0 ? "subscope " : "scope ") << "{\n";

  if (shsc->upper_scope != nullptr) {
    pretty_print(shsc->upper_scope, depth + 1);
  }

  pretty_print(&shsc->current_scope, depth + 1);

  std::cout << \
    std::string(depth, '\t') << "} End " << \
    (depth > 0 ? "subscope " : "scope ") << "\n";
}

void pretty_print(Scope* sc, int depth) {
  for (const auto& [k, v] : *sc) {
    std::cout << \
      std::string(depth, '\t') << \
      "\"" << k << "\": ";

    pretty_print(*v);

    std::cout << "\n";
  }
}

void test_pretty_print_literal() {
  ShimmerLiteral str_lit_1(-1, "Hi there");
  ShimmerLiteral str_lit_2(-1, "Hello world!");
  ShimmerLiteral int_lit_1(-1, 3);
  ShimmerLiteral int_lit_2(-1, 42);
  ShimmerLiteral bool_lit_1(-1, true);
  ShimmerLiteral bool_lit_2(-1, false);

  // TODO
  //ShimmerLiteral id_lit_1(...);
  //ShimmerLiteral id_lit_2(...);

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
