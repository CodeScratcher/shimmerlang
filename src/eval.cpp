#include "DotClasses.h"
void eval(DotTree tree) {
   for (DotStatement i : tree.getTree()) {
     i.eval();
   }
}