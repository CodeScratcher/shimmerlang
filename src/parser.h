#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "ShimmerClasses.h"

typedef enum {
  NAME_OR_LITERAL, PARAM, COMMA, STATEMENT_OR_CALL, FURTHER_FUNC
} Expectation;

class Parser {
  public:
    ShimmerParam current_expr;
    Expectation expectation;
    DotStatement to_add;
    ShimmerParam expr;
    int this_token_id;

    DotToken this_token;
    DotToken next_token;
    std::vector<DotToken> tokens;

    std::vector<DotStatement> statements;
    std::vector<ShimmerParam> params;

    DotTree to_return;

    Parser(std::vector<DotToken> _tokens);
    Parser(std::vector<DotToken> _tokens, ShimmerParam start);
    DotTree parse();

    void name_or_literal_expectation();
    void statement_or_call_expectation();
    void comma_expectation();
    void further_func_expectation();
    void param_expectation();
    ShimmerUnclosedFunc fn_parser();
    void print_tokens();
    void print_token(int i);
};

void print_debug_info(DotTree x);
const char* param_recursive_str(ShimmerParam to_convert);
void print_statement_info(DotStatement i);

#endif
