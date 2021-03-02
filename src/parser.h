#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "ShimmerClasses.h"

typedef enum {
  NAME_OR_LITERAL, PARAM, COMMA, STATEMENT_OR_CALL, FURTHER_FUNC
} Expectation;

class Parser {
  public:
    ShimmerExpr current_expr;
    Expectation expectation;
    DotStatement to_add;
    ShimmerExpr expr;
    int this_token_id;
    bool first_param;
    DotToken this_token;
    DotToken next_token;
    std::vector<DotToken> tokens;

    std::vector<DotStatement> statements;
    std::vector<ShimmerExpr> params;

    DotTree to_return;

    Parser(std::vector<DotToken> _tokens);
    Parser(std::vector<DotToken> _tokens, ShimmerExpr start);
    DotTree parse();
    ShimmerUnclosedFunc parse_fn();

    void name_or_literal_expectation();
    void statement_or_call_expectation();
    void comma_expectation();
    void further_func_expectation();
    void param_expectation();

    void print_tokens();
    void print_token(int i);
};

void print_debug_info(DotTree x);
const char* param_recursive_str(ShimmerExpr to_convert);
void print_statement_info(DotStatement i);
const char* get_expectation_name(Expectation expect);

const char* parse_test();

#endif
