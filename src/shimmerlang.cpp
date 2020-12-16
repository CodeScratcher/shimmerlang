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
  // VER is passed as a macro argument to the compiler
  std::string message = "Shimmerlang version " + std::string("(Not yet)") + \
                        " licensed under the MIT license.\n";

  if (argc > 1) {
    if (!interpret_program(argv[1])) {
      return 1;
    }
  }
  else {
    std::cout << tc::cyan << message << tc::reset;
    interpret_shell();
  }

  return 0;
}

bool interpret_program(char* program_name) {
  std::ifstream file;
  file.open(program_name);

  if (!file) {
    std::cout << tc::red << "Error while opening file.\n" << tc::reset;
    return false;
  }
  else {
    std::stringstream buffer;
    buffer << file.rdbuf();
    if (!execute(buffer.str(), std::string(program_name))) {
      std::cout << tc::red << "Terminating program...\n" << tc::reset;
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
    std::cout << tc::bold << tc::green << "shimmer % " << tc::reset;
    std::getline(std::cin, to_eval);
    execute(to_eval, "<repl>");
  }
}

int execute(std::string str, std::string loc) {
  try {
    Parser parser(lex(str));
    DotTree parsed = parser.parse();
    eval(parsed);
  
    return true;
  }
  catch (std::runtime_error& err) {
    std::cout << tc::red << "In file " << loc << " on line " \
              << err.what() << "\n\n" << tc::reset;

    return false;
  }
}
