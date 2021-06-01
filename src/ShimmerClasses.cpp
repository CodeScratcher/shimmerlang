#include <any>
#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <unordered_map>

#include "ShimmerClasses.h"

#include "errors.h"
#include "eval.h"
// #include "parser.h"
#include "tree_print.h"
#include "utility.h"

/* Default constructor for a generic token, to be overridden */
ShimmerToken::ShimmerToken() {
  // Default constructor does nothing
}

/* Returns the line of a token. */
int ShimmerToken::get_line() {
  return line;
}

/* Returns the literal text of a token. */
std::string ShimmerToken::get_contents() {
  return contents;
}

/* Returns the actual int from an int token. */
// THIS FUNCTION AND PARSED_CONTENTS SHOULD BE DEFINED BY SHIMMERINT, MAYBE???
int ShimmerToken::get_parsed_contents() {
  return parsed_contents;
}

/* Returns the type of a token */
std::string ShimmerToken::get_token_type() {
  return token_type;
}

/* Return true if the token's type and the passed type are the same */
bool ShimmerToken::is_of_type(std::string type) {
  return token_type.compare(type) == 0;
}

/* Returns true if the token's type and the passed type are different */
bool ShimmerToken::not_of_type(std::string type) {
  return token_type.compare(type) != 0;
}

/* Constructor for the "(" token */
ShimmerLParen::ShimmerLParen(int line) {
  this->line = line;
  token_type = "ShimmerLParen";
  // ShimmerLParen will always be a '('
  contents = "(";
}

/* Constructor for the ")" token */
ShimmerRParen::ShimmerRParen(int line) {
  this->line = line;
  token_type = "ShimmerRParen";
  // ShimmerRParen will always be a ')'
  contents = ")";
}

/* Constructor for the "{" token */
ShimmerLBrace::ShimmerLBrace(int line) {
  this->line = line;
  token_type = "ShimmerLBrace";
  // ShimmerLBrace will always be a '{'
  contents = "{";
}

/* Constructor for the "}" token */
ShimmerRBrace::ShimmerRBrace(int line) {
  this->line = line;
  token_type = "ShimmerRBrace";
  // ShimmerRBrace will always be a '}'
  contents = "}";
}

/* Constructor for the "$" token */
ShimmerIDLiteralSign::ShimmerIDLiteralSign(int line) {
  this->line = line;
  token_type = "ShimmerIDLiteralSign";
  // ShimmerIDLiteralSign will always be a '$'
  contents = "$";
}

/* Constructor for the "," token */
ShimmerComma::ShimmerComma(int line) {
  this->line = line;
  token_type = "ShimmerComma";
  // ShimmerComma will always be a ','
  contents = ",";
}

/* Constructor for an integer token */
ShimmerInt::ShimmerInt(int line, std::string content) {
  this->line = line;
  token_type = "ShimmerInt";
  contents = content;
  parsed_contents = std::stoi(content);
}

/* Constructor for a string token */
ShimmerString::ShimmerString(int line, std::string content) {
  this->line = line;
  token_type = "ShimmerString";
  contents = content;
}

/* Default constructor for an identifier token */
ShimmerIdentifier::ShimmerIdentifier() {
  // Default constructor does nothing
}

/* Constructor for an identifier token */
ShimmerIdentifier::ShimmerIdentifier(int line, std::string _contents) {
  this->line = line;
  token_type = "ShimmerIdentifier";
  contents = _contents;
}

/* Default constructor for a statement */
ShimmerStatement::ShimmerStatement() {
  // Default constructor does nothing
}

/* Constructor for a statement */
ShimmerStatement::ShimmerStatement(ShimmerExpr _expr, std::vector<ShimmerExpr> _params) {
  expr = _expr;
  params = _params;
}

/* Copy constructor for a statement */
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

LookupResult ShimmerStatement::lookup_tables(ShimmerScope* scope) {
  if (expr.get_param_type() != IDENTIFIER) {
    return LookupResult();
  }

  std::string func_name = expr.get_identifier_val().get_contents();
  int func_call_line = expr.get_identifier_val().get_line();

  if (func_name == "print") {
    int param_count = 0;
    bool first_param = true;

    for (auto p : get_params()) {
      if (!first_param) {
        std::cout << " ";
      }
      else {
        first_param = false;
      }

      std::cout << p.get_literal_val().get_str();
      param_count++;
    }

    return LookupResult(ShimmerLiteral(-1, param_count));
  }
  else if (func_name == "read") {
    if (get_params().size() == 0) {
      std::string line;
      std::getline(std::cin, line);

      return LookupResult(ShimmerLiteral(-1, line));
    }
    else if (get_params().size() == 1) {
      throw_error(func_call_line, "Sorry, reading from files is not implemeted yet!");
    }
    else {
      throw_error(
        func_call_line,
        "read() expects either 0 or 1 parameters, but recieved ",
        std::to_string(get_params().size())
      );
    }
  }
  else if (func_name == "add") {
    error_on_missing_params(
      func_call_line, 2,
      "add() expects at least 2 parameters, but recieved TODO"
    );

    int operand1 = get_params().at(0).literal_val->get_int();
    int operand2 = get_params().at(1).literal_val->get_int();

    return LookupResult(ShimmerLiteral(-1, operand1 + operand2));
  }
  else if (func_name == "sub") {
    int operand1 = get_params().at(0).literal_val->get_int();
    int operand2 = get_params().at(1).literal_val->get_int();
    return LookupResult(ShimmerLiteral(-1, operand1 - operand2));
  }
  else if (func_name == "mul") {
    int operand1 = get_params().at(0).literal_val->get_int();
    int operand2 = get_params().at(1).literal_val->get_int();
    return LookupResult(ShimmerLiteral(-1, operand1 * operand2));
  }
  else if (func_name == "div") {
    int operand1 = get_params().at(0).literal_val->get_int();
    int operand2 = get_params().at(1).literal_val->get_int();

    if (operand2 == 0) {
      throw_error(func_call_line, "Division by zero is illegal");
    }

    return LookupResult(ShimmerLiteral(-1, operand1 / operand2));
  }
  else if (func_name == "def") {
    ShimmerLiteral* p0 = get_params().at(0).literal_val;
    ShimmerLiteral* p1 = get_params().at(1).literal_val;

    scope->declare_variable(p0->get_id().get_contents(), *p1);

    std::cout << "== Begin pretty print scope ==\n";
    pretty_print(scope);
    std::cout << "== End pretty print scope ==\n";

    return LookupResult(ShimmerLiteral(-1, 0));
  }
  else if (func_name == "set") {
    scope->set_variable(get_params().at(0).literal_val->get_id().get_contents(), *get_params().at(1).literal_val);
    return LookupResult(ShimmerLiteral(-1, 0));
  }
  else if (func_name == "get") {
    ShimmerLiteral res = scope->get_variable(get_params().at(0).literal_val->get_id().get_contents());
    return LookupResult(res);
  }
  else if (func_name == "foo") {
    _throw_error(
      987, "FOO WAS CALLED WITH: %s, %s",
      get_params().at(0).get_literal_val().get_str().c_str(),
      get_params().at(0).get_literal_val().get_str().c_str()
    );

    // throw_error(
    //   345, "678"
    // );

    return LookupResult(ShimmerLiteral(-1, 0xDEADBEEF));
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

  LookupResult res = lookup_tables(scope);

  if (res.found) {
    return res.value;
  }
  else if (false) {
    /* TODO: Create function to lookup user defined functions and evaluate them */
  }
  else {
    throw_error(-1, "Function not found");
  }

  return ShimmerLiteral(-1, 0); // line is -1 because we can't figure out the line number
}

void ShimmerStatement::error_on_wrong_num_params(int line, int min, int max, std::string msg) {
  error_on_missing_params(line, min, msg);
  error_on_extra_params(line, max, msg);
}

void ShimmerStatement::error_on_missing_params(int line, int min, std::string msg) {
  pretty_print(*this);
  std::cout << "\n";

  if (params.size() < min) {
    //throw std::runtime_error(std::to_string(line) + std::string(":\n\t") + msg);
    throw_error(line, msg);
  }
}

void ShimmerStatement::error_on_extra_params(int line, int max, std::string msg) {
  pretty_print(*this);
  std::cout << "\n";

  if (params.size() > max) {
    //throw std::runtime_error(std::to_string(line) + std::string(":\n\t") + msg);
    throw_error(line, msg);
  }
}

// template<typename... Types> void __error_on_wrong_num_params(int line, int min, int max, Types... args) {

// }

// template<typename... Types> void __error_on_missing_params(int line, int min, Types... args) {

// }

// template<typename... Types> void __error_on_extra_params(int line, int max, Types... args) {

// }

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
      throw_error(-1, "Undefined reference to variable ", var_name);
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
  ShimmerLiteral* nval = new ShimmerLiteral(val);
  current_scope[var_name] = nval;
}

ShimmerUnclosedFunc::ShimmerUnclosedFunc(std::vector<ShimmerIdentifier> _params, ShimmerTree _tree) {
  params = _params;
  tree = _tree;
}

ShimmerClosedFunc::ShimmerClosedFunc(ShimmerUnclosedFunc to_close, ShimmerScope* closed_scope) {
  params = to_close.params;
  tree = to_close.tree;
}
