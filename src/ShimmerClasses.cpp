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

ShimmerToken::ShimmerToken() {
  // Default constructor does nothing
}

int ShimmerToken::get_line() {
  return line;
}

std::string ShimmerToken::get_contents() {
  return contents;
}

int ShimmerToken::get_parsed_contents() {
  return parsed_contents;
}

std::string ShimmerToken::get_token_type() {
  return token_type;
}

bool ShimmerToken::is_of_type(std::string type) {
  return token_type.compare(type) == 0;
}

bool ShimmerToken::not_of_type(std::string type) {
  return token_type.compare(type) != 0;
}

ShimmerLParen::ShimmerLParen(int line) {
  this->line = line;
  token_type = "ShimmerLParen";
  // ShimmerLParen will always be a '('
  contents = "(";
}

ShimmerRParen::ShimmerRParen(int line) {
  this->line = line;
  token_type = "ShimmerRParen";
  // ShimmerRParen will always be a ')'
  contents = ")";
}

ShimmerLBrace::ShimmerLBrace(int line) {
  this->line = line;
  token_type = "ShimmerLBrace";
  // ShimmerLBrace will always be a '{'
  contents = "{";
}

ShimmerRBrace::ShimmerRBrace(int line) {
  this->line = line;
  token_type = "ShimmerRBrace";
  // ShimmerRBrace will always be a '}'
  contents = "}";
}

ShimmerIDLiteralSign::ShimmerIDLiteralSign(int line) {
  this->line = line;
  token_type = "ShimmerIDLiteralSign";
  contents = "$";
}

ShimmerComma::ShimmerComma(int line) {
  this->line = line;
  token_type = "ShimmerComma";
  // ShimmerComma will always be a ','
  contents = ",";
}

ShimmerInt::ShimmerInt(int line, std::string content) {
  this->line = line;
  token_type = "ShimmerInt";
  contents = content;
  parsed_contents = std::stoi(content);
}

ShimmerString::ShimmerString(int line, std::string content) {
  this->line = line;
  token_type = "ShimmerString";
  contents = content;
}


ShimmerIdentifier::ShimmerIdentifier() {
  // Default constructor does nothing
}

ShimmerIdentifier::ShimmerIdentifier(int line, std::string _contents) {
  token_type = "ShimmerIdentifier";
  contents = _contents;
}

ShimmerStatement::ShimmerStatement() {
  // Default constructor does nothing
}

ShimmerStatement::ShimmerStatement(ShimmerExpr _expr, std::vector<ShimmerExpr> _params) {
  expr = _expr;
  params = _params;
}

ShimmerStatement::ShimmerStatement(const ShimmerStatement &statement) {
	expr = ShimmerExpr(statement.expr);

	for (ShimmerExpr i : statement.params) {
    ShimmerExpr* sp = new ShimmerExpr(i);
		params.push_back(*sp);
	}
}

std::vector<ShimmerExpr> ShimmerStatement::get_params() {
  return params;
}

ShimmerExpr ShimmerStatement::get_expr() {
  return std::forward<ShimmerExpr>(expr);
}

void ShimmerStatement::set_params(std::vector<ShimmerExpr> param) {
  params = param;
}

void ShimmerStatement::set_expr(ShimmerExpr expr) {
  this->expr = expr;
}

LookupResult ShimmerStatement::lookup_tables() {
  if (expr.get_param_type() != IDENTIFIER) {
    return LookupResult();
  }

  std::string name = expr.get_identifier_val().get_contents();

  if (name == "print") {
    for (auto p : get_params()) {
      std::cout << p.get_literal_val().get_str() << " ";
    }

    std::cout << "\n";

    return LookupResult(ShimmerLiteral(-1, 0));
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

    return LookupResult(ShimmerLiteral(-1, operand1 + operand2));
  }
  else if (name == "sub") {
    int operand1 = get_params().at(0).literal_val->get_int();
    int operand2 = get_params().at(1).literal_val->get_int();
    return LookupResult(ShimmerLiteral(-1, operand1 - operand2));
  }
  else if (name == "mul") {
    int operand1 = get_params().at(0).literal_val->get_int();
    int operand2 = get_params().at(1).literal_val->get_int();
    return LookupResult(ShimmerLiteral(-1, operand1 * operand2));
  }
  else if (name == "div") {
    int operand1 = get_params().at(0).literal_val->get_int();
    int operand2 = get_params().at(1).literal_val->get_int();

    if (operand2 == 0) {
      throw_error("Division by zero is illegal", -1);
    }

    return LookupResult(ShimmerLiteral(-1, operand1 / operand2));
  }

  return LookupResult();
}
// Evaluate a single thing
ShimmerLiteral ShimmerStatement::eval(ShimmerScope* scope) {
  for (int i = 0; i < params.size(); i++) {
    std::cout << "On param #" << i << "\n";

    if (params.at(i).is_of_type(STATEMENT)) {
      pretty_print(*this);
			ShimmerStatement new_statement = ShimmerStatement(params.at(i).get_statement_val());
      params.at(i) = ShimmerExpr(new_statement.eval(scope));
    }
    else if (params.at(i).is_of_type(IDENTIFIER)) {
      ShimmerLiteral x = scope->get_variable(params.at(i).get_identifier_val().get_contents());
      params.at(i) = ShimmerExpr(x);
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

  return ShimmerLiteral(-1, 0); // line is -1 because we can't figure out the line number
}

void ShimmerStatement::error_on_missing_params(int line, int min, std::string msg) {
  //print_statement_info(*this);
  pretty_print(*this);
  std::cout << "\n";
  if (params.size() < min) {
    throw std::runtime_error(std::to_string(line) + std::string(":\n\t") + msg);
  }
}

void ShimmerStatement::error_on_extra_params(int line, int max, std::string msg) {
  pretty_print(*this);
  std::cout << "\n";

  if (params.size() > max) {
    throw std::runtime_error(std::to_string(line) + std::string(":\n\t") + msg);
  }
}

ShimmerTree::ShimmerTree() {
  // Default constructor does nothing
}

ShimmerTree::ShimmerTree(std::vector<ShimmerStatement> statements) {
  tree = statements;
}

std::vector<ShimmerStatement> ShimmerTree::get_tree() {
  return tree;
}

ShimmerLiteral::ShimmerLiteral() {
  // Default constructor does nothing
}

ShimmerLiteral::ShimmerLiteral(int line, int val) {
  type = TypeInt;
  int_value = val;
}

ShimmerLiteral::ShimmerLiteral(int line, std::string val) {
  type = TypeString;
  str_value = val;
}

/* 
ShimmerLiteral::ShimmerLiteral(int line, ShimmerUnclosedFunc val) {
  type = TypeFunc;
  static ShimmerUnclosedFunc value = val;
  func_value = &value;
}
*/

ShimmerLiteral::ShimmerLiteral(int line, ShimmerIdentifier val) {
  type = TypeId;
  id_value = val;
}

int ShimmerLiteral::get_type() {
  return type;
}

int ShimmerLiteral::get_int() {
	 if (type == TypeString) {
    return std::stoi(str_value);
  }
  else return int_value;
}

bool ShimmerLiteral::get_bool() {
  return bool_value;
}

std::string ShimmerLiteral::get_str() {
  if (type == TypeInt) {
    return std::to_string(int_value);
  }
  else return str_value;
}

/*
ShimmerUnclosedFunc ShimmerLiteral::get_func() {
  return *func_value;
}
*/

ShimmerIdentifier ShimmerLiteral::get_id() {
  return id_value;
}

ShimmerExpr::ShimmerExpr() {
  // Default constructor does nothing
}

ShimmerExpr::ShimmerExpr(ShimmerLiteral literal_value) {
  param_type = LITERAL;
  literal_val = new ShimmerLiteral(literal_value);
}

ShimmerExpr::ShimmerExpr(ShimmerStatement statement_value) {
  param_type = STATEMENT;
  statement_val = new ShimmerStatement(statement_value);
}

ShimmerExpr::ShimmerExpr(ShimmerIdentifier identifier_value) {
  param_type = IDENTIFIER;
  identifier_val = identifier_value;
}

ShimmerExpr::ShimmerExpr(ShimmerUnclosedFunc& func_value) {
  param_type = FUNCTION;
  func_val = new ShimmerUnclosedFunc(func_value);
}

// ShimmerExpr::ShimmerExpr(const ShimmerExpr& param){
//   param_type = param.param_type;

//   if (param_type == LITERAL) {
// 		const ShimmerLiteral newLiteral = *param.literal_val;
//     literal_val = new ShimmerLiteral(newLiteral);
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

ParamType ShimmerExpr::get_param_type() {
  return param_type;
}

bool ShimmerExpr::is_of_type(ParamType type) {
  return get_param_type() == type;
}

bool ShimmerExpr::not_of_type(ParamType type) {
  return get_param_type() != type;
}

ShimmerLiteral ShimmerExpr::get_literal_val() {
  return *literal_val;
}

ShimmerStatement ShimmerExpr::get_statement_val() {
  return *statement_val;
}

ShimmerIdentifier ShimmerExpr::get_identifier_val() {
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

ShimmerLiteral ShimmerScope::get_variable(std::string var_name) {
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

void ShimmerScope::set_variable(std::string var_name, ShimmerLiteral val) {
  if (current_scope.find(var_name) != current_scope.end()) {
    *current_scope.at(var_name) = val;
  }
  else {
    upper_scope->set_variable(var_name, val);
  }
}

void ShimmerScope::declare_variable(std::string var_name, ShimmerLiteral val) {
  *current_scope.at(var_name) = val;
}

ShimmerUnclosedFunc::ShimmerUnclosedFunc(std::vector<ShimmerIdentifier> _params, ShimmerTree _tree) {
  params = _params;
  tree = _tree;
}

ShimmerClosedFunc::ShimmerClosedFunc(ShimmerUnclosedFunc to_close, ShimmerScope* closed_scope) {
  params = to_close.params;
  tree = to_close.tree;
}
