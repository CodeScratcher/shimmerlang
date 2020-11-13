#include <string>
#include "ShimmerClasses.h"

void pretty_print(DotTree tree,          std::string prefix = " ");
void pretty_print(DotStatement sub_tree, std::string prefix = " ");
void pretty_print(ShimmerParam sub_tree, std::string prefix = " ");
void pretty_print(DotLiteral lit);