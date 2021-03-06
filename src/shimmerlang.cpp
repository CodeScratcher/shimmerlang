#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

#include "CMakeConfig.h"
#include "ShimmerClasses.h"
#include "easteregg.h"
#include "eval.h"
#include "lexer.h"
#include "linenoise.h"
#include "parser.h"
#include "text_effects.h"
#include "pretty_print.h"
#include "prompt.h"

bool interpret_program(char* program_name);
void interpret_shell();

std::tuple<ShimmerLiteral, bool> execute(std::string str, std::string loc);

std::string easteregg = GET_THE_EASTEREGG;

int main(int argc, char* argv[]) {
  if (argc > 1) {
    if (!interpret_program(argv[1])) {
      return 1;
    }
  }
  else {
    std::cout << \
      tc::cyan << \
      tc::bold << \
      "Shimmerlang version " << \
      std::string(VERSION) << \
      " licensed under the MIT license. See LICENSE.txt for more information." << \
      tc::reset << "\n";

    std::cout << \
      tc::blue << \
      "Use Ctrl-C, Ctrl-D, exit(status), or /quit to exit.\n" << \
      tc::reset << "\n";

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

    if (!std::get<1>(execute(buffer.str(), std::string(program_name)))) {
      std::cout << tc::red << "Terminating program...\n" << tc::reset;
      return false;
    }
    else {
      return true;
    }
  }
}

void interpret_shell() {
#ifdef USE_LINENOISE
  linenoiseSetMultiLine(1);
  linenoiseHistoryLoad("~/.shmr_history");
#endif

  char* to_eval;

  while (true) {
    std::cout << tc::reset;
  
    to_eval = prompt(">>> ");

    if (to_eval == (char*) - 1) {
      std::cout << "\n";
      return;
    }
    if (to_eval == nullptr || to_eval == NULL) {
      *to_eval = '\0';
    }
    else if (to_eval[0] != '\0' && to_eval[0] != '/') {
#ifdef USE_LINENOISE
      linenoiseHistoryAdd(to_eval);
      linenoiseHistorySave("~/.shmr_history");
#endif

      std::cout << "\n=> " + std::get<0>(execute(to_eval, "<repl>")).get_str() + "\n";
    }
#ifdef USE_LINENOISE
    else if (strncmp(to_eval, "/setmaxhistlen", 14) == 0) {
      int len = atoi(to_eval + 14);
      linenoiseHistorySetMaxLen(len);
      std::cout << "Set max history length to " << std::to_string(len) << ".\n";
    }
    else if (strncmp(to_eval, "/mask", 5) == 0) {
      linenoiseMaskModeEnable();
      std::cout << "Mask mode enabled.\n";
    }
    else if (strncmp(to_eval, "/unmask", 7) == 0) {
      linenoiseMaskModeDisable();
      std::cout << "Mask mode disabled.\n";
    }
#endif
    else if (strncmp(to_eval, "/easteregg", 10) == 0) {
      std::cout << easteregg << "\n";
    }
    else if (strncmp(to_eval, "/shrug", 6) == 0) {
      std::cout << "\u203E\\_(\u30C4)_/\u203E\n";
    }
    else if (strncmp(to_eval, "/quit", 5) == 0) {
      return;
    }
    else if (to_eval[0] == '/') {
      std::cout << "Unrecognized command: " << to_eval << "\n";
    }

    free(to_eval);
  }
}

std::tuple<ShimmerLiteral, bool> execute(std::string str, std::string loc) {
  try {
    Parser parser(lex(str));
    ShimmerTree parsed = parser.parse();
    ShimmerLiteral evaled = eval_tree(parsed);

    return {evaled, true};
  }
  catch (std::runtime_error& err) {
    std::cout << \
      tc::red << \
      "In file " << \
      loc << \
      " on line " << \
      err.what() << \
      tc::reset << \
      "\n\n";

    int line;
    sscanf(err.what(), "%d", &line);

    return {ShimmerLiteral(line, std::string("ERROR")), false};
  }
}
