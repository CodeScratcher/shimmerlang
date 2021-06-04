#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "ShimmerClasses.h"

typedef enum {
  NAME_OR_LITERAL, PARAM, COMMA, ID_OR_CALL, FURTHER_FUNC, SYMBOL
} Expectation;

class Parser {
  public:
    Parser(std::vector<ShimmerToken> _tokens);
    Parser(std::vector<ShimmerToken> _tokens, ShimmerExpr start);

    ShimmerTree parse();
    ShimmerUnclosedFunc parse_fn();

    void handle_expectation(Expectation exp);
    void symbol_expectation();
    void name_or_literal_expectation();
    void id_or_call_expectation();
    void comma_expectation();
    void further_func_expectation();
    void param_expectation();

    void print_tokens();
    void print_token(int i);

    ShimmerExpr current_expr;
    Expectation expectation;
    ShimmerStatement to_add;
    ShimmerExpr expr;

    int this_token_id;
    bool first_param;
    ShimmerToken this_token;
    ShimmerToken next_token;
    std::vector<ShimmerToken> tokens;
    std::vector<ShimmerExpr> statements;
    std::vector<ShimmerExpr> params;

    ShimmerTree to_return;
};

void print_debug_info(ShimmerTree x);
const char* param_recursive_str(ShimmerExpr to_convert);
void print_statement_info(ShimmerStatement i);
const char* get_expectation_name(Expectation expect);

const char* parse_test();

#endif
