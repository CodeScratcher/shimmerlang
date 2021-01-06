#include "ShimmerClasses.h"
#include "tree_print.h"
#include <iostream>

int eval(DotTree tree) {
  ShimmerScope scope = ShimmerScope(Scope());

  for (DotStatement i : tree.get_tree()) {
    std::cout << "Pretty printing...\n";
    std::cout << "Done.\nEvaluating...\n";
    i.eval(&scope);
    std::cout << "Done.\n";
  }

  return 0;
}