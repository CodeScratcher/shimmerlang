#include <vector>
#include <iostream>
#include <string>
#include "lexer.h"
#include "DotClasses.h"
DotTree parse(std::vector<DotToken> tokens) {
  std::vector<DotStatement> statements;
  DotStatement to_add;
  bool in_params = false;
  std::vector<DotLiteral> params;
  std::vector<DotToken> tokens_for_recursion;
  for (int i = 0; i < tokens.size(); i++) {
    DotToken on = tokens.at(i);
    if (on.get_token_type().compare("DotIdentifier") == 0 && !in_params) {
      std::cout << on.getContents();
      to_add.set_identifier(on.getContents());
    }
    else if (on.get_token_type().compare("DotLParen")) {
      in_params = true;
    }
    else if (on.get_token_type().compare("DotRParen")) {
      in_params = false;
      statements.push_back(to_add);
    }
    else if (on.get_token_type().compare("DotIdentifier")) {
      for(int j = 0; tokens.at(j).get_token_type().compare("DotComma") == 0; j++) {
        i++;
        tokens_for_recursion.push_back(tokens.at(j));
      }
      params.push_back(DotLiteral(parse(tokens_for_recursion).getTree().at(0)));
    }
    else if (on.get_token_type().compare("DotInt")) {
      params.push_back(DotLiteral(on.getParsedInt()));
    }
    else if (on.get_token_type().compare("DotString")) {
      params.push_back(DotLiteral(on.getContents()));
    }
  }
  DotTree toReturn = DotTree(statements); 
  return toReturn;
}
#ifdef DEBUG
const char* parse_to_str() {
  DotTree x = parse(lex("print('hello world')"));
  for (DotStatement i : x.getTree()) {
    std::cout << i.get_identifier();
  }
  return "Hello World";
}
#endif