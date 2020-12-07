#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include "ShimmerClasses.h"

typedef enum {NONE, CMNT1, CMNT2, CMNT3, CMNT4, ID, INT, STR} State;
std::string str_repr(State state);

std::vector<DotToken> lex(std::string str);
DotToken make_token(State now_in, std::string current_token_contents, int line);
const char* lex_to_str(std::vector<DotToken> lexed);
const char* lex_test();

#endif