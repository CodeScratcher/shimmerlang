#ifndef EVAL_H
#define EVAL_H

#include <vector>
#include "ShimmerClasses.h"

ShimmerLiteral eval_tree(ShimmerTree tree);
ShimmerLiteral eval_tree(ShimmerTree tree, ShimmerScope* above_scope, std::vector<ShimmerIdentifier> params, std::vector<ShimmerExpr> args);
#endif
