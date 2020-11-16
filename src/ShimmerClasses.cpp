#include <any>
#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <unordered_map>

#include "ShimmerClasses.h"
// #include "parser.h"
#include "tree_print.h"
#include "eval.h"

DotToken::DotToken() {
  // Default constructor does nothing
}
int DotToken::get_line_number() {
  return line_number;
}

std::string DotToken::get_contents() {
  return contents;
}

int DotToken::get_parsed_contents() {
  return parsed_contents;
}

std::string DotToken::get_token_type() {
  return token_type;
}

bool DotToken::is_of_type(std::string type) {
  return token_type.compare(type) == 0;
}

DotLParen::DotLParen(int line) {
  line_number = line;
  token_type = "DotLParen";
  // DotLParen will always be a '('
  contents = "(";
}

DotRParen::DotRParen(int line) {
  line_number = line;
  token_type = "DotRParen";
  // DotRParen will always be a ')'
  contents = ")";
}

DotLBrace::DotLBrace(int line) {
  line_number = line;
  token_type = "DotLBrace";
  // DotLBrace will always be a '{'
  contents = "{";
}

DotRBrace::DotRBrace(int line) {
  line_number = line;
  token_type = "DotRBrace";
  // DotRBrace will always be a '}'
  contents = "}";
}

DotIDLiteralSign::DotIDLiteralSign(int line) {
  line_number = line;
  token_type = "DotIDLiteralSign";
  contents = "$";
}

DotComma::DotComma(int line) {
  line_number = line;
  token_type = "DotComma";
  // DotComma will always be a ','
  contents = ",";
}

DotInt::DotInt(int line, std::string content) {
  line_number = line;
  token_type = "DotInt";
  contents = content;
  parsed_contents = std::stoi(content);
}

DotString::DotString(int line, std::string content) {
  line_number = line;
  token_type = "DotString";
  contents = content;
}


DotIdentifier::DotIdentifier() {
  // Default constructor does nothing
}

DotIdentifier::DotIdentifier(int line, std::string content) {
  token_type = "DotIdentifier";
  contents = content;
}

DotStatement::DotStatement() {
  // Default constructor does nothing
}

DotStatement::DotStatement(ShimmerParam exp, std::vector<ShimmerParam> param) {
  expr = exp;
  params = param;
}

std::vector<ShimmerParam> DotStatement::get_params() {
  return params;
}

ShimmerParam DotStatement::get_expr() {
  return expr;
}

void DotStatement::set_params(std::vector<ShimmerParam> param) {
  params = param;
}

void DotStatement::set_expr(ShimmerParam expr) {
  this->expr = expr;
}

DotLiteral DotStatement::eval(ShimmerScope* scope) {
  pretty_print(*this);
  for (int i = 0; i < params.size(); i++) {
    if (params.at(i).is_of_type(STATEMENT)) {
      DotLiteral x = params.at(i).get_statement_val().eval(scope);
      params.at(i) = ShimmerParam(x);
    }
    else if (params.at(i).is_of_type(IDENTIFIER)) {
      DotLiteral x = scope->get_variable(params.at(i).get_identifier_val().get_contents());
      params.at(i) = ShimmerParam(x);
    }
  }
	/* TODO: Create evaluator inc. functions */
  return DotLiteral(-1, 0); // line is -1 because we can't figure out the line number
}

void DotStatement::error_on_missing_params(int line, int min, std::string msg) {
  //print_statement_info(*this);
  pretty_print(*this);
  std::cout << "\n";
  if (params.size() < min) {
    throw std::runtime_error(std::to_string(line) + std::string(":\n\t") + msg);
  }
}

void DotStatement::error_on_extra_params(int line, int max, std::string msg) {
  //print_statement_info(*this);
  pretty_print(*this);
  std::cout << "\n";
  if (params.size() > max) {
    throw std::runtime_error(std::to_string(line) + std::string(":\n\t") + msg);
  }
}

DotTree::DotTree() {
  // Default constructor does nothing
}

DotTree::DotTree(std::vector<DotStatement> statements) {
  tree = statements;
}

std::vector<DotStatement> DotTree::get_tree() {
  return tree;
}

DotLiteral::DotLiteral() {
  // Default constructor does nothing
}

DotLiteral::DotLiteral(int line, int val) {
  type = TypeInt;
  int_value = val;
}

DotLiteral::DotLiteral(int line, std::string val) {
  type = TypeString;
  str_value = val;
}

DotLiteral::DotLiteral(int line, DotTree val) {
  type = TypeFunc;
  func_value = val;
}

DotLiteral::DotLiteral(int line, DotIdentifier val) {
  type = TypeId;
  id_value = val;
}

int DotLiteral::get_type() {
  return type;
}

int DotLiteral::get_int() {
	 if (type == TypeString) {
    return std::stoi(str_value);
  }
  else return int_value;
}

bool DotLiteral::get_bool() {
  return bool_value;
}

std::string DotLiteral::get_str() {
  if (type == TypeInt) {
    return std::to_string(int_value);
  }
  else return str_value;
}

DotTree DotLiteral::get_func() {
  return func_value;
}

DotIdentifier DotLiteral::get_id() {
  return id_value;
}

ShimmerParam::ShimmerParam() {
  // Default constructor does nothing
}

ShimmerParam::ShimmerParam(DotLiteral& literal_value) {
  param_type = LITERAL;
  literal_val = &literal_value;
}

ShimmerParam::ShimmerParam(DotStatement& statement_value) {
  param_type = STATEMENT;
  statement_val = &statement_value;
}

ShimmerParam::ShimmerParam(DotIdentifier identifier_value) {
  param_type = IDENTIFIER;
  identifier_val = identifier_value;
}

ParamType ShimmerParam::get_param_type() {
  return param_type;
}

bool ShimmerParam::is_of_type(ParamType type) {
  return get_param_type() == type;
}

DotLiteral ShimmerParam::get_literal_val() {
  return *literal_val;
}

DotStatement ShimmerParam::get_statement_val() {
  return *statement_val;
}

DotIdentifier ShimmerParam::get_identifier_val() {
  return identifier_val;
}

ShimmerScope::ShimmerScope() {
  // Default constructor does nothing
}

ShimmerScope::ShimmerScope(Scope cur_scope) {
  upper_scope = nullptr;
  current_scope = cur_scope;
}

ShimmerScope::ShimmerScope(ShimmerScope* up_scope, Scope cur_scope) {
  upper_scope = up_scope;
  current_scope = cur_scope;
}

DotLiteral ShimmerScope::get_variable(std::string var_name) {
  if (current_scope.find(var_name) != current_scope.end()) {
    return current_scope.at(var_name);
  }
  else {
    if (upper_scope != nullptr) {
      return upper_scope->get_variable(var_name);
    }
    else {
      throw std::runtime_error("Variable not found: " + var_name);
    }
  }
}
void ShimmerScope::set_variable(std::string var_name, DotLiteral val) {
  if (current_scope.find(var_name) != current_scope.end()) {
    current_scope.at(var_name) = val;
  }
  else {
    upper_scope->set_variable(var_name, val);
  }
}
void ShimmerScope::declare_variable(std::string var_name, DotLiteral val) {
  current_scope[var_name] = val;
}