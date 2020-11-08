#include <any>
#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <unordered_map>

#include "ShimmerClasses.h"
#include "eval.h"

DotToken::DotToken() {
  // Default constructor does nothing
}

std::string DotToken::get_contents() {
  return contents;
}

int DotToken::get_parsed_contents(){
  return parsed_contents;
}

std::string DotToken::get_token_type() {
  return token_type;
}

bool DotToken::is_of_type(std::string type) {
  return token_type.compare(type) == 0;
}

DotLParen::DotLParen() {
  token_type = "DotLParen";
  // DotLParen will always be a '('
  contents = "(";
}

DotRParen::DotRParen() {
  token_type = "DotRParen";
  // DotRParen will always be a ')'
  contents = ")";
}

DotLBrace::DotLBrace() {
  token_type = "DotLBrace";
  // DotLBrace will always be a '{'
  contents = "{";
}

DotRBrace::DotRBrace() {
  token_type = "DotRBrace";
  // DotRBrace will always be a '}'
  contents = "}";
}

DotIDLiteralSign::DotIDLiteralSign() {
  token_type = "DotIDLiteralSign";
  contents = "$";
}

DotComma::DotComma() {
  token_type = "DotComma";
  // DotComma will always be a ','
  contents = ",";
}

DotInt::DotInt(std::string content) {
  token_type = "DotInt";
  contents = content;
  parsed_contents = std::stoi(content);
}

DotString::DotString(std::string content) {
  token_type = "DotString";
  contents = content;
}

DotIdentifier::DotIdentifier(std::string content) {
  token_type = "DotIdentifier";
  contents = content;
}

DotStatement::DotStatement() {
  // Default constructor does nothing
}

DotStatement::DotStatement(std::string ident, std::vector<ShimmerParam> param) {
  identifier = ident;
  params = param;
}

std::vector<ShimmerParam> DotStatement::get_params() {
  return params;
}

std::string DotStatement::get_identifier() {
  return identifier;
}

void DotStatement::set_params(std::vector<ShimmerParam> param) {
  params = param;
}

void DotStatement::set_identifier(std::string ident) {
  identifier = ident;
}

DotLiteral DotStatement::eval() {
  for (int i = 0; i < params.size(); i++) {
    if (params.at(i).is_of_type(STATEMENT)) {
      DotLiteral x = params.at(i).get_statement_val().eval();
      params.at(i) = ShimmerParam(x);
    }
  }
  if(std::string("help").compare(identifier) == 0) {
    std::cout << "Read readme.md\n";
  }
  else if (std::string("print").compare(identifier) == 0) {
    std::cout << params.at(0).get_literal_val().get_str() << "\n";
  }
  else if (std::string("add").compare(identifier) == 0) {
    return DotLiteral(params.at(0).get_literal_val().get_int() + \
                      params.at(1).get_literal_val().get_int());
  }
  else if (std::string("sub").compare(identifier) == 0) {
    return DotLiteral(params.at(0).get_literal_val().get_int() - \
                      params.at(1).get_literal_val().get_int());
  }
  else if (std::string("mult").compare(identifier) == 0) {
    return DotLiteral(params.at(0).get_literal_val().get_int() * \
                      params.at(1).get_literal_val().get_int());
  }
  else if (std::string("div").compare(identifier) == 0) {
    return DotLiteral(params.at(0).get_literal_val().get_int() / \
                      params.at(1).get_literal_val().get_int());
  }
  else if(std::string("input").compare(identifier) == 0) {
    std::cout << params.at(0).get_literal_val().get_str();
    std::string buffer;
    std::getline(std::cin, buffer);
    return DotLiteral(buffer);
  }
  return DotLiteral(0);
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

DotLiteral::DotLiteral(int val) {
  type = TypeInt;
  int_value = val;
}

DotLiteral::DotLiteral(std::string val) {
  type = TypeString;
  str_value = val;
}

DotLiteral::DotLiteral(DotTree val) {
  type = TypeFunc;
  func_value = val;
}

DotLiteral::DotLiteral(DotIdentifier val) {
  type = TypeId;
  id_value = val;
}

int DotLiteral::get_type() {
  return type;
}

int DotLiteral::get_int() {
  return int_value;
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

ShimmerParam::ShimmerParam(DotLiteral literal_value) {
  is_literal = true;
  is_id = false;
  literal_val = literal_value;
}

ShimmerParam::ShimmerParam(DotStatement statement_value) {
  is_literal = false;
  is_id = false;
  statement_val = statement_value;
}

ShimmerParam::ShimmerParam(DotIdentifier identifier_value) {
  is_literal = false;
  is_id = true;
  identifier_val = identifier_value;
}

ParamType ShimmerParam::get_param_type() {
  return param_type;
}

bool ShimmerParam::is_of_type(ParamType type) {
  return get_param_type() == type;
}

DotLiteral ShimmerParam::get_literal_val() {
  return literal_val;
}

DotStatement ShimmerParam::get_statement_val() {
  return statement_val;
}

DotStatement ShimmerParam::get_identifier_val() {
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

DotLiteral ShimmerScope::get_variable_by_name(std::string var_name) {
  if (current_scope.find(key) != current_scope.end) {
    return current_scope.at(var_name);
  }
  else {
    return *upper_scope.get_variable_by_name(var_name);
  }
}