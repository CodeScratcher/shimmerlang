#include <vector>
#include <string>
#include "ShimmerClasses.h"

typedef enum {NONE, ID, INT, STR} State;

std::vector<DotToken> lex(std::string str);
const char* lex_to_str(std::vector<DotToken> lexed);