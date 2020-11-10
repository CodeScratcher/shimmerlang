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

bool interpret_program(char* program_name);
void interpret_shell();

int execute(std::string str, std::string loc);
int main(int argc, char* argv[]) {
  if (argc > 1) {
    if (!interpret_program(argv[1])) {
      return 1;
    }
  }
  else {
    interpret_shell();
  }

  return 0;
}

bool interpret_program(char* program_name) {
  std::ifstream file;
  file.open(program_name);

  if (!file) {
    std::cout << "Error while opening file.";
    return false;
  }
  else {
    std::stringstream buffer;
    buffer << file.rdbuf();
    if (!execute(buffer.str(), std::string(program_name))) {
      std::cout << RED("Terminating program...\n");
      return false;
    }
    else {
      return true;
    }
  }
}

void interpret_shell() {
  while(1) {
    std::string to_eval;
    std::cout << BOLD(L_GREEN("shimmer % "));
    std::getline(std::cin, to_eval);
    execute(to_eval, "<repl>");
  }
}

int execute(std::string str, std::string loc) {
  try {
    eval(parse(lex(str)));
    return true;
  }
  catch (std::runtime_error& err) {
    std::cout << RED("In file " << loc << " on line " << err.what() << "\n\n");
    return false;
  }
}
