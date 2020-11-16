#include <string>
#include "ShimmerClasses.h"

void pretty_print(DotTree tree);
void pretty_print(DotStatement statement, int indent=0);
void pretty_print(ShimmerParam sub_tree, int indent=0);
void pretty_print(DotLiteral lit);