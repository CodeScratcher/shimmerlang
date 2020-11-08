#include "ShimmerClasses.h"

int eval(DotTree tree) {
  ShimmerScope scope = ShimmerScope(Scope());

  for (DotStatement i : tree.get_tree()) {
    i.eval(scope);
  }

  return 0;
}