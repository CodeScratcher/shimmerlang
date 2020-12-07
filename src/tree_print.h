#ifndef TREE_PRINT_H
#define TREE_PRINT_H

#include <string>
#include "ShimmerClasses.h"

void pretty_print(DotTree tree);
void pretty_print(DotStatement statement);
void pretty_print(ShimmerParam param, std::vector<bool> is_exploring,
                  int depth, bool is_last);
std::string pretty_print(ShimmerParam param);
std::string pretty_print(DotLiteral lit);

// void pretty_print(DotTree tree);
// void pretty_print(DotStatement statement, int indent=0);
// void pretty_print(ShimmerParam sub_tree, int indent=0);
// void pretty_print(DotLiteral lit);

#endif