#include <iostream>
#include <vector>

#include "ShimmerClasses.h"
#include "errors.h"
#include "pretty_print.h"

ShimmerLiteral eval_tree(ShimmerTree tree) {
#ifdef DEBUG
  std::cout << "Pretty printing whole tree...\n";
  pretty_print(tree);
  std::cout << "Done!\n";
#endif

  ShimmerScope* scope = new ShimmerScope(Scope());
  ShimmerLiteral return_val;
  int j = 0;
  for (ShimmerExpr i : tree.get_tree()) {
    j++;

#ifdef DEBUG
    std::cout << "Pretty printing one expression...\n";
    pretty_print(i);
    std::cout << "Done.\nEvaluating...\n";
#endif

    if (i.is_of_type(STATEMENT))  {
      if (j == tree.get_tree().size()) {
        return i.get_statement_val().eval(scope);
      }

      return_val = i.get_statement_val().eval(scope);
    }
    else if (i.is_of_type(LITERAL))    return_val = i.get_literal_val();
    else if (i.is_of_type(IDENTIFIER)) return_val = scope->get_variable(i.get_identifier_val().get_contents());
    else if (i.is_of_type(FUNCTION))   { 
      return_val = ShimmerLiteral(-1, ShimmerClosedFunc(i.get_func_val(), scope));
#ifdef DEBUG
      pretty_print(ShimmerExpr(return_val));
#endif
    }
#ifdef DEBUG
    std::cout << "Done.\n";
#endif
  }

  return return_val;
}

ShimmerLiteral eval_tree(ShimmerTree tree, ShimmerScope* above_scope, std::vector<ShimmerIdentifier> params, std::vector<ShimmerExpr> args) {
#ifdef DEBUG
  std::cout << "Pretty printing whole tree...\n";
  pretty_print(tree);
  std::cout << "Done!\n";
#endif
  Scope s;

  std::string str;

  str += "Function expected at least ";
  str += std::to_string(params.size());
  str += " arguments but got only ";
  str += std::to_string(args.size());

  if (args.size() < params.size()) {
    throw_error(-1, str);
  }
  
  for (size_t i = 0; i < params.size(); ++i) {
    ShimmerLiteral* x = new ShimmerLiteral(args[i].get_literal_val());
    s[params[i].get_contents()] = x;
  }

  ShimmerScope* scope = new ShimmerScope(above_scope, s);
  ShimmerLiteral return_val;
  
  int j = 0;

  for (ShimmerExpr i : tree.get_tree()) {
    j++;

#ifdef DEBUG
    std::cout << "Pretty printing one expression...\n";
    pretty_print(i);
    std::cout << "Done.\nEvaluating...\n";
#endif

    if (i.is_of_type(STATEMENT))  {
      if (j == tree.get_tree().size()) {
        return i.get_statement_val().eval(scope);
      }

      return_val = i.get_statement_val().eval(scope);
    }
    else if (i.is_of_type(LITERAL))    return_val = i.get_literal_val();
    else if (i.is_of_type(IDENTIFIER)) return_val = scope->get_variable(i.get_identifier_val().get_contents());
    else if (i.is_of_type(FUNCTION)) { 
      ShimmerClosedFunc x = ShimmerClosedFunc(*i.func_val, scope);
      return_val = ShimmerLiteral(-1, x);

      pretty_print(ShimmerExpr(return_val));
    }

#ifdef DEBUG
    std::cout << "Done.\n";
#endif
  }

  return return_val;
}

std::tuple<ShimmerLiteral, ShimmerScope*> eval_and_get_scope(ShimmerTree tree) {
#ifdef DEBUG
  std::cout << "Pretty printing whole tree...\n";
  pretty_print(tree);
  std::cout << "Done!\n";
#endif

  ShimmerScope* scope = new ShimmerScope(Scope());
  ShimmerLiteral return_val;
  int j = 0;
  for (ShimmerExpr i : tree.get_tree()) {
    j++;

#ifdef DEBUG
    std::cout << "Pretty printing one expression...\n";
    pretty_print(i);
    std::cout << "Done.\nEvaluating...\n";
#endif

    if (i.is_of_type(STATEMENT))  {
      if (j == tree.get_tree().size()) {
        return std::tuple<ShimmerLiteral, ShimmerScope*>{i.get_statement_val().eval(scope), scope};
      }

      return_val = i.get_statement_val().eval(scope);
    }
    else if (i.is_of_type(LITERAL))    return_val = i.get_literal_val();
    else if (i.is_of_type(IDENTIFIER)) return_val = scope->get_variable(i.get_identifier_val().get_contents());
    else if (i.is_of_type(FUNCTION))   { 
      return_val = ShimmerLiteral(-1, ShimmerClosedFunc(i.get_func_val(), scope));
      pretty_print(ShimmerExpr(return_val));
    }

#ifdef DEBUG
    std::cout << "Done.\n";
#endif
  }

  return std::tuple<ShimmerLiteral, ShimmerScope*>{return_val, scope};
}
