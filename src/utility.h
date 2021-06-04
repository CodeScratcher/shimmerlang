#ifndef UTILITY_H
#define UTILITY_H

#include <string>

#include "lexer.h"

chtype escape_ch(chtype esc);
std::string unescape(chtype esc);
std::string unescape(std::string esc);
const char* c_unescape(std::string esc);

#endif
