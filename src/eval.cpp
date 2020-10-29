#include "ShimmerClasses.h"
int eval(DotTree tree) {
  for (DotStatement i : tree.get_tree()) {
    i.eval();
  }

  return 0;
}