#include <vector>
#include <iostream>
#include <string>
#include "parser.h"
#include "lexer.h"
#include "ShimmerClasses.h"

#define TOKEN_TYPE_EQUAL(index, type) (tokens.at((index)).get_token_type().compare((type)) == 0))

DotTree parse(std::vector<DotToken> tokens) {
  std::vector<DotStatement> statements;
  DotStatement to_add;
  bool in_params = false;
  std::vector<ShimmerParam> params;
  std::vector<DotToken> tokens_for_recursion;
  for (int i = 0; i < tokens.size(); i++) {
    DotToken this_token = tokens.at(i);

    if (this_token.get_token_type().compare("DotIdentifier") == 0 && !in_params) {
      to_add.set_identifier(this_token.get_contents());
    }
    else if (this_token.get_token_type().compare("DotLParen") == 0) {
      in_params = true;
    }
    else if (this_token.get_token_type().compare("DotRParen") == 0) {
      in_params = false;
      to_add.set_params(params);
      params.clear();
      statements.push_back(to_add);
    }
    else if (this_token.get_token_type().compare("DotIdentifier") == 0) {
      int sub_expr_layer = 0;
      for(int j = i; ; j++) {
        if (tokens.at(j).get_token_type().compare("DotRParen") == 0 ) {
          if (sub_expr_layer == 0) {
            break;
          }
          else {
            sub_expr_layer--;
          }
        }
        if (tokens.at(j).get_token_type().compare("DotLParen") == 0) {
          sub_expr_layer++;
        }
        i++;
        tokens_for_recursion.push_back(tokens.at(j));
      }
      DotStatement to_push = parse(tokens_for_recursion).get_tree().at(0);
      params.push_back(ShimmerParam(to_push));
    }
    else if (this_token.get_token_type().compare("DotInt") == 0) {
      params.push_back(ShimmerParam(this_token.get_parsed_contents()));
    }
    else if (this_token.get_token_type().compare("DotString") == 0) {
      params.push_back(ShimmerParam(this_token.get_contents()));//get_parsed_contents?
    }
  }
  DotTree toReturn = DotTree(statements); 
  return toReturn;
}

#ifdef DEBUG
const char* parse_test() {
  DotTree x = parse(lex("print(add(4, div(6, 2)))"));
  std::cout << "Parsing done." << "\n";

  for (DotStatement i : x.get_tree()) {
    std::cout << i.get_identifier();
  }

  return "Test complete";
}
const char* param_recursive_str(ShimmerParam to_convert) {
  if(to_convert.get_is_literal()) {
    return to_convert.get_literal_val().get_str().c_str();
  }
  else {
    for (ShimmerParam i : to_convert.get_statement_val().get_params()) {
      std::cout << param_recursive_str(i) << "\n";
      return "No";
    }
  }
}
#endif
