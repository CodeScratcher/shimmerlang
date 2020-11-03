#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ShimmerClasses.h"
#include "text_effects.h"
#include "lexer.h"
#include "parser.h"
#include "eval.h"

std::vector<DotToken> tokens;

int execute(std::string str);

int main(int argc, char* argv[]) {
  if (argc > 1) {
    // Interpreter
    std::ifstream file;
    file.open(argv[1]);

    if (!file) {
      std::cout << "Error while opening file.";
    }
    else {
      std::stringstream buffer;
      buffer << file.rdbuf();
      tokens = lex(buffer.str());
      execute(buffer.str());
    }
  }
  else {
    // Shell
    while(1) {
      std::string to_eval;
      std::cout << BOLD(L_GREEN("shimmer % "));
      std::getline(std::cin, to_eval);
      execute(to_eval);
    }
  }

  return 0;
}

int execute(std::string str) {
  return eval(parse(lex(str)));
}
