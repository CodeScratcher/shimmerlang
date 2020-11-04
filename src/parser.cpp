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
      for(int j = i;  ; j++) {
        i++;
        tokens_for_recursion.push_back(tokens.at(j));
       // std::cout << tokens.at(j).get_token_type() << j << "\n";
        if (tokens.at(j).get_token_type().compare("DotRParen") == 0 ) {
        sub_expr_layer--;
        std::cout << sub_expr_layer << "\n";
        if (sub_expr_layer == 0) {
            break;
          }
        
        }
        if (tokens.at(j).get_token_type().compare("DotLParen") == 0) {
          sub_expr_layer++;
        }
         
      }
   //   std::cout << "Made it to the end of this level, moving to next. \n";
      lex_to_str(tokens_for_recursion);
      DotTree parsed = parse(tokens_for_recursion);
      print_debug_info(parsed);
      DotStatement to_push = parsed.get_tree().at(0);
      params.push_back(ShimmerParam(to_push));
    }
    else if (this_token.get_token_type().compare("DotInt") == 0) {
      ShimmerParam x = ShimmerParam(DotLiteral(this_token.get_parsed_contents()));
      params.push_back(x);
    }
    else if (this_token.get_token_type().compare("DotString") == 0) {
      params.push_back(ShimmerParam(DotLiteral(this_token.get_contents())));//get_parsed_contents?
    }
  }
  DotTree toReturn = DotTree(statements); 
  return toReturn;
}

#ifdef DEBUG
const char* param_recursive_str(ShimmerParam to_convert) {
  if(to_convert.get_is_literal()) {
    return to_convert.get_literal_val().get_str().c_str();
  }
  else {
    for (ShimmerParam i : to_convert.get_statement_val().get_params()) {
      std::cout << param_recursive_str(i) << "\n";
    }
    return "Param recursive";
  }
}
void print_debug_info(DotTree x) {
  for (DotStatement i : x.get_tree()) {
  //  std::cout << i.get_identifier() << "\n";
    for (ShimmerParam j : i.get_params()){
      std::cout << param_recursive_str(j) << "\n";
    }
  }
}
const char* parse_test() {
  DotTree x = parse(lex("add(3,3)"));
  std::cout << "Parsing done." << "\n";

  print_debug_info(x);  

  return "Test complete";
}

#endif
