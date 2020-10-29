#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "DotClasses.h"
#include "lexer.h"
#include "parser.h"
#include "eval.h"

std::vector<DotToken> tokens;

int dot_eval(std::string str);

int main(int argc, char* argv[]) {
  if (argc > 1) {
    std::ifstream file;
    file.open(argv[1]);
    if (!file) {
      std::cout << "File not found.";
    }
    else {
      std::stringstream buffer;
      buffer << file.rdbuf();
      tokens = lex(buffer.str());
      dot_eval(buffer.str());
    }
  }
  else {
    while(1) {
      std::string to_eval;
      std::cout << "shimmer % ";
      std::getline(std::cin, to_eval);
      dot_eval(to_eval);
    }
  }

  return 0;
}

int dot_eval(std::string str) {
  return eval(parse(lex(str)));
}
