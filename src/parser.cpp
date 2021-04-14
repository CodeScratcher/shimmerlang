#include <vector>
#include <iostream>
#include <string>
#include <signal.h>

#if defined BENCHMARK || defined DEBUG
#include <fstream>
#include <sstream>
#endif

#include "errors.h"
#include "parser.h"
#include "lexer.h"
#include "ShimmerClasses.h"

// Parser constructor
Parser::Parser(std::vector<ShimmerToken> _tokens) {
  expectation = NAME_OR_LITERAL;
	tokens = _tokens;
  
}

Parser::Parser(std::vector<ShimmerToken> _tokens, ShimmerExpr start) {
  expectation = STATEMENT_OR_CALL;
  tokens = _tokens;
  expr = start;
}

ShimmerUnclosedFunc Parser::parse_fn() {
  bool comma_expected = false;
  std::vector<ShimmerIdentifier> ids;
  first_param = true;

  while (true) {
    this_token = tokens.at(++this_token_id);

    if (comma_expected) {
      if (this_token.is_of_type("ShimmerRParen")) {
        break;
      }
      else if (this_token.not_of_type("ShimmerComma")) {
        throw_error("Expected comma but got ", this_token.get_contents(), -1);
      }
      else {
        comma_expected = false;
      }
    }
    else {
      if (first_param && this_token.is_of_type("ShimmerRParen")) {
        break;
      }

      ids.push_back(ShimmerIdentifier(this_token.get_line(), this_token.get_contents()));
      comma_expected = true;
      first_param = false;
    }
  }

  std::vector<ShimmerToken> tokens_for_recursion;
  this_token = tokens.at(++this_token_id);

  if (this_token.not_of_type("ShimmerLBrace")) {
    throw_error("Expected opening brace for function body.", -1);
  }

  int fn_layer = 1;

  while (true) {
    this_token = tokens.at(++this_token_id);

    if (this_token.is_of_type("ShimmerRBrace")) {
      if (--fn_layer == 0) {
        break;
      }
    }

    if (this_token.is_of_type("ShimmerLBrace")) {
      fn_layer++;
    }

    tokens_for_recursion.push_back(this_token);
  }

  std::cout << "=== begin sub-parser ===\n";
  ShimmerTree tree = Parser(tokens_for_recursion).parse();
  std::cout << "=== end sub-parser ===\n";
  ++this_token_id;

  return ShimmerUnclosedFunc(ids, tree);
}

ShimmerTree Parser::parse() {
  for (this_token_id = 0; this_token_id < tokens.size(); this_token_id++) {
    std::cout << "Parsing token number #" << std::to_string(this_token_id) << "\n";

    this_token = tokens.at(this_token_id);

    if (expectation == NAME_OR_LITERAL) {
      name_or_literal_expectation();
    }
    else if (expectation == STATEMENT_OR_CALL) {
      statement_or_call_expectation();
    }
    else if (expectation == COMMA) {
      comma_expectation();
    }
    else if (expectation == FURTHER_FUNC) {
      further_func_expectation();
    }
    else if (expectation == PARAM) {
      param_expectation();
    }
    else if (expectation == )
  }

  to_return = ShimmerTree(statements);
  return to_return;
}

void Parser::name_or_literal_expectation() {
  if (this_token.is_of_type("ShimmerIdentifier")) {
    expectation = STATEMENT_OR_CALL;
    expr = ShimmerExpr(ShimmerIdentifier(this_token.get_line(), this_token.get_contents()));
  }
  else if (this_token.is_of_type("ShimmerInt")) {
    // Todo: Add straight value support (useful for functions)
  }
  else if (this_token.is_of_type("ShimmerString")) {
    // Todo: Add straight value support (useful for functions)
  }
  else {
    throw_error("Expected name or value but got: ", this_token.get_contents(), this_token.get_line());
  }
}

void Parser::statement_or_call_expectation() {
  if (this_token.is_of_type("ShimmerLParen")) {
    expectation = PARAM;
    first_param = true;
    to_add.set_expr(expr);
  }
  else if (this_token.is_of_type("ShimmerInt")) {
    // Todo: Add straight value support (useful for functions)
  }
  else if (this_token.is_of_type("ShimmerString")) {
    // Todo: Add straight value support (useful for functions)
  }
  else {
    throw_error("Expected variable definition but got: ", \
                this_token.get_contents(), this_token.get_line());
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

    statements.push_back(to_add);
    params.clear();
    expectation = NAME_OR_LITERAL;
    to_add = ShimmerStatement();

    return;
  }

  if (!this_token.is_of_type("ShimmerComma")) {
    throw_error("Expected comma but got: ", this_token.get_contents(), this_token.get_line());
  }

  expectation = PARAM;
}

void Parser::further_func_expectation() {
  if (this_token.is_of_type("ShimmerComma")) {
    ShimmerExpr* param = new ShimmerExpr(current_expr);
    params.push_back(*param);
    expectation = PARAM;
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
    
    statements.push_back(to_add);
    params.clear();
    expectation = NAME_OR_LITERAL;
    printf("address of to_add: %p\n", (void*) &to_add);
    to_add = ShimmerStatement();
    return;
  }
  else if (!this_token.is_of_type("ShimmerLParen")) {
    throw_error("Expected parenthesis or comma but got: ", this_token.get_contents(), this_token.get_line());
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
      throw_error("Expected closing parentheses but got: ", this_token.get_contents(), this_token.get_line());
    }
  }

  lex_to_str(tokens_for_recursion);

  this_token_id = j;

  Parser sub_parser = Parser(tokens_for_recursion, current_expr);
  ShimmerTree* parsed = new ShimmerTree(sub_parser.parse());
  std::cout << "=== end sub-parser ===\n";
  ShimmerStatement* res = new ShimmerStatement(parsed->get_tree().at(0));
  current_expr = ShimmerExpr(*res);
  ShimmerExpr* param = new ShimmerExpr(current_expr);
  params.push_back(*param);
  expectation = COMMA;
}

void Parser::param_expectation() {
  ShimmerLiteral lit;
  if (this_token.is_of_type("ShimmerRParen") && first_param) {
    for (ShimmerExpr i : params) {
      std::cout << i.get_literal_val().get_str();
    }
    to_add.set_params(params);
    statements.push_back(to_add);
    params.clear();
    expectation = NAME_OR_LITERAL;
    to_add = ShimmerStatement();
    return;
  }
  else if (this_token.is_of_type("ShimmerIdentifier")) {
    
    current_expr = ShimmerExpr(ShimmerIdentifier(this_token.get_line(), this_token.get_contents()));
    expectation = FURTHER_FUNC;
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
    throw_error("Expected function parameter but got: ", this_token.get_contents(), this_token.get_line());
  }
  first_param = false;
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
    case STATEMENT_OR_CALL: return "Statement or Call";
    case FURTHER_FUNC:      return "Further Func";
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
      throw_error("Missing closing parenthesis.", this_token.get_line(), this_token.get_line());
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
  for (ShimmerStatement i : x.get_tree()) {
		print_statement_info(i);
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
