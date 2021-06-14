#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

#include "ShimmerClasses.h"

#define LINECMNT_DELIM ';'

#define BLCKCMNT_DELIM_A '*'
#define BLCKCMNT_DELIM_B '#'

typedef enum {UNKNOWN = -1, NONE, COMMENT, BLCKCMNT_1, BLCKCMNT_2, BLCKCMNT_3, ID, NUMBER, STR} LexerState;
std::string str_repr(LexerState ls);

typedef char chtype;

std::vector<ShimmerToken> lex(std::string str);

ShimmerToken make_token(LexerState now_in, std::string current_token_contents, int line);

const char* lex_to_str(std::vector<ShimmerToken> lexed);
const char* lex_test();

#endif