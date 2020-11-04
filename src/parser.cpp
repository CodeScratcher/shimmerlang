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
      for(int j = i; tokens.at(j).get_token_type().compare("DotRParen") != 0; j++) {
        i++;
        tokens_for_recursion.push_back(tokens.at(j));
      }
      tokens_for_recursion.push_back(DotRParen());
      params.push_back(ShimmerParam(parse(tokens_for_recursion).get_tree().at(0)));
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
  DotTree x = parse(lex("print(add(5, 3))"));
  std::cout << "Parsing done." << "\n";

  for (DotStatement i : x.get_tree()) {
    std::cout << i.get_identifier();
  }

  return "Test complete";
}
#endif
