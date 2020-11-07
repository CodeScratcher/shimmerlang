#include <vector>
#include <string>
#include "ShimmerClasses.h"

typedef enum {NONE, ID, INT, STR} State;

class Lexer {
  public:
    Lexer();
    std::vector<DotToken> lex(std::string str);
    bool currently_in_token();
    void finish_current_token();
    void clear_current_token();
    DotToken make_token();
    const char* lex_to_str(std::vector<DotToken> lexed);
    const char* lex_test();

  private:
    char string_delimiter;
    std::string current_token_contents;
    State now_in;
};
