#ifndef TREE_PRINT_H
#define TREE_PRINT_H

#include <string>

#include "ShimmerClasses.h"

void pretty_print(ShimmerTree tree);
void pretty_print(ShimmerStatement statement, int depth = 0);
void pretty_print(ShimmerExpr expr, int depth = 0);
void pretty_print(ShimmerLiteral lit, int depth = 0);

void pretty_print(ShimmerScope* shsc, int depth = 0);
void pretty_print(Scope* sc, int depth = 0);

void pretty_print(ShimmerUnclosedFunc func, int depth = 0);
void pretty_print(ShimmerClosedFunc func, int depth = 0);

void test_pretty_print_literal();

#endif