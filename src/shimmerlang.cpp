#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ShimmerClasses.h"
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
      dot_eval(buffer.str());
    }
  }
  else {
    // Shell
    while(1) {
      std::string to_eval;
      std::cout << "\x1b[1;92mshimmer % \x1b[0m";
      std::getline(std::cin, to_eval);
      dot_eval(to_eval);
    }
  }

  return 0;
}

int dot_eval(std::string str) {
  return execute(parse(lex(str)));
}
