#include <vector>
#include <string>
#include "ShimmerClasses.h"

typedef enum state_enum {NONE, INT, STR, ID} State;

class Lexer {
  public:
    Lexer();
    std::vector<DotToken> lex(std::string str);
    void test_make_token();

  private:
    std::string current_token_contents;
    char string_watch_out_for;
    State currently_in;

    void append_to_current_token(char& ch);
    void finish_current_token();
    DotToken make_token();
    //void test_make_token(); made public for testing
};