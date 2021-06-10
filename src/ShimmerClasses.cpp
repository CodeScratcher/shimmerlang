#include <any>
#include <iostream>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <tuple>

#include "ShimmerClasses.h"
#include "errors.h"
#include "eval.h"
#include "lexer.h"
#include "parser.h"
#include "text_effects.h"
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
      std::string val;
      std::getline(std::cin, val);

      return LookupResult(ShimmerLiteral(func_call_line, val));
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
    return math_func_var(func_name, func_call_line, 0, BuiltinFuncs::add());
  }
  else if (func_name == "sub") {
    return math_func_dya(func_name, func_call_line, get_params(), BuiltinFuncs::sub());
  }
  else if (func_name == "mul") {
    return math_func_var(func_name, func_call_line, 1, BuiltinFuncs::mul());
  }
  else if (func_name == "div") {
    return math_func_dya(func_name, func_call_line, get_params(), BuiltinFuncs::div());
  }
  else if (func_name == "and") {
    ShimmerLiteral* p1 = get_params().at(0).literal_val;
    ShimmerLiteral* p2 = get_params().at(0).literal_val;
    return LookupResult(ShimmerLiteral(func_call_line, p1->get_bool() && p2->get_bool()));
  }
  else if (func_name == "or") {
    ShimmerLiteral* p1 = get_params().at(0).literal_val;
    ShimmerLiteral* p2 = get_params().at(0).literal_val;
    return LookupResult(ShimmerLiteral(func_call_line, p1->get_bool() || p2->get_bool()));
  }
  else if (func_name == "not") {
    ShimmerLiteral* p1 = get_params().at(0).literal_val;
    return LookupResult(ShimmerLiteral(func_call_line, !p1->get_bool()));
  }
  else if (func_name == "xor") {
    ShimmerLiteral* p1 = get_params().at(0).literal_val;
    ShimmerLiteral* p2 = get_params().at(0).literal_val;
    return LookupResult(ShimmerLiteral(func_call_line, p1->get_bool() ^ p2->get_bool()));
  }
  else if (func_name == "lt") {
    ShimmerLiteral* p1 = get_params().at(0).literal_val;
    ShimmerLiteral* p2 = get_params().at(1).literal_val;
    return LookupResult(ShimmerLiteral(func_call_line, p1->get_int() < p2->get_int()));
  }
  else if (func_name == "gt") {
    ShimmerLiteral* p1 = get_params().at(0).literal_val;
    ShimmerLiteral* p2 = get_params().at(1).literal_val;
    return LookupResult(ShimmerLiteral(func_call_line, p1->get_int() > p2->get_int()));
  }
  else if (func_name == "eq") {
    bool equal = false;
    ShimmerLiteral* p1 = get_params().at(0).literal_val;
    ShimmerLiteral* p2 = get_params().at(0).literal_val;

    if (p1->get_type() != p2->get_type()) {
      return LookupResult(ShimmerLiteral(func_call_line, equal));
    }

    switch(p1->get_type()) {
      case TypeBool:   equal = p1->get_bool() == p2->get_bool(); break;
      case TypeString: equal = p1->get_str() == p2->get_str();   break;
      case TypeInt:    equal = p1->get_int() == p2->get_int();   break;
      //case TypeId:     equal = p1->get_id() == p2->get_id();     break;
      //case TypeFunc:   equal = p1->get_func() == p2->get_func(); break;
    }

    return LookupResult(ShimmerLiteral(func_call_line, equal));
  }
  else if (func_name == "def") {
    ShimmerLiteral* p0 = get_params().at(0).literal_val;
    ShimmerLiteral* p1 = get_params().at(1).literal_val;

    scope->declare_variable(p0->get_id().get_contents(), *p1);
    pretty_print(scope);
    return LookupResult(ShimmerLiteral(func_call_line, 0));
  }
  else if (func_name == "set") {
    ShimmerLiteral* p0 = get_params().at(0).literal_val;
    ShimmerLiteral* p1 = get_params().at(1).literal_val;

    scope->set_variable(p0->get_id().get_contents(), *p1);
    return LookupResult(ShimmerLiteral(func_call_line, 0));
  }
  else if (func_name == "get") {
    ShimmerLiteral* p0 = get_params().at(0).literal_val;

    ShimmerLiteral res = scope->get_variable(p0->get_id().get_contents());
    return LookupResult(res);
  }
  else if (func_name == "if") {
    ShimmerLiteral* p0 = get_params().at(0).literal_val;
    ShimmerLiteral* p1 = get_params().at(1).literal_val;
    ShimmerLiteral* p2 = get_params().at(2).literal_val;
    
    if (p0->get_bool() == true) {
      return LookupResult(*p1);
    }
    else {
      return LookupResult(*p2);
    }
  }
  else if (func_name == "import") {
    // steps:
    // open library
    ShimmerLiteral* p0 = get_params().at(0).literal_val;
    std::string lib_name = p0->get_str();

    std::ifstream file;
    file.open(lib_name + ".shmr");

    if (!file) {
      file.open("/usr/lib/shimmer/" + lib_name + ".shmr");
      if (!file) {
        file.open("~/.shimmer/libs/" + lib_name + ".shmr");
        if (!file) {
          throw_error(-1, "Error opening library");
        }
      }
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string data = buffer.str();

    // evaluate library, getting scope
    ShimmerScope* scope;
    try {
      Parser parser(lex(data));
      ShimmerTree parsed = parser.parse();
      scope = std::get<1>(eval_and_get_scope(parsed));
    }
    catch (std::runtime_error& err) {
      std::cout << \
        tc::red << \
        "In file " << \
        lib_name << \
        ".shmr on line " << \
        err.what() << \
        tc::reset << \
        "\n\n";


      throw_error(-1, "Library "+ p0->get_str() + " failed to execute");
    }
    // cons scope onto function (since it is a linked list)
    ShimmerLiteral* p1 = get_params().at(1).literal_val;
    ShimmerClosedFunc* func = new ShimmerClosedFunc(p1->get_func());
    func->closed_scope->cons_scope(scope);
    // return function
    return LookupResult(ShimmerLiteral(func_call_line, *func));
  }
  else if (func_name == "__debug__") {
    _throw_error(
      987, "FOO WAS CALLED WITH: %s, %s",
      get_params().at(0).get_literal_val().get_str().c_str(),
      get_params().at(0).get_literal_val().get_str().c_str()
    );

    // throw_error(
    //   345, "678"
    // );

    return LookupResult(ShimmerLiteral(func_call_line, (int) 0xDEADBEEF));
  }

  return LookupResult();
}

LookupResult ShimmerStatement::math_func_var(std::string name, int line, int start, BIFNextFunc next) {
  error_on_missing_params(
    line, 2,
    name + "() expects at least 2 parameters, but recieved " + std::to_string(get_params().size())
  );

  int res = start;

  for (auto p : get_params()) {
    res = next(line, res, p.literal_val);
  }

  return LookupResult(ShimmerLiteral(line, res));
}

LookupResult ShimmerStatement::math_func_dya(std::string name, int line, std::vector<ShimmerExpr> params, BIFCalcFunc calc) {
  ShimmerLiteral* op1 = params.at(0).literal_val;
  ShimmerLiteral* op2 = params.at(1).literal_val;

  return LookupResult(ShimmerLiteral(line, calc(line, op1, op2)));
}

BIFNextFunc BuiltinFuncs::add() {
  return [](int line, int res, ShimmerLiteral* param) -> int { return res + param->get_int(); };
}

BIFCalcFunc BuiltinFuncs::sub() {
  return [](int line, ShimmerLiteral* op1, ShimmerLiteral* op2) -> int { return op1->get_int() - op2->get_int(); };
}

BIFNextFunc BuiltinFuncs::mul() {
  return [](int line, int res, ShimmerLiteral* param) -> int { return res * param->get_int(); };
}

BIFCalcFunc BuiltinFuncs::div() {
  return [](int line, ShimmerLiteral* op1, ShimmerLiteral* op2) -> int {
    if (op2->get_int() == 0) throw_error(line, "Division by 0 is illegal");
    return op1->get_int() / op2->get_int();
  };
}

// Evaluate a single thing
ShimmerLiteral ShimmerStatement::eval(ShimmerScope* scope) {
  for (int i = 0; i < params.size(); i++) {
    std::cout << "On param #" << i << "\n";

    if (params.at(i).is_of_type(STATEMENT)) {
      pretty_print(*this);
			ShimmerStatement new_statement = ShimmerStatement(params.at(i).get_statement_val());
      ShimmerExpr res = ShimmerExpr(new_statement.eval(scope));
      params.at(i) = res;
    }
    else if (params.at(i).is_of_type(IDENTIFIER)) {
      ShimmerLiteral x = scope->get_variable(params.at(i).get_identifier_val().get_contents());
      params.at(i) = ShimmerExpr(x);
    }
    else if (params.at(i).is_of_type(FUNCTION)) {
      
      ShimmerLiteral x = ShimmerLiteral(-1, ShimmerClosedFunc(params.at(i).get_func_val(), scope));
      printf("Address: %p\n", (void*)params.at(i).func_val);
      params.at(i) = ShimmerExpr(x);
    }
    // else if (params.at(i).is_of_type(LITERAL)) {
    //    std::cout << params.at(i).get_literal_val().get_str();
    // }
  }

  LookupResult res = lookup_tables(scope);

  if (res.found) {
    return res.value;
  }
  else {
    if (expr.is_of_type(FUNCTION)) {
      return eval_tree(expr.get_func_val().tree, scope, expr.get_func_val().params, params);
    }
    else if (expr.is_of_type(STATEMENT)) {
      ShimmerClosedFunc res = expr.get_statement_val().eval(scope).get_func();
      return eval_tree(res.tree, res.closed_scope, res.params, params);
    }
    else if (expr.is_of_type(IDENTIFIER)) {
      ShimmerClosedFunc res = scope->get_variable(expr.get_identifier_val().get_contents()).get_func();
      return eval_tree(res.tree, res.closed_scope, res.params, params);
    }
    else {
      throw_error(-1, "Function not found");
    }
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

ShimmerTree::ShimmerTree(std::vector<ShimmerExpr> statements) {
  tree = statements;
}

std::vector<ShimmerExpr> ShimmerTree::get_tree() {
  return tree; /**** We've been trying to reach you about your car's extended warranty ****/
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

ShimmerLiteral::ShimmerLiteral(int line, ShimmerIdentifier val) {
  type = TypeId;
  id_value = val;
}


ShimmerLiteral::ShimmerLiteral(int line, ShimmerClosedFunc val) {
  type = TypeFunc;
  //static ShimmerClosedFunc value = val;
  func_value = val;
}

ShimmerLiteral::ShimmerLiteral(int line, bool val) {
  type = TypeBool;
  bool_value = val;
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
  else if (type == TypeBool) return bool_value ? "true" : "false";
  else return str_value;
}


ShimmerClosedFunc ShimmerLiteral::get_func() {
  return func_value;
}


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
ShimmerUnclosedFunc ShimmerExpr::get_func_val() {
  return *func_val;
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
void ShimmerScope::cons_scope(ShimmerScope* _upper_scope) {
  if (upper_scope != nullptr) {
    upper_scope->cons_scope(_upper_scope);
  }
  else {
    upper_scope = _upper_scope;
  }
}
ShimmerUnclosedFunc::ShimmerUnclosedFunc(std::vector<ShimmerIdentifier> _params, ShimmerTree _tree) {
  params = _params;
  tree = _tree;
}

ShimmerClosedFunc::ShimmerClosedFunc(ShimmerUnclosedFunc to_close, ShimmerScope* _closed_scope) {
  params = to_close.params;
  tree = to_close.tree;
  closed_scope = _closed_scope;
}
