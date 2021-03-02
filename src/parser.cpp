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
Parser::Parser(std::vector<DotToken> _tokens) {
  expectation = NAME_OR_LITERAL;
	tokens = _tokens;
  
}

Parser::Parser(std::vector<DotToken> _tokens, ShimmerExpr start) {
  expectation = STATEMENT_OR_CALL;
  tokens = _tokens;
  expr = start;
}

ShimmerUnclosedFunc Parser::parse_fn() {
  bool comma_expected = false;
  std::vector<DotIdentifier> ids;
  first_param = true;

  while (true) {
    this_token = tokens.at(++this_token_id);

    if (comma_expected) {
      if (this_token.is_of_type("DotRParen")) {
        break;
      }
      else if (this_token.not_of_type("DotComma")) {
        throw_error("Expected comma but got ", this_token.get_contents(), -1);
      }
      else {
        comma_expected = false;
      }
    }
    else {
      if (first_param && this_token.is_of_type("DotRParen")) {
        break;
      }

      ids.push_back(DotIdentifier(this_token.get_line(), this_token.get_contents()));
      comma_expected = true;
      first_param = false;
    }
  }

  std::vector<DotToken> tokens_for_recursion;
  this_token = tokens.at(++this_token_id);

  if (this_token.not_of_type("DotLBrace")) {
    throw_error("Expected opening brace for function body.", -1);
  }

  int fn_layer = 1;
  while (true) {
    this_token = tokens.at(++this_token_id);
    if (this_token.is_of_type("DotRBrace")) {
      if (--fn_layer == 0) {
        break;
      }
    }
    if (this_token.is_of_type("DotLBrace")) {
      fn_layer++;
    }
    tokens_for_recursion.push_back(this_token);
  }

  std::cout << "=== sub-parser ===\n";
  DotTree tree = Parser(tokens_for_recursion).parse();
  std::cout << "=== end sub-parser ===\n";
  ++this_token_id;

  return ShimmerUnclosedFunc(ids, tree);
}

DotTree Parser::parse() {
  for (this_token_id = 0; this_token_id < tokens.size(); this_token_id++) {
    std::cout << "Trying to parse\n";
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
  }

  to_return = DotTree(statements);
  return to_return;
}

void Parser::name_or_literal_expectation() {
  if (this_token.is_of_type("DotIdentifier")) {
    expectation = STATEMENT_OR_CALL;
    expr = ShimmerExpr(DotIdentifier(this_token.get_line(), this_token.get_contents()));
  }
  else if (this_token.is_of_type("DotInt")) {
    // Todo: Add straight value support (useful for functions)
  }
  else if (this_token.is_of_type("DotString")) {
    // Todo: Add straight value support (useful for functions)
  }
  else {
    throw_error("Expected name or value but got: ", this_token.get_contents(), this_token.get_line());
  }
}

void Parser::statement_or_call_expectation() {
  if (this_token.is_of_type("DotLParen")) {
    expectation = PARAM;
    first_param = true;
    to_add.set_expr(expr);
  }
  else if (this_token.is_of_type("DotInt")) {
    // Todo: Add straight value support (useful for functions)
  }
  else if (this_token.is_of_type("DotString")) {
    // Todo: Add straight value support (useful for functions)
  }
  else {
    throw_error("Expected variable definition but got: ", \
                this_token.get_contents(), this_token.get_line());
  }
}

void Parser::comma_expectation() {
  if (this_token.is_of_type("DotRParen")) {
    to_add.set_params(params);
    for (ShimmerExpr i : to_add.get_params()) {
      if (i.get_param_type() == LITERAL) {
        printf("address of param: %p\n", (void*) i.literal_val);
      }
      if (i.get_param_type() == STATEMENT) {
        printf("address of param: %p\n", (void*)i.statement_val);
      }
    }

    statements.push_back(to_add);
    params.clear();
    expectation = NAME_OR_LITERAL;
    to_add = DotStatement();

    return;
  }

  if (!this_token.is_of_type("DotComma")) {
    throw_error("Expected comma but got: ", this_token.get_contents(), this_token.get_line());
  }

  expectation = PARAM;
}

void Parser::further_func_expectation() {
  if (this_token.is_of_type("DotComma")) {
    ShimmerExpr* param = new ShimmerExpr(current_expr);
    params.push_back(*param);
    expectation = PARAM;
  }
  else if (this_token.is_of_type("DotRParen")) {
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
    to_add = DotStatement();
    return;
  }
  else if (!this_token.is_of_type("DotLParen")) {
    throw_error("Expected parenthesis or comma but got: ", this_token.get_contents(), this_token.get_line());
  }

  std::vector<DotToken> tokens_for_recursion;

  int sub_expr_layer = 0;
  int j = this_token_id;

  std::cout << "=== sub-parser === \n";

  while (true) {
    DotToken tok = tokens.at(j);
    tokens_for_recursion.push_back(tok);

    if (tok.is_of_type("DotRParen")) {
      sub_expr_layer--;
      std::cout << "Encountered DotRParen. New sub_expr_layer: " << sub_expr_layer << "\n";

      if (sub_expr_layer == 0) {
        break;
      }
    }

    if (tok.is_of_type("DotLParen")) {
      sub_expr_layer++;
      std::cout << "Encountered DotLParen. New sub_expr_layer: " << sub_expr_layer << "\n";
    }

    j++;

    if (j == tokens.size()) {
      throw_error("Expected closing parentheses but got: ", this_token.get_contents(), this_token.get_line());
    }
  }

  lex_to_str(tokens_for_recursion);

  this_token_id = j;

  Parser sub_parser = Parser(tokens_for_recursion, current_expr);
  DotTree* parsed = new DotTree(sub_parser.parse());
  std::cout << "=== end sub-parser ===\n";
  DotStatement* res = new DotStatement(parsed->get_tree().at(0));
  current_expr = ShimmerExpr(*res);
  ShimmerExpr* param = new ShimmerExpr(current_expr);
  params.push_back(*param);
  expectation = COMMA;
}

void Parser::param_expectation() {
  DotLiteral lit;
  if (this_token.is_of_type("DotRParen") && first_param) {
    for (ShimmerExpr i : params) {
      std::cout << i.get_literal_val().get_str();
    }
    to_add.set_params(params);
    statements.push_back(to_add);
    params.clear();
    expectation = NAME_OR_LITERAL;
    to_add = DotStatement();
    return;
  }
  else if (this_token.is_of_type("DotIdentifier")) {
    
    current_expr = ShimmerExpr(DotIdentifier(this_token.get_line(), this_token.get_contents()));
    expectation = FURTHER_FUNC;
    return;
  }
  else if (this_token.is_of_type("DotLParen")) {
    ShimmerUnclosedFunc fn = parse_fn();
    current_expr = ShimmerExpr(fn);
    expectation = FURTHER_FUNC;
    return;
  }
  else if (this_token.is_of_type("DotInt") || \
           this_token.is_of_type("DotString")) {
    DotLiteral* literal_val = new DotLiteral;
    if (this_token.is_of_type("DotInt")) {
      literal_val = new DotLiteral(this_token.get_line(), this_token.get_parsed_contents());
    }
    else if (this_token.is_of_type("DotString")) {
      literal_val = new DotLiteral(this_token.get_line(), this_token.get_contents());
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
    case NAME_OR_LITERAL:   return "Name or literal";
    case PARAM:             return "Param";
    case COMMA:             return "Comma";
    case STATEMENT_OR_CALL: return "Statement or call";
    case FURTHER_FUNC:      return "Further func";
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
    if (tokens.at(j).is_of_type("DotRParen")) {
      sub_expr_layer--;
      if (sub_expr_layer == 0) {
          break;
      }
    }
    if (tokens.at(j).is_of_type("DotLParen")) {
      sub_expr_layer++;
    }
  }
  
  tokens_for_recursion.push_back(tokens.at(j));
  i = j;
  DotTree parsed = parse(tokens_for_recursion);
  DotStatement to_push = parsed.get_tree().at(0);
  ShimmerExpr param = ShimmerExpr(to_push);
  params.push_back(param);
  tokens_for_recursion.clear();
*/


const char* param_recursive_str(ShimmerExpr to_convert) {
  if (to_convert.get_param_type() == LITERAL) {
    DotLiteral liter = to_convert.get_literal_val();
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

void print_statement_info(DotStatement i) {
	std::cout << i.get_expr().get_identifier_val().get_contents() << "->";
	for (ShimmerExpr j : i.get_params()) {
		std::cout << param_recursive_str(j) << " ";
	}
}

void print_debug_info(DotTree x) {
  for (DotStatement i : x.get_tree()) {
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
		DotTree parsed = parser.parse();
		std::cout << "Parsing done." << "\n";

		print_debug_info(parsed);  

		return "Test complete";
	}
}

#endif
