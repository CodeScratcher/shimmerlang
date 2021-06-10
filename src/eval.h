#ifndef EVAL_H
#define EVAL_H

#include <vector>
#include <tuple>
#include "ShimmerClasses.h"

ShimmerLiteral eval_tree(ShimmerTree tree);
ShimmerLiteral eval_tree(ShimmerTree tree, ShimmerScope* above_scope, std::vector<ShimmerIdentifier> params, std::vector<ShimmerExpr> args);
std::tuple<ShimmerLiteral, ShimmerScope*> eval_and_get_scope(ShimmerTree tree);
#endif
