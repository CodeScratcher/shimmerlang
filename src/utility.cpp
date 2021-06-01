#include "utility.h"

#include <string>

#include "lexer.h"

std::string unescape(chtype esc) {
  switch (esc) {
    case '\'': return "\\'";  break;
    case '\"': return "\\\""; break;
    case '\?': return "\\?";  break;
    case '\\': return "\\\\"; break;
    case '\a': return "\\a";  break;
    case '\b': return "\\b";  break;
    case '\f': return "\\f";  break;
    case '\n': return "\\n";  break;
    case '\r': return "\\r";  break;
    case '\t': return "\\t";  break;
    case '\v': return "\\v";  break;
  }
}

std::string unescape(std::string esc) {
  std::string result;

  for (auto c : esc) {
    result.append(unescape(c));
  }

  return result;
}

const char* c_unescape(std::string esc) {
  return unescape(esc).c_str();
}
