#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include "ShimmerClasses.h"

typedef enum {UNKNOWN = -1, NONE, COMMENT, ID, INT, STR} LexerState;
std::string str_repr(LexerState ls);

typedef char chtype;

std::vector<ShimmerToken> lex(std::string str);
chtype esc_seq(chtype ch);

ShimmerToken make_token(LexerState now_in, std::string current_token_contents, int line);

const char* lex_to_str(std::vector<ShimmerToken> lexed);
const char* lex_test();

#endif