#include <vector>
#include <iostream>
#include <string>
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
  to_return = DotTree(std::vector<DotStatement>{});
}

Parser::Parser(std::vector<DotToken> _tokens, ShimmerParam start) {
  expectation = STATEMENT_OR_CALL;
	tokens = _tokens;
  to_return = DotTree(std::vector<DotStatement>{});
  std::cout << "to_return.size() before: " << to_return.get_tree().size() << "\n";
	to_return.get_tree()[0].set_expr(start);
  std::cout << "to_return.size() after: " << to_return.get_tree().size() << "\n";
}

DotTree Parser::parse() {
  // for (DotToken i : _tokens) {
  //   std::cout << "This token's type: " << i.get_token_type() << "\n";
  //   std::cout << "This token's contents: " << i.get_contents() << "\n";
  //   std::cout << "This token's line: " << i.get_line() << "\n\n";
  // }
  // DotTree temp;
  // return temp;

  for (this_token_id = 0; this_token_id < tokens.size(); this_token_id++) {
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

    std::cout << "Now on iteration #" << std::to_string(this_token_id) << " of for loop.\n";
    std::cout << "This token's type: " << this_token.get_token_type() << "\n";
    std::cout << "This token's contents: " << this_token.get_contents() << "\n";
    std::cout << "This token's line: " << this_token.get_line() << "\n\n";
  }

  return to_return;
}

void Parser::name_or_literal_expectation() {
  if (this_token.is_of_type("DotIdentifier")) {
    expectation = STATEMENT_OR_CALL;
    id = DotIdentifier(this_token.get_line(), this_token.get_contents());
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
    to_add.set_expr(ShimmerParam(id));
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
  std::vector<DotToken> tokens_for_recursion;

  int sub_expr_layer = 0;
  int j = this_token_id;
  while (true) {
    DotToken tok = tokens.at(j);
    if (tok.is_of_type("DotLParen")) {
      sub_expr_layer++;
    }
    else if (tok.is_of_type("DotRParen")) {
      sub_expr_layer--;
    }
    j++;
  }
  if (this_token.is_of_type("COMMA")) {
    // TODO this
  }
}

void Parser::param_expectation() {
  DotLiteral lit;

  if (this_token.is_of_type("DotIdentifier")) {
    lit = DotLiteral(this_token.get_line(), this_token.get_contents());
    params.push_back(ShimmerParam(lit));
    expectation = FURTHER_FUNC;
  }
  else if (this_token.is_of_type("DotInt") || \
           this_token.is_of_type("DotString")) {

    if (this_token.is_of_type("DotInt")) {
      lit = DotLiteral(this_token.get_line(), this_token.get_parsed_contents());
    }
    else if (this_token.is_of_type("DotString")) {
      lit = DotLiteral(this_token.get_line(), this_token.get_contents());
    }

    params.push_back(ShimmerParam(lit));
    expectation = COMMA;
  }
  else {
    throw_error("Expected function parameter but got: ", this_token.get_contents(), this_token.get_line());
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
          ShimmerParam param = ShimmerParam(to_push);
          params.push_back(param);
					tokens_for_recursion.clear();
*/


const char* param_recursive_str(ShimmerParam to_convert) {
  if (to_convert.get_param_type() == LITERAL) {
    DotLiteral liter = to_convert.get_literal_val();
    if (liter.type == TypeString || liter.type == TypeInt) {
      return liter.get_str().c_str();
    }
  }
  else if (to_convert.get_param_type() == STATEMENT) {
    for (ShimmerParam i : to_convert.get_statement_val().get_params()) {
      print_statement_info(i.get_statement_val());
    }
    return "Param recursive";
  }
}

void print_statement_info(DotStatement i) {
	std::cout << i.get_expr().get_identifier_val().get_contents() << "->";
	for (ShimmerParam j : i.get_params()) {
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
    Parser parser;
		DotTree parsed = parser.parse(lex(buffer.str()));
		std::cout << "Parsing done." << "\n";

		print_debug_info(parsed);  

		return "Test complete";
	}
}

#endif
