#include <string>

#include "lexer.h"
#include "utility.h"

chtype escape_ch(chtype esc) {
  switch (esc) {
    case '\'': return '\''; break;
    case '\"': return '"';  break;
    case '\?': return '?';  break;
    case '\\': return '\\'; break;
    case '\a': return 'a';  break;
    case '\b': return 'b';  break;
    case '\f': return 'f';  break;
    case '\n': return 'n';  break;
    case '\r': return 'r';  break;
    case '\t': return 't';  break;
    case '\v': return 'v';  break;
    default:   return '!';   break;
  }
}

std::string unescape(chtype esc) {
  chtype c = escape_ch(esc);

  return (
    c == '!' ? std::string(1, esc) : std::string("\\") + c
  );
}

std::string unescape(std::string esc) {
  std::string result;

  for (auto c : esc) {
    result.append(unescape(c));
  }

  return result;
}

const char* c_unescape(std::string esc) {
  char* out = (char*) malloc(100 * sizeof *out);
  char* p = out;

  for (auto c : esc) {
    *p++ = '\\';
    *p++ = escape_ch(c);
  }

  return out;
}
