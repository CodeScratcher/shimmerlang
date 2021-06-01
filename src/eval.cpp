#include "ShimmerClasses.h"
#include "tree_print.h"
#include <iostream>

int eval(ShimmerTree tree) {
  std::cout << "Pretty printing whole tree...\n";
  pretty_print(tree);
  std::cout << "Done!\n";

  ShimmerScope scope = ShimmerScope(Scope());

  for (ShimmerStatement i : tree.get_tree()) {
    std::cout << "Pretty printing...\n";
    pretty_print(i);
    std::cout << "Done.\nEvaluating...\n";
    i.eval(&scope);
    std::cout << "Done.\n";
  }

  return 0;
}
