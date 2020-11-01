#include <iostream>
#include "lexer_alt.h"

Lexer::Lexer() {
  currently_in = NONE;
  current_token_contents = "";
}

std::vector<DotToken> Lexer::lex(std::string str) {

}

DotToken Lexer::make_token() {
  switch (currently_in) {
    case INT:
     return DotInt(current_token_contents);

    case STR:
      return DotString(current_token_contents);

    case ID:
      return DotIdentifier(current_token_contents);

    default:
      throw std::runtime_error("currently_in = " + std::to_string(currently_in));
  }
}

#ifdef DEBUG
void Lexer::test_make_token() {
  currently_in = INT;
  current_token_contents = "42";
  DotToken temp = make_token();
  std::cout << "INT 42: " << temp.get_contents() << " " << \
               temp.get_token_type() << "\n";

  currently_in = STR;
  current_token_contents = "foobar";
  temp = make_token();
  std::cout << "STR foobar: " << temp.get_contents() << " " << \
               temp.get_token_type() << "\n";

  currently_in = ID;
  current_token_contents = "myName";
  temp = make_token();
  std::cout << "ID myName: " << temp.get_contents() << " " << \
               temp.get_token_type() << "\n";

  currently_in = NONE;
  current_token_contents = "omg";
  temp = make_token();
  std::cout << "NONE omg: " << temp.get_contents() << " " << \
               temp.get_token_type() << "\n";
}

void lexer_test() {
  Lexer l;
  l.test_make_token();
}
#endif