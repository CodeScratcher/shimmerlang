#include <vector>
#include <iostream>
#include <string>
#include "parser.h"
#include "lexer.h"
#include "ShimmerClasses.h"

DotTree parse(std::vector<DotToken> tokens) {
  std::vector<DotStatement> statements;
  DotStatement to_add;
  bool in_params = false;
  bool separated = true;
  std::vector<ShimmerParam> params;
  std::vector<DotToken> tokens_for_recursion;
  for (int i = 0; i < tokens.size(); i++) {
    DotToken this_token = tokens.at(i);
    if (this_token.get_token_type().compare("DotIdentifier") == 0 && !in_params) {
      to_add.set_identifier(this_token.get_contents());
    }
    else if (this_token.get_token_type().compare("DotLParen") == 0 && !in_params) {
      in_params = true;
    }
    else if (this_token.get_token_type().compare("DotRParen") == 0) {
      in_params = false;
      to_add.set_params(params);
      params.clear();
      statements.push_back(to_add);
    }
    else if (this_token.get_token_type().compare("DotIdentifier") == 0 && in_params) {
      if (separated) {
        int sub_expr_layer = 0;
        int j = i;
        while(true) {
          tokens_for_recursion.push_back(tokens.at(j));
          j++;
          if (tokens.at(j).get_token_type().compare("DotRParen") == 0 ) {
          sub_expr_layer--;
          if (sub_expr_layer == 0) {
              break;
            }
          
          }
          if (tokens.at(j).get_token_type().compare("DotLParen") == 0) {
            sub_expr_layer++;
          }
        }
        if (j > tokens.size()) {
          throw std::runtime_error("Missing closing parenthesis.");
        }
        tokens_for_recursion.push_back(tokens.at(j));
        i = j;
        DotTree parsed = parse(tokens_for_recursion);
        DotStatement to_push = parsed.get_tree().at(0);
        ShimmerParam param = ShimmerParam(to_push);
        params.push_back(param);
      }
      else {
        throw std::runtime_error(std::string("Missing a comma at token: ") + this_token.get_contents());
      }
    }
    else if (this_token.get_token_type().compare("DotInt") == 0) {
      if (separated) {
        ShimmerParam x(DotLiteral(this_token.get_parsed_contents()));
        params.push_back(x);
        separated = false;
      }
      else {
        throw std::runtime_error(std::string("Missing a comma at token: ") + this_token.get_contents());
      }
    }
    else if (this_token.get_token_type().compare("DotString") == 0) {
      if (separated) {
      params.push_back(ShimmerParam(DotLiteral(this_token.get_contents())));
      separated = false;
      }
      else {
        throw std::runtime_error(std::string("Missing a comma at token: ")+ this_token.get_contents());
      }
    }
    else if (this_token.get_token_type().compare("DotComma") == 0) {
      separated = true;
    }
  }
  DotTree toReturn = DotTree(statements); 
  return toReturn;
}

#ifdef DEBUG
const char* param_recursive_str(ShimmerParam to_convert) {
  if(to_convert.get_is_literal()) {
    DotLiteral liter = to_convert.get_literal_val();
    std::cout << liter.type << "\n";
    if (liter.type == TypeString || liter.type == TypeInt) {
      return liter.get_str().c_str();
    }
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
    for (ShimmerParam j : i.get_params()){
      std::cout << param_recursive_str(j) << "\n";
    }
  }
}
const char* parse_test() {
  DotTree x = parse(lex("print(add(sub(2, 1), sub(3, add(1, 1))))"));
  std::cout << "Parsing done." << "\n";

  print_debug_info(x);  

  return "Test complete";
}

#endif
