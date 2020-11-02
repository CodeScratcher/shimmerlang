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
  std::vector<DotLiteral> params;
  std::vector<DotToken> tokens_for_recursion;
  for (int i = 0; i < tokens.size(); i++) {
    DotToken this_token = tokens.at(i);

    if (this_token.get_token_type().compare("DotIdentifier") == 0 && !in_params) {
      to_add.set_identifier(this_token.get_contents());
    }
    else if (this_token.get_token_type().compare("DotLParen") == 0) {
      std::cout << "Found LParen" << "\n";
      in_params = true;
    }
    else if (this_token.get_token_type().compare("DotRParen") == 0) {
      std::cout << "Found RParen at end" << "\n";
      in_params = false;
      std::cout << to_add.get_identifier() << "\n";
      std::cout << to_add.get_params() << "\n";
      statements.push_back(to_add);
    }
    else if (this_token.get_token_type().compare("DotIdentifier") == 0) {
      for(int j = 0; tokens.at(j).get_token_type().compare("DotComma") == 0; j++) {
        i++;
        if (tokens.at(j).get_token_type().compare("DotRParen") == 0) {
          std::cout << "Found RParen at end";
          in_params = false;
          statements.push_back(to_add);
          break;
        }
        tokens_for_recursion.push_back(tokens.at(j));
      }
      params.push_back(DotLiteral(parse(tokens_for_recursion).get_tree().at(0)));
    }
    else if (this_token.get_token_type().compare("DotInt") == 0) {
      params.push_back(DotLiteral(this_token.get_parsed_contents()));
    }
    else if (this_token.get_token_type().compare("DotString") == 0) {
      params.push_back(DotLiteral(this_token.get_contents()));//get_parsed_contents?
    }
  }
  DotTree toReturn = DotTree(statements); 
  return toReturn;
}

#ifdef DEBUG
const char* parse_test() {
  DotTree x = parse(lex("print('hello world')"));
  std::cout << "Parsing done." << "\n";

  for (DotStatement i : x.get_tree()) {
    std::cout << i.get_identifier();
  }

  return "Test complete";
}
#endif
