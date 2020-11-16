#include <vector>
#include "ShimmerClasses.h"

typedef enum {
  VALUE, PARAM, COMMA, VALUE_OR_PARENS
} Expectation;

DotTree parse(std::vector<DotToken>);
void throw_error(std::string msg, std::string line);
void throw_error(std::string msg, int line);

void print_debug_info(DotTree x);
const char* param_recursive_str(ShimmerParam to_convert);
void print_statement_info(DotStatement i);