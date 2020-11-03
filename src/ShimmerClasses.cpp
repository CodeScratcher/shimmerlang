#include <string>
#include "eval.h"
#include <vector>
#include <any>
#include <iostream>
#include <typeinfo>
#include "ShimmerClasses.h"

DotTree::DotTree(std::vector<DotStatement> statement) {
  statements = statement;
}

DotTree::DotTree() {
  
}

DotToken::DotToken() {

}

std::string DotToken::get_contents() {
  return contents;
}

std::string DotToken::get_token_type() {
  return token_type;
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
std::vector<DotLiteral> DotStatement::get_params() {
  return params;
}
void DotStatement::set_params(std::vector<DotLiteral> param) {
  params = param;
}
DotIdentifier::DotIdentifier(std::string content) {
  token_type = "DotIdentifier";
  // Identifier will have contents
  contents = content;
}

DotString::DotString(std::string content) {
  token_type = "DotString";
  contents = content;
}

DotInt::DotInt(std::string content) {
  token_type = "DotInt";
  contents = content;
  parsed_contents = std::stoi(content);
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

DotComma::DotComma() {
  token_type = "DotComma";
  // DotComma will always be a ','
  contents = ",";
}

DotLiteral::DotLiteral(int val) {
  type = TypeInt;
  int_value = val;
}

DotLiteral::DotLiteral(std::string val) {
  type = TypeString;
  str_value = val;
}

DotLiteral::DotLiteral(DotTree function) {
  type = TypeFunc;
  func_value = function;
}

DotLiteral::DotLiteral(DotStatement statement) { 
  DotLiteral evaluated = statement.eval();
  switch(evaluated.get_type()) {
    case TypeBool:
      bool_value = evaluated.get_bool();
      break;
    case TypeString:
      str_value = evaluated.get_str();
      break;
    case TypeFunc:
      func_value = evaluated.get_func();
      break;
    case TypeInt:
      int_value = evaluated.get_int();
      break;
  }
}

int DotLiteral::get_type() {
  return type;
}

int DotLiteral::get_int() {
  return int_value;
}

DotTree DotLiteral::get_func() {
  return func_value;
}

bool DotLiteral::get_bool() {
  return bool_value;
}

std::string DotLiteral::get_str() {
  std::cout << type;
  if (type == TypeInt) {
    return std::to_string(int_value);
  }
  else return str_value;
}

DotStatement::DotStatement(std::string ident, std::vector<DotLiteral> param) {
  identifier = ident;
  params = param;
}

std::string DotStatement::get_identifier() {
  return identifier;
}

void DotStatement::set_identifier(std::string ident) {
  identifier = ident;
}

DotStatement::DotStatement() {}
std::vector<DotStatement> DotTree::get_tree() {
  return statements;
}

DotLiteral DotStatement::eval() {
  
  if (std::string("print").compare(identifier) == 0) {
    std::cout << params.at(0).get_str() << "\n";
  }
  if (std::string("add").compare(identifier) == 0) {
    DotLiteral x = DotLiteral(params.at(0).get_int() + params.at(1).get_int());
    return x;
  }
  if (std::string("sub").compare(identifier) == 0) {
    return DotLiteral(params.at(0).get_int() - params.at(1).get_int());
  }
  if (std::string("mult").compare(identifier) == 0) {
    return DotLiteral(params.at(0).get_int() * params.at(1).get_int());
  }
  if (std::string("div").compare(identifier) == 0) {
    return DotLiteral(params.at(0).get_int() / params.at(1).get_int());
  }
  if(std::string("input").compare(identifier) == 0) {
    std::cout << params.at(0).get_str();
    std::string buffer;
    std::getline(std::cin, buffer);
    return DotLiteral(buffer);
  }
  return DotLiteral(0);
}
int DotToken::get_parsed_contents(){
  return parsed_contents;
}