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
#include "errors.h"

DotToken::DotToken() {
  // Default constructor does nothing
}

int DotToken::get_line() {
  return line;
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

bool DotToken::not_of_type(std::string type) {
  return token_type.compare(type) != 0;
}

DotLParen::DotLParen(int line) {
  this->line = line;
  token_type = "DotLParen";
  // DotLParen will always be a '('
  contents = "(";
}

DotRParen::DotRParen(int line) {
  this->line = line;
  token_type = "DotRParen";
  // DotRParen will always be a ')'
  contents = ")";
}

DotLBrace::DotLBrace(int line) {
  this->line = line;
  token_type = "DotLBrace";
  // DotLBrace will always be a '{'
  contents = "{";
}

DotRBrace::DotRBrace(int line) {
  this->line = line;
  token_type = "DotRBrace";
  // DotRBrace will always be a '}'
  contents = "}";
}

DotIDLiteralSign::DotIDLiteralSign(int line) {
  this->line = line;
  token_type = "DotIDLiteralSign";
  contents = "$";
}

DotComma::DotComma(int line) {
  this->line = line;
  token_type = "DotComma";
  // DotComma will always be a ','
  contents = ",";
}

DotInt::DotInt(int line, std::string content) {
  this->line = line;
  token_type = "DotInt";
  contents = content;
  parsed_contents = std::stoi(content);
}

DotString::DotString(int line, std::string content) {
  this->line = line;
  token_type = "DotString";
  contents = content;
}


DotIdentifier::DotIdentifier() {
  // Default constructor does nothing
}

DotIdentifier::DotIdentifier(int line, std::string _contents) {
  token_type = "DotIdentifier";
  contents = _contents;
}

DotStatement::DotStatement() {
  // Default constructor does nothing
}

DotStatement::DotStatement(ShimmerParam _expr, std::vector<ShimmerParam> _params) {
  expr = _expr;
  params = _params;
}

DotStatement::DotStatement(const DotStatement &statement) {
	expr = ShimmerParam(statement.expr);

	for (ShimmerParam i : statement.params) {
    ShimmerParam* sp = new ShimmerParam(i);
		params.push_back(*sp);
	}
}

std::vector<ShimmerParam> DotStatement::get_params() {
  return params;
}

ShimmerParam DotStatement::get_expr() {
  return std::forward<ShimmerParam>(expr);
}

void DotStatement::set_params(std::vector<ShimmerParam> param) {
  params = param;
}

void DotStatement::set_expr(ShimmerParam expr) {
  this->expr = expr;
}

LookupResult DotStatement::lookup_tables() {
  if (expr.get_param_type() != IDENTIFIER) {
    return LookupResult();
  }

  std::string name = expr.get_identifier_val().get_contents();

  if (name == "print") {
    for (auto p : get_params()) {
      std::cout << p.get_literal_val().get_str() << " ";
    }

    std::cout << "\n";

    return LookupResult(DotLiteral(-1, 0));
  }
  else if (name == "add") {
    int operand1 = get_params().at(0).literal_val->get_int();
    int operand2 = get_params().at(1).literal_val->get_int();

    // std::cout << "Performing " << std::to_string(operand1) << " + " << std::to_string(operand2) << ".\n";
    // std::cout << "The address of the first  operand is: ";
    // printf("%p.\n", (void*) &(get_params().at(0)));
    // std::cout << "The address of the second operand is: ";
    // printf("%p.\n", (void*) &(get_params().at(1)));
    // std::cout << "The address of the first  operand's literal_val is: ";
    // printf("%p.\n", (void*) (get_params().at(0).literal_val));
    // std::cout << "The address of the second operand's literal_val is: ";
    // printf("%p.\n", (void*) (get_params().at(1).literal_val));
    // std::cout << "The address of the address of the first  operand's literal_val is: ";
    // printf("%p.\n", (void*) &(get_params().at(0).literal_val));
    // std::cout << "The address of the address of the second operand's literal_val is: ";
    // printf("%p.\n", (void*) &(get_params().at(1).literal_val));
    // std::cout << "The address of the first  operand's int is: ";
    // printf("%p.\n", (void*) &operand1);
    // std::cout << "The address of the second operand's int is: ";
    // printf("%p.\n", (void*) &operand2);

    return LookupResult(DotLiteral(-1, operand1 + operand2));
  }
  else if (name == "sub") {
    int operand1 = get_params().at(0).literal_val->get_int();
    int operand2 = get_params().at(1).literal_val->get_int();
    return LookupResult(DotLiteral(-1, operand1 - operand2));
  }
  else if (name == "mul") {
    int operand1 = get_params().at(0).literal_val->get_int();
    int operand2 = get_params().at(1).literal_val->get_int();
    return LookupResult(DotLiteral(-1, operand1 * operand2));
  }
  else if (name == "div") {
    int operand1 = get_params().at(0).literal_val->get_int();
    int operand2 = get_params().at(1).literal_val->get_int();

    if (operand2 == 0) {
      throw_error("Division by zero is illegal", -1);
    }

    return LookupResult(DotLiteral(-1, operand1 / operand2));
  }

  return LookupResult();
}

DotLiteral DotStatement::eval(ShimmerScope* scope) {
  for (int i = 0; i < params.size(); i++) {
    std::cout << "On param #" << i << "\n";

    if (params.at(i).is_of_type(STATEMENT)) {
			DotStatement new_statement = DotStatement(params.at(i).get_statement_val());
      pretty_print(*this);
      // IMPORTANT - OVERWRITING HAPPENS ON NEXT STATEMENT
      // IMPORTANT - IMPORTANT - OVERWRITING DOES NOT HAPPEN ON NEXT STATEMENT
      DotLiteral foo = new_statement.eval(scope);
      // delete[] params.at(i).statement_val;
      params.at(i) = ShimmerParam(/*new_statement.eval(scope)*/ foo);
       
      std::cout << "Evaluated and got: " << params.at(i).get_literal_val().get_str() << "\n";
       
    }
    else if (params.at(i).is_of_type(IDENTIFIER)) {
      DotLiteral x = scope->get_variable(params.at(i).get_identifier_val().get_contents());
      params.at(i) = ShimmerParam(x);
    }
    // else if (params.at(i).is_of_type(LITERAL)) {
    //   std::cout << params.at(i).get_literal_val().get_str();
    // }
  }

  LookupResult res = lookup_tables();

  if (res.found) {
    return res.value;
  }
  else if (false) {
    /* TODO: Create function to lookup user defined functions and evaluate them */
  }
  else {
    throw_error("Function not found", -1);
  }

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

/* 
DotLiteral::DotLiteral(int line, ShimmerUnclosedFunc val) {
  type = TypeFunc;
  static ShimmerUnclosedFunc value = val;
  func_value = &value;
}
*/

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

/*
ShimmerUnclosedFunc DotLiteral::get_func() {
  return *func_value;
}
*/

DotIdentifier DotLiteral::get_id() {
  return id_value;
}

ShimmerParam::ShimmerParam() {
  // Default constructor does nothing
}

ShimmerParam::ShimmerParam(DotLiteral literal_value) {
  param_type = LITERAL;
  literal_val = new DotLiteral(literal_value);
}

ShimmerParam::ShimmerParam(DotStatement statement_value) {
  param_type = STATEMENT;
  statement_val = new DotStatement(statement_value);
}

ShimmerParam::ShimmerParam(DotIdentifier identifier_value) {
  param_type = IDENTIFIER;
  identifier_val = identifier_value;
}

ShimmerParam::ShimmerParam(ShimmerUnclosedFunc& func_value) {
  param_type = FUNCTION;
  func_val = new ShimmerUnclosedFunc(func_value);
}

// ShimmerParam::ShimmerParam(const ShimmerParam& param){
//   param_type = param.param_type;

//   if (param_type == LITERAL) {
// 		const DotLiteral newLiteral = *param.literal_val;
//     literal_val = new DotLiteral(newLiteral);
//   }
//   else if (param_type == STATEMENT) {
//     statement_val = param.statement_val;
//   }
//   else if (param_type == FUNCTION) {
//     func_val = param.func_val;
//   }
//   else {
//     identifier_val = param.identifier_val;
//   }
// }

ParamType ShimmerParam::get_param_type() {
  return param_type;
}

bool ShimmerParam::is_of_type(ParamType type) {
  return get_param_type() == type;
}

bool ShimmerParam::not_of_type(ParamType type) {
  return get_param_type() != type;
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
    return *current_scope.at(var_name);
  }
  else {
    if (upper_scope != nullptr) {
      return upper_scope->get_variable(var_name);
    }
    else {
      throw std::runtime_error("Undefined reference to variable " + var_name);
    }
  }
}

void ShimmerScope::set_variable(std::string var_name, DotLiteral val) {
  if (current_scope.find(var_name) != current_scope.end()) {
    *current_scope.at(var_name) = val;
  }
  else {
    upper_scope->set_variable(var_name, val);
  }
}

void ShimmerScope::declare_variable(std::string var_name, DotLiteral val) {
  *current_scope.at(var_name) = val;
}

ShimmerUnclosedFunc::ShimmerUnclosedFunc(std::vector<DotIdentifier> _params, DotTree _tree) {
  params = _params;
  tree = _tree;
}

ShimmerClosedFunc::ShimmerClosedFunc(ShimmerUnclosedFunc to_close, ShimmerScope* closed_scope) {
  params = to_close.params;
  tree = to_close.tree;
}
