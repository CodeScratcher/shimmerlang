#include <vector>
#include <iostream>
#include <string>
#if defined BENCHMARK || defined DEBUG
#include <fstream>
#include <sstream>
#endif
#include "parser.h"
#include "lexer.h"
#include "ShimmerClasses.h"

DotTree parse(std::vector<DotToken> tokens) {
  std::vector<DotStatement> statements;
  DotStatement to_add;
  bool in_params = false;
  bool separated = true;
  bool is_id_token = false;
  std::vector<ShimmerParam> params;
  std::vector<DotToken> tokens_for_recursion;
  lex_to_str(tokens);
  for (int i = 0; i < tokens.size(); i++) {
    
    DotToken this_token = tokens.at(i);
    //DotToken next_token = tokens.at(i+1);

    if (this_token.is_of_type("DotIdentifier") && !in_params) {
      DotIdentifier x(this_token.get_line_number(), this_token.get_contents());
      ShimmerParam y(x);
      to_add.set_expr(y);
    }
    else if (this_token.is_of_type("DotLParen") && !in_params) {
      in_params = true;
    }
    else if (this_token.is_of_type("DotRParen")) {
      if (!in_params) {
        throw_error("Unexpected right parenthesis.", this_token.get_line_number());
      }

      in_params = false;
      separated = true;
      to_add.set_params(params);
      params.clear();
      statements.push_back(to_add);
			to_add = DotStatement();
    }
    else if (this_token.is_of_type("DotIDLiteralSign")) {
      is_id_token = true;
    }
    else if (is_id_token) {
      if (this_token.is_of_type("DotIdentifier")) {
        DotIdentifier x(this_token.get_line_number(), this_token.get_contents());
        DotLiteral y(this_token.get_line_number(), x);
        params.push_back(ShimmerParam(y));
        is_id_token = false;
      }
      else {
        std::string message = "Expected identifier but got: " + this_token.get_contents();
        throw_error(message, this_token.get_line_number());
      } 
    }
    else if (this_token.is_of_type("DotIdentifier") && in_params) {
      if (tokens.at(i+1).is_of_type("DotLParen")) {///////////next_token
        if (separated) {
          int sub_expr_layer = 0;
          int j = i;
          while(true) {
            tokens_for_recursion.push_back(tokens.at(j));
            j++; 
            if (j == tokens.size()) {
              throw_error("Missing closing parenthesis.", this_token.get_line_number());
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
        }
        else {
          std::cout << "Location #1\n";
          std::string message = "Missing a comma at token: " + this_token.get_contents();
          throw_error(message, this_token.get_line_number());
        }
      }
      else {
        if (separated) {
          DotIdentifier x(this_token.get_line_number(), this_token.get_contents());
          ShimmerParam y(x);
          params.push_back(y);
          separated = false;
        }
        else {
          std::cout << "Location #2\n";
          std::string message = "Missing a comma at token: " + this_token.get_contents();
          throw_error(message, this_token.get_line_number());
        }
      }
    }
    else if (this_token.is_of_type("DotInt")) {
      if (separated) {
        int line = this_token.get_line_number();

        if (tokens.at(i+1).is_of_type("DotLParen")){////////////////next_token
          std::string message = "Int cannot be called: " + this_token.get_contents();
          throw_error(message, this_token.get_line_number());
        }

        DotLiteral x(line, this_token.get_parsed_contents());
        ShimmerParam y(x);
        params.push_back(y);
        separated = false;
      }
      else {
        std::cout << "Location #3\n";
        std::string message = "Missing a comma at token: " + this_token.get_contents();
        throw_error(message, this_token.get_line_number());
      }
    }
    else if (this_token.is_of_type("DotString")) {
      if (separated) {
        int line = this_token.get_line_number();

        if (tokens.at(i+1).is_of_type("DotLParen")) {//////////////next_token
          std::string message = "String cannot be called: " + this_token.get_contents();
          throw_error(message, this_token.get_line_number());
        }

        DotLiteral x(line, this_token.get_contents());
        ShimmerParam y(x);
        params.push_back(y);
        separated = false;
      }
      else {
        std::cout << "Location #4\n";
        std::string message = "Missing a comma at token: " + this_token.get_contents();
        throw_error(message, this_token.get_line_number());
      }
    }
    else if (this_token.is_of_type("DotComma")) {
      std::cout << "Comma\n";
      separated = true;
    }
  }

  if (in_params) {
    throw_error("Missing closing parenthesis.", "end");
  }
  DotTree toReturn = DotTree(statements); 
  return toReturn;
}

void throw_error(std::string msg, std::string line) {
  std::string line_str = line + ":\n\t";
  throw std::runtime_error(line_str + msg);
}

void throw_error(std::string msg, int line) {
  std::string line_str = std::to_string(line) + ":\n\t";
  throw std::runtime_error(line_str + msg);
}

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
		DotTree x = parse(lex(buffer.str()));
		std::cout << "Parsing done." << "\n";

		print_debug_info(x);  

		return "Test complete";
	}
}

#endif
