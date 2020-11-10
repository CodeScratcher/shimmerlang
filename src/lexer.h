#include <vector>
#include <string>
#include "ShimmerClasses.h"

typedef enum {NONE, ID, INT, STR} State;

std::vector<DotToken> lex(std::string str);
DotToken make_token(State now_in, std::string current_token_contents, int line);
const char* lex_to_str(std::vector<DotToken> lexed);
const char* lex_test();