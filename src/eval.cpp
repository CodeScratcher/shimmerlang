#include "ShimmerClasses.h"
#include "tree_print.h"
#include <iostream>

int eval(DotTree tree) {
  std::cout << "int eval(DotTree) was called.\n";
  ShimmerScope scope = ShimmerScope(Scope());

  std::cout << "Entering for loop. Length of statement vector: ";
  std::cout << std::to_string(tree.get_tree().size()) << ".\n";
  for (DotStatement i : tree.get_tree()) {
    std::cout << "Pretty printing...\n";
    pretty_print(i);
    std::cout << "Done.\nEvaluating...\n";
    i.eval(&scope);
    std::cout << "Done.\n";
  }

  return 0;
}