#include "DotClasses.h"
int eval(DotTree tree) {
  for (DotStatement i : tree.getTree()) {
    i.eval();
  }

  return 0;
}
