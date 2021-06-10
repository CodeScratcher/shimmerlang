#include <iostream>
#include <signal.h>
#include <string>
#include <vector>

#if defined BENCHMARK || defined DEBUG
#include <fstream>
#include <sstream>
#endif

#include "ShimmerClasses.h"
#include "errors.h"
#include "lexer.h"
#include "parser.h"

// Parser constructor
Parser::Parser(std::vector<ShimmerToken> _tokens) {
  expectation = NAME_OR_LITERAL;
	tokens = _tokens;
}

Parser::Parser(std::vector<ShimmerToken> _tokens, ShimmerExpr start) {
  expectation = ID_OR_CALL;
  tokens = _tokens;
  expr = start;
}

ShimmerUnclosedFunc Parser::parse_fn() {
  bool comma_expected = false;
  std::vector<ShimmerIdentifier> args;
  first_param = true;

  while (true) {
    this_token = tokens.at(++this_token_id);

    if (comma_expected) {
      if (this_token.is_of_type("ShimmerRParen")) {
        break;
      }
      else if (this_token.is_of_type("ShimmerComma")) {
        comma_expected = false;
      }
      else {
        throw_error(-1, "Expected comma or parenthesis but got: ", this_token.get_contents());
      }
    }
    else {
      if (first_param && this_token.is_of_type("ShimmerRParen")) {
        break;
      }

      args.push_back(ShimmerIdentifier(this_token.get_line(), this_token.get_contents()));
      comma_expected = true;
      first_param = false;
    }
  }

  std::vector<ShimmerToken> tokens_for_recursion;
  this_token = tokens.at(++this_token_id);

  if (this_token.not_of_type("ShimmerLBrace")) {
    throw_error(this_token.get_line(), "Expected opening brace for function body.");
  }

  int fn_layer = 1;

  while (true) {
    this_token = tokens.at(++this_token_id);

    if (this_token.is_of_type("ShimmerLBrace")) {
      fn_layer++;
    }
    else if (this_token.is_of_type("ShimmerRBrace")) {
      if (--fn_layer == 0) {
        break;
      }
    }

    tokens_for_recursion.push_back(this_token);
  }

  std::cout << "=== begin sub-parser ===\n";
  // lex_to_str(tokens_for_recursion);
  ShimmerTree tree = Parser(tokens_for_recursion).parse();
  std::cout << "=== end sub-parser ===\n";

  return ShimmerUnclosedFunc(args, tree);
}

ShimmerTree Parser::parse() {
  for (this_token_id = 0; this_token_id < tokens.size(); this_token_id++) {
    this_token = tokens.at(this_token_id);
    handle_expectation(expectation);
  }

  if (expectation == ID_OR_CALL) {
    
    statements.push_back(ShimmerExpr(expr));

  }

  to_return = ShimmerTree(statements);
  return to_return;
}

void Parser::handle_expectation(Expectation exp) {
  if      (exp == NAME_OR_LITERAL) name_or_literal_expectation();
  else if (exp == ID_OR_CALL)      id_or_call_expectation();
  else if (exp == COMMA)           comma_expectation();
  else if (exp == FURTHER_FUNC)    further_func_expectation();
  else if (exp == PARAM)           param_expectation();
  else if (exp == SYMBOL)          symbol_expectation();
}

void Parser::name_or_literal_expectation() {
  if (this_token.is_of_type("ShimmerIdentifier")) {
    if (this_token.get_contents() == "true") {
      statements.push_back(
        ShimmerExpr(ShimmerLiteral(-1, true))
      );
    }
    else if (this_token.get_contents() == "false") {
      statements.push_back(
        ShimmerExpr(ShimmerLiteral(-1, false))
      );
    }
    else {
      expectation = ID_OR_CALL;
      expr = ShimmerExpr(ShimmerIdentifier(this_token.get_line(), this_token.get_contents()));
    }
  }
  else if (this_token.is_of_type("ShimmerInt")) {
    statements.push_back(
      ShimmerExpr(ShimmerLiteral(this_token.get_line(), this_token.get_parsed_contents()))
    );
  }
  else if (this_token.is_of_type("ShimmerString")) {
    statements.push_back(
      ShimmerExpr(ShimmerLiteral(this_token.get_line(), this_token.get_contents()))
    );
  }
  else if (this_token.is_of_type("ShimmerLParen")) {
    ShimmerUnclosedFunc fn = parse_fn();
    expr = ShimmerExpr(fn);
    expectation = ID_OR_CALL;
  }
  else {
    throw_error(this_token.get_line(), "Expected name or value but got: ", this_token.get_contents());
  }
}

void Parser::id_or_call_expectation() {
  if (this_token.is_of_type("ShimmerLParen")) {
    bool call = true;
    int new_id = this_token_id;
    int fn_layer = 1;
    ShimmerToken peek_token;

    while (true) {
      peek_token = tokens.at(++new_id);

      if (peek_token.is_of_type("ShimmerLParen")) {
        fn_layer++;
      }
      else if (peek_token.is_of_type("ShimmerRParen")) {
        if (--fn_layer == 0) {
          break;
        }
      }
    }

    if (new_id < tokens.size() - 1) {
      peek_token = tokens.at(++new_id);

      if (peek_token.is_of_type("ShimmerLBrace")) {
        call = false;
      }
    }

    if (call) {
      first_param = true;
      to_add.set_expr(expr);
      expectation = PARAM;
    }
    else {
      statements.push_back(ShimmerExpr(expr));
      ShimmerUnclosedFunc fn = parse_fn();
      expr = ShimmerExpr(fn);
      expectation = ID_OR_CALL;
    }
  }
#                                                    warning Clean up here!
  else if (this_token.is_of_type("ShimmerInt")) {
    statements.push_back(ShimmerExpr(expr));
    ShimmerLiteral* literal_val = new ShimmerLiteral;
    literal_val = new ShimmerLiteral(this_token.get_line(), this_token.get_parsed_contents());
    statements.push_back(ShimmerExpr(*literal_val));
    expectation = NAME_OR_LITERAL;
  }
  else if (this_token.is_of_type("ShimmerString")) {
    statements.push_back(ShimmerExpr(expr));
    ShimmerLiteral* literal_val = new ShimmerLiteral;
    literal_val = new ShimmerLiteral(this_token.get_line(), this_token.get_contents());
    statements.push_back(ShimmerExpr(*literal_val));
    expectation = NAME_OR_LITERAL;
  }
  else if (this_token.is_of_type("ShimmerIdentifier")) {
    statements.push_back(ShimmerExpr(expr));
    if (this_token.get_contents() == "true") {
      statements.push_back(
        ShimmerExpr(ShimmerLiteral(-1, true))
      );
      expectation = NAME_OR_LITERAL;
    }
    else if (this_token.get_contents() == "false") {
      statements.push_back(
        ShimmerExpr(ShimmerLiteral(-1, false))
      );
      expectation = NAME_OR_LITERAL;
    }
    else {
      expectation = ID_OR_CALL;
      expr = ShimmerExpr(ShimmerIdentifier(this_token.get_line(), this_token.get_contents()));
    }
  }
  else {
    throw_error(
      this_token.get_line(),
      "Expected variable definition but got: ",
      this_token.get_contents()
    );
  }
}

void Parser::comma_expectation() {
  if (this_token.is_of_type("ShimmerRParen")) {
    to_add.set_params(params);
    for (ShimmerExpr i : to_add.get_params()) {
      if (i.get_param_type() == LITERAL) {
        printf("address of literal param: %p\n", (void*) i.literal_val);
      }
      if (i.get_param_type() == STATEMENT) {
        printf("address of statement param: %p\n", (void*) i.statement_val);
      }
    }

    params.clear();
    expr = ShimmerExpr(to_add);
    expectation = ID_OR_CALL;
    to_add = ShimmerStatement();

    return;
  }

  if (this_token.not_of_type("ShimmerComma")) {
    throw_error(this_token.get_line(), "Expected comma but got: ", this_token.get_contents());
  }

  expectation = PARAM;
}

void Parser::further_func_expectation() {
  if (this_token.is_of_type("ShimmerComma")) {
    ShimmerExpr* param = new ShimmerExpr(current_expr);
    params.push_back(*param);
    expectation = PARAM;
    return;
  }
  else if (this_token.is_of_type("ShimmerRParen")) {
  	ShimmerExpr* param = new ShimmerExpr(current_expr);
    printf("new printing: %p\n", (void*) param);
    params.push_back(*param);
    to_add.set_params(params);

    for (ShimmerExpr i : to_add.get_params()) {
      if (i.get_param_type() == STATEMENT) {
        printf("address of param: %p\n", (void*) i.statement_val);
      }
    }
    
    expr = ShimmerExpr(to_add);
    params.clear();
    expectation = ID_OR_CALL;
    printf("address of to_add: %p\n", (void*) &to_add);
    to_add = ShimmerStatement();
    return;
  }
  else if (!this_token.is_of_type("ShimmerLParen")) {
    throw_error(this_token.get_line(), "Expected parenthesis or comma but got: ", this_token.get_contents());
  }

  std::vector<ShimmerToken> tokens_for_recursion;

  int sub_expr_layer = 0;
  int j = this_token_id;

  std::cout << "=== sub-parser === \n";

  while (true) {
    ShimmerToken tok = tokens.at(j);
    tokens_for_recursion.push_back(tok);

    if (tok.is_of_type("ShimmerRParen")) {
      sub_expr_layer--;
      std::cout << "Encountered ShimmerRParen. New sub_expr_layer: " << sub_expr_layer << "\n";

      if (sub_expr_layer == 0) {
        break;
      }
    }

    if (tok.is_of_type("ShimmerLParen")) {
      sub_expr_layer++;
      std::cout << "Encountered ShimmerLParen. New sub_expr_layer: " << sub_expr_layer << "\n";
    }

    j++;

    if (j == tokens.size()) {
      throw_error(this_token.get_line(), "Expected closing parentheses but got: ", this_token.get_contents());
    }
  }

  lex_to_str(tokens_for_recursion);

  this_token_id = j;

  Parser sub_parser = Parser(tokens_for_recursion, current_expr);
  ShimmerTree* parsed = new ShimmerTree(sub_parser.parse());
  std::cout << "=== end sub-parser ===\n";
  current_expr = ShimmerExpr(parsed->get_tree().at(0));
  expectation = FURTHER_FUNC;
}

void Parser::param_expectation() {
  ShimmerLiteral lit;
  if (this_token.is_of_type("ShimmerRParen") && first_param) {
    to_add.set_params(params);
    for (ShimmerExpr i : params) {
      std::cout << i.get_literal_val().get_str();
    }
    expr = ShimmerExpr(to_add);
    params.clear();
    expectation = ID_OR_CALL;
    to_add = ShimmerStatement();
    return;
  }
  else if (this_token.is_of_type("ShimmerIDLiteralSign")) {
    expectation = SYMBOL;
  }
  else if (this_token.is_of_type("ShimmerIdentifier")) {
    if (this_token.get_contents() == "true") {
      ShimmerLiteral* literal_val = new ShimmerLiteral;

      literal_val = new ShimmerLiteral(-1, true);
      
      params.push_back(ShimmerExpr(*literal_val));
      expectation = COMMA;
    }
    else if (this_token.get_contents() == "false") {
      ShimmerLiteral* literal_val = new ShimmerLiteral;

      literal_val = new ShimmerLiteral(-1, false);
      
      params.push_back(ShimmerExpr(*literal_val));
      expectation = COMMA;
    }
    else {
      current_expr = ShimmerExpr(ShimmerIdentifier(this_token.get_line(), this_token.get_contents()));
      expectation = FURTHER_FUNC;
    }
    return;
  }
  else if (this_token.is_of_type("ShimmerLParen")) {
    ShimmerUnclosedFunc fn = parse_fn();
    current_expr = ShimmerExpr(fn);
    expectation = FURTHER_FUNC;
    return;
  }
  else if (
    this_token.is_of_type("ShimmerInt") ||
    this_token.is_of_type("ShimmerString")
  ) {
    ShimmerLiteral* literal_val = new ShimmerLiteral;

    if (this_token.is_of_type("ShimmerInt")) {
      literal_val = new ShimmerLiteral(this_token.get_line(), this_token.get_parsed_contents());
    }
    else if (this_token.is_of_type("ShimmerString")) {
      literal_val = new ShimmerLiteral(this_token.get_line(), this_token.get_contents());
    }

    params.push_back(ShimmerExpr(*literal_val));
    expectation = COMMA;
  }
  else {
    throw_error(this_token.get_line(), "Expected function parameter but got: ", this_token.get_contents());
  }
  first_param = false;
}

void Parser::symbol_expectation() {
  if (this_token.not_of_type("ShimmerIdentifier")) {
    throw_error(this_token.get_line(), "Expected identifier but got: ", this_token.get_contents());
  }

  expr = ShimmerExpr(
    ShimmerLiteral(
      this_token.get_line(), 
      ShimmerIdentifier(this_token.get_line(), this_token.get_contents())
    )
  );

  params.push_back(expr);
  expectation = COMMA;
}

void Parser::print_tokens() {
  for (int i = 0; i < tokens.size(); ++i) {
    print_token(i);
  }
}

void Parser::print_token(int i) {
  std::cout << "Token #" << std::to_string(i);
  std::cout << "\tis a " << tokens.at(i).get_token_type();
  std::cout << "\t\twith contents \"" << tokens.at(i).get_contents();
  std::cout << "\"\t\ton line " << tokens.at(i).get_line();
  std::cout << ",\t expected to be a " << get_expectation_name(expectation) << ".\n";
}

const char* get_expectation_name(Expectation expect)
{
  switch (expect) 
  {
    case NAME_OR_LITERAL:   return "Name or Literal";
    case PARAM:             return "Param";
    case COMMA:             return "Comma";
    case ID_OR_CALL: return "Statement or Call";
    case FURTHER_FUNC:      return "Further Func";
    case SYMBOL:            return "Symbol";
  }
}

/* Old code for look ahead
if (separated) {
  int sub_expr_layer = 0;
  int j = i;
  while(true) {
    tokens_for_recursion.push_back(tokens.at(j));
    j++; 
    if (j == tokens.size()) {
      throw_error(this_token.get_line(), "Missing closing parenthesis.", this_token.get_line());
    }
    if (tokens.at(j).is_of_type("ShimmerRParen")) {
      sub_expr_layer--;
      if (sub_expr_layer == 0) {
          break;
      }
    }
    if (tokens.at(j).is_of_type("ShimmerLParen")) {
      sub_expr_layer++;
    }
  }
  
  tokens_for_recursion.push_back(tokens.at(j));
  i = j;
  ShimmerTree parsed = parse(tokens_for_recursion);
  ShimmerStatement to_push = parsed.get_tree().at(0);
  ShimmerExpr param = ShimmerExpr(to_push);
  params.push_back(param);
  tokens_for_recursion.clear();
*/


const char* param_recursive_str(ShimmerExpr to_convert) {
  if (to_convert.get_param_type() == LITERAL) {
    ShimmerLiteral liter = to_convert.get_literal_val();
    if (liter.type == TypeString || liter.type == TypeInt) {
      return liter.get_str().c_str();
    }
  }
  else if (to_convert.get_param_type() == STATEMENT) {
    for (ShimmerExpr i : to_convert.get_statement_val().get_params()) {
      print_statement_info(i.get_statement_val());
    }

    return "Param recursive";
  }
}

void print_statement_info(ShimmerStatement i) {
	std::cout << i.get_expr().get_identifier_val().get_contents() << "->";
	for (ShimmerExpr j : i.get_params()) {
		std::cout << param_recursive_str(j) << " ";
	}
}

void print_debug_info(ShimmerTree x) {
  for (ShimmerExpr i : x.get_tree()) {
    if (i.is_of_type(STATEMENT)) print_statement_info(i.get_statement_val());
  }
}

#ifdef DEBUG

const char* parse_test() {
	std::ifstream file;
  file.open("test.shmr");

  if (!file) {
    std::cout << "Error while opening file.";
    return "Error while opening file.";
  }
  else {
		std::stringstream buffer;
    buffer << file.rdbuf();
    Parser parser(lex(buffer.str()));
		ShimmerTree parsed = parser.parse();
		std::cout << "Parsing done." << "\n";

		print_debug_info(parsed);  

		return "Test complete";
	}
}

#endif
