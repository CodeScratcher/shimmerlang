#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ShimmerClasses.h"
#include "text_effects.h"
#include "lexer.h"
#include "parser.h"
#include "eval.h"

bool interpret_program(char* program_name);
void interpret_shell();

int execute(std::string str, std::string loc);

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << RED("Error: needs a version number.\n");
    return 1;
  }

  std::string message = "Shimmerlang version " + std::string(argv[1]) + \
                        " Licensed under the MIT license.\n";

  if (argc > 2) {
    if (!interpret_program(argv[2])) {
      return 1;
    }
  }
  else {
    std::cout << message;
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
    Parser parser;
    DotTree parsed = parser.parse(lex(str));
    eval(parsed);
    return true;
  }
  catch (std::runtime_error& err) {
    std::cout << RED("In file " << loc << " on line " << err.what() << "\n\n");
    return false;
  }
}
