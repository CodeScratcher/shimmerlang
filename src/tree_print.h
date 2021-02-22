#ifndef TREE_PRINT_H
#define TREE_PRINT_H

#include <string>
#include "ShimmerClasses.h"

void pretty_print(DotTree tree);
void pretty_print(DotStatement statement, int depth = 0);
void pretty_print(ShimmerParam param, int depth = 0);
void pretty_print(DotLiteral lit, int depth = 0);

void test_pretty_print_literal();

// void pretty_print(DotTree tree);
// void pretty_print(DotStatement statement, int indent=0);
// void pretty_print(ShimmerParam sub_tree, int indent=0);
// void pretty_print(DotLiteral lit);

#endif