#include <vector>
#include "ShimmerClasses.h"

typedef enum {
  NAME_OR_LITERAL, PARAM, COMMA, VAR_VALUE, FURTHER_FUNC
} Expectation;

class Parser {
  public:
    ShimmerParam current_expr;
    Expectation expectation;
    DotStatement to_add;
    DotIdentifier id;
    int this_token_id;

    DotToken this_token;
    std::vector<DotToken> tokens;

    std::vector<DotStatement> statements;
    std::vector<ShimmerParam> params;

    DotTree to_return;

    Parser();
    DotTree parse(std::vector<DotToken>);

    void name_or_literal_expectation();
    void var_val_expectation();
    void comma_expectation();
    void further_func_expectation();
    void param_expectation();
};

void print_debug_info(DotTree x);
const char* param_recursive_str(ShimmerParam to_convert);
void print_statement_info(DotStatement i);