#include <iostream>

#include "ShimmerClasses.h"
#include "tree_print.h"

ShimmerLiteral eval_tree(ShimmerTree tree) {
  std::cout << "Pretty printing whole tree...\n";
  pretty_print(tree);
  std::cout << "Done!\n";

  ShimmerScope scope = ShimmerScope(Scope());
  ShimmerLiteral return_val;

  for (ShimmerExpr i : tree.get_tree()) {
    std::cout << "Pretty printing...\n";
    pretty_print(i);
    std::cout << "Done.\nEvaluating...\n";

    if      (i.is_of_type(STATEMENT))  return_val = i.get_statement_val().eval(&scope);
    else if (i.is_of_type(LITERAL))    return_val = i.get_literal_val();
    else if (i.is_of_type(IDENTIFIER)) return_val = scope.get_variable(i.get_identifier_val().get_contents());
    else if (i.is_of_type(FUNCTION))   return_val = ShimmerLiteral(-1, ShimmerClosedFunc(i.get_func_val(), &scope));
  
    std::cout << "Done.\n";
  }

  return return_val;
}
