#include <any>
#include <iostream>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <tuple>
#include <stdlib.h>

#include "ShimmerClasses.h"
#include "errors.h"
#include "eval.h"
#include "lexer.h"
#include "parser.h"
#include "text_effects.h"
#include "pretty_print.h"
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
double ShimmerToken::get_parsed_contents() {
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

/* Constructor for an number token */
ShimmerNumber::ShimmerNumber(int line, std::string content) {
  this->line = line;
  token_type = "ShimmerNumber";
  contents = content;
  parsed_contents = std::stod(content);
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
  
  // Misc

  if (func_name == "exit") {
    int status = (
      get_params().size() == 0 ? 0 :
      get_params().at(0).literal_val->get_num()
    );

    exit(status);

    return LookupResult(ShimmerLiteral(func_call_line, (double) status));
  }

  // IO

  else if (func_name == "print") {
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

    return LookupResult(ShimmerLiteral(-1, (double) param_count));
  }
  else if (func_name == "read") {
    if (get_params().size() == 0) {
      std::string val;
      std::getline(std::cin, val);

      return LookupResult(ShimmerLiteral(func_call_line, val));
    }
    else if (get_params().size() == 1) {
      ShimmerLiteral* fname = get_params().at(0).literal_val;

      std::ifstream file(fname->get_str());
      if (!file) throw_error(func_call_line, "Error opening file ", fname->get_str());

      std::stringstream buffer;
      buffer << file.rdbuf();
      return LookupResult(ShimmerLiteral(func_call_line, buffer.str()));
    }
    else {
      throw_error(
        func_call_line,
        "read() expects either 0 or 1 parameters, but recieved ",
        std::to_string(get_params().size())
      );
    }
  }
  else if (func_name == "write") {
    error_on_missing_params(
      func_call_line, 2,
      "write() expects at least 2 parameters, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* fname    = get_params().at(0).literal_val;
    ShimmerLiteral* contents = get_params().at(1).literal_val;

    std::ofstream file(fname->get_str());
    file << contents->get_str();
    file.close();

    return LookupResult(ShimmerLiteral(-1, (double) 0));
  }
  else if (func_name == "append") {
    error_on_missing_params(
      func_call_line, 2,
      "append() expects at least 2 parameters, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* fname    = get_params().at(0).literal_val;
    ShimmerLiteral* contents = get_params().at(1).literal_val;

    std::ofstream file(fname->get_str(), std::ios_base::app);
    file << contents->get_str();
    file.close();

    return LookupResult(ShimmerLiteral(-1, (double) 0));
  }

  // Arithmetics

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

  // Simple boolean logic

  else if (func_name == "and") {
    error_on_missing_params(
      func_call_line, 2,
      "and() expects at least 2 parameters, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* val1 = get_params().at(0).literal_val;
    ShimmerLiteral* val2 = get_params().at(0).literal_val;
    return LookupResult(ShimmerLiteral(func_call_line, val1->get_bool() && val2->get_bool()));
  }
  else if (func_name == "or") {
    error_on_missing_params(
      func_call_line, 2,
      "or() expects at least 2 parameters, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* val1 = get_params().at(0).literal_val;
    ShimmerLiteral* val2 = get_params().at(0).literal_val;
    return LookupResult(ShimmerLiteral(func_call_line, val1->get_bool() || val2->get_bool()));
  }
  else if (func_name == "not") {
    error_on_missing_params(
      func_call_line, 1,
      "not() expects at least 1 parameter, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* val1 = get_params().at(0).literal_val;
    return LookupResult(ShimmerLiteral(func_call_line, !val1->get_bool()));
  }

  // Comparisons

  else if (func_name == "lt") {
    error_on_missing_params(
      func_call_line, 2,
      "lt() expects at least 2 parameters, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* val1 = get_params().at(0).literal_val;
    ShimmerLiteral* val2 = get_params().at(1).literal_val;

    return LookupResult(ShimmerLiteral(func_call_line, val1->get_num() < val2->get_num()));
  }
  else if (func_name == "gt") {
    error_on_missing_params(
      func_call_line, 2,
      "gt() expects at least 2 parameters, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* val1 = get_params().at(0).literal_val;
    ShimmerLiteral* val2 = get_params().at(1).literal_val;

    return LookupResult(ShimmerLiteral(func_call_line, val1->get_num() > val2->get_num()));
  }
  else if (func_name == "eq") {
    error_on_missing_params(
      func_call_line, 2,
      "eq() expects at least 2 parameters, but recieved " + std::to_string(get_params().size())
    );

    bool equal = false;

    ShimmerLiteral* val1 = get_params().at(0).literal_val;
    ShimmerLiteral* val2 = get_params().at(1).literal_val;

    if (val1->get_type() != val2->get_type()) {
      return LookupResult(ShimmerLiteral(func_call_line, equal));
    }

    switch (val1->get_type()) {
      case TypeBool:   equal = val1->get_bool() == val2->get_bool();  break;
      case TypeString: equal = val1->get_str()  == val2->get_str();   break;
      case TypeNumber: equal = val1->get_num()  == val2->get_num();   break;
      //case TypeId:     equal = val1->get_id()   == val2->get_id();   break;
      //case TypeFunc:   equal = val1->get_func() == val2->get_func(); break;
    }

    return LookupResult(ShimmerLiteral(func_call_line, equal));
  }
  
  // Conversions

  else if (func_name == "num") {
    error_on_missing_params(
      func_call_line, 1,
      "num() expects at least 1 parameter, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* val = get_params().at(0).literal_val;
    return LookupResult(ShimmerLiteral(func_call_line, val->get_num()));
  }
  else if (func_name == "str") {
    error_on_missing_params(
      func_call_line, 1,
      "str() expects at least 1 parameter, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* val = get_params().at(0).literal_val;
    return LookupResult(ShimmerLiteral(func_call_line, val->get_str()));
  }
  else if (func_name == "bool") {
    error_on_missing_params(
      func_call_line, 2,
      "bool() expects at least 1 parameter, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* val = get_params().at(0).literal_val;
    return LookupResult(ShimmerLiteral(func_call_line, val->get_bool()));
  }  

  // String manipulation

  else if (func_name == "get_char") {
    error_on_missing_params(
      func_call_line, 2,
      "get_char() expects at least 2 parameters, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* string = get_params().at(0).literal_val;
    ShimmerLiteral* index  = get_params().at(1).literal_val;

    std::string str = std::string(1, string->get_str().at(index->get_num()));

    return LookupResult(ShimmerLiteral(func_call_line, str));
  }
  else if (func_name == "set_char") {
    error_on_missing_params(
      func_call_line, 3,
      "replace() expects at least 3 parameters, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* string = get_params().at(0).literal_val;
    ShimmerLiteral* index  = get_params().at(1).literal_val;
    ShimmerLiteral* char_  = get_params().at(2).literal_val;

    std::string str = string->get_str();
    str.at(index->get_num()) = char_->get_str().at(0);

    return LookupResult(ShimmerLiteral(func_call_line, str));
  }
  else if (func_name == "insert") {
    error_on_missing_params(
      func_call_line, 3,
      "insert() expects at least 3 parameters, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* string    = get_params().at(0).literal_val;
    ShimmerLiteral* index     = get_params().at(1).literal_val;
    ShimmerLiteral* substring = get_params().at(2).literal_val;

    std::string str = string->get_str();
    str.insert(index->get_num(), substring->get_str());

    return LookupResult(ShimmerLiteral(func_call_line, str));
  }
  else if (func_name == "remove") {
    error_on_missing_params(
      func_call_line, 3,
      "remove() expects at least 3 parameters, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* search = get_params().at(0).literal_val;
    ShimmerLiteral* index1 = get_params().at(1).literal_val;
    ShimmerLiteral* index2 = get_params().at(2).literal_val;

    std::string str = search->get_str();
    str.erase((int) index1->get_num(), (int) index2->get_num());

    return LookupResult(ShimmerLiteral(func_call_line, str));
  }
  else if (func_name == "concat") {
    error_on_missing_params(
      func_call_line, 2, 
      "concat() expects at least 2 parameters, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* str1 = get_params().at(0).literal_val;
    ShimmerLiteral* str2 = get_params().at(1).literal_val;

    return LookupResult(ShimmerLiteral(func_call_line, str1->get_str() + str2->get_str()));
  }
  else if (func_name == "slice") {
    error_on_missing_params(
      func_call_line, 2,
      "slice() expects at least 2 parameters, but recieved " + std::to_string(get_params().size())
    );

    ShimmerLiteral* str   = get_params().at(0).literal_val;
    ShimmerLiteral* start = get_params().at(1).literal_val;

    std::string result;
    
    if (get_params().size() >= 3) {
      ShimmerLiteral* stop = get_params().at(2).literal_val;

      result = str->get_str().substr(start->get_num(), stop->get_num() - start->get_num());
    }
    else {
      result = str->get_str().substr(start->get_num());
    }

    return LookupResult(ShimmerLiteral(func_call_line, result));
  }

  // Variables

  else if (func_name == "def") {
    ShimmerLiteral* var_name  = get_params().at(0).literal_val;
    ShimmerLiteral* var_value = get_params().at(1).literal_val;

    scope->declare_variable(var_name->get_id().get_contents(), *var_value);

#ifdef DEBUG
    pretty_print(scope);
#endif

    return LookupResult(ShimmerLiteral(func_call_line, (double) 0));
  }
  else if (func_name == "set") {
    ShimmerLiteral* var_name  = get_params().at(0).literal_val;
    ShimmerLiteral* var_value = get_params().at(1).literal_val;

    scope->set_variable(var_name->get_id().get_contents(), *var_value);
    return LookupResult(ShimmerLiteral(func_call_line, (double) 0));
  }
  else if (func_name == "get") {
    ShimmerLiteral* var_name = get_params().at(0).literal_val;

    ShimmerLiteral res = scope->get_variable(var_name->get_id().get_contents());
    return LookupResult(res);
  }
  
  // Control flow 

  else if (func_name == "if_else") {
    ShimmerLiteral* cond     = get_params().at(0).literal_val;
    ShimmerLiteral* if_true  = get_params().at(1).literal_val;
    ShimmerLiteral* if_false = get_params().at(2).literal_val;
    
    if (cond->get_bool() == true) {
      return LookupResult(*if_true);
    }
    else {
      return LookupResult(*if_false);
    }
  }

  // Libraries

  else if (func_name == "import") {
    std::string lib_name = get_params().at(0).literal_val->get_str();

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

      throw_error(-1, "Library "+ lib_name + " failed to execute");
    }

    // cons scope onto function (since it is a linked list)
    ShimmerLiteral* p1 = get_params().at(1).literal_val;
    ShimmerClosedFunc* func = new ShimmerClosedFunc(p1->get_func());
    func->closed_scope->cons_scope(scope);
    // return function
    return LookupResult(ShimmerLiteral(func_call_line, *func));
  }

  // DEBUG

  else if (func_name == "__debug__") {
    _throw_error(
      987, "FOO WAS CALLED WITH: %s, %s",
      get_params().at(0).get_literal_val().get_str().c_str(),
      get_params().at(0).get_literal_val().get_str().c_str()
    );

    // throw_error(
    //   345, "678"
    // );

    return LookupResult(ShimmerLiteral(func_call_line, (double) 0xDEADBEEF));
  }

  return LookupResult();
}

LookupResult ShimmerStatement::math_func_var(std::string name, int line, double start, BIFNextFunc next) {
  error_on_missing_params(
    line, 2,
    name + "() expects at least 2 parameters, but recieved " + std::to_string(get_params().size())
  );

  double res = start;

  for (auto p : get_params()) {
    res = next(line, res, p.literal_val);
  }

  return LookupResult(ShimmerLiteral(line, (double)res));
}

LookupResult ShimmerStatement::math_func_dya(std::string name, int line, std::vector<ShimmerExpr> params, BIFCalcFunc calc) {
  error_on_missing_params(
    line, 2,
    name + "() expects at least 2 parameters, but recieved " + std::to_string(get_params().size())
  );

  ShimmerLiteral* op1 = params.at(0).literal_val;
  ShimmerLiteral* op2 = params.at(1).literal_val;

  return LookupResult(ShimmerLiteral(line, (double)calc(line, op1, op2)));
}

BIFNextFunc BuiltinFuncs::add() {
  return [](int line, double res, ShimmerLiteral* param) -> double { return (double)res + param->get_num(); };
}

BIFCalcFunc BuiltinFuncs::sub() {
  return [](int line, ShimmerLiteral* op1, ShimmerLiteral* op2) -> double { return (double)op1->get_num() - op2->get_num(); };
}

BIFNextFunc BuiltinFuncs::mul() {
  return [](int line, double res, ShimmerLiteral* param) -> double { return (double)res * param->get_num(); };
}

BIFCalcFunc BuiltinFuncs::div() {
  return [](int line, ShimmerLiteral* op1, ShimmerLiteral* op2) -> double {
    if (op2->get_num() == 0) throw_error(line, "Division by 0 is illegal");
    return (double)op1->get_num() / op2->get_num();
  };
}

// Evaluate a single thing
ShimmerLiteral ShimmerStatement::eval(ShimmerScope* scope) {
  for (std::vector<ShimmerExpr>::size_type i = 0; i < params.size(); i++) {
#ifdef DEBUG
    std::cout << "On param #" << i << "\n";
#endif
    if (params.at(i).is_of_type(STATEMENT)) {
#ifdef DEBUG
      pretty_print(*this);
#endif
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
      ShimmerLiteral lres =  expr.get_statement_val().eval(scope);
      if (lres.type != TypeFunc) {
        throw_error(-1, "Not a function.");
      }
      ShimmerClosedFunc res = lres.get_func();
      return eval_tree(res.tree, res.closed_scope, res.params, params);
    }
    else if (expr.is_of_type(IDENTIFIER)) {
      ShimmerLiteral lres = scope->get_variable(expr.get_identifier_val().get_contents());
      if (lres.type != TypeFunc) {
        throw_error(-1, "Not a function.");
      }
      ShimmerClosedFunc res = lres.get_func();
      return eval_tree(res.tree, res.closed_scope, res.params, params);
    }
    else {
      throw_error(-1, "Function not found");
    }
  }
  
#warning try to fix this -1 line
  return ShimmerLiteral(-1, (double) 0); // line is -1 because we can't figure out the line number
}

void ShimmerStatement::error_on_wrong_num_params(int line, unsigned int min, unsigned int max, std::string msg) {
  error_on_missing_params(line, min, msg);
  error_on_extra_params(line, max, msg);
}

void ShimmerStatement::error_on_missing_params(int line, unsigned int min, std::string msg) {
#ifdef DEBUG
  pretty_print(*this);

  std::cout << "\n";
#endif

  if (params.size() < min) {
    throw_error(line, msg);
  }
}

void ShimmerStatement::error_on_extra_params(int line, unsigned int max, std::string msg) {
#ifdef DEBUG
  pretty_print(*this);

  std::cout << "\n";
#endif
  if (params.size() > max) {
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

ShimmerLiteral::ShimmerLiteral(int line, double val) {
  type = TypeNumber;
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

double ShimmerLiteral::get_num() {
	 if (type == TypeString) {
    try {
      return std::stod(str_value);
    }
    catch (const std::invalid_argument& ia) {
      throw_error(-1, "String \""+ str_value + "\" can't be converted to a number");
    }
  }
  else if (type == TypeBool) return bool_value ? 0 : 1;
  else return int_value;
}

bool ShimmerLiteral::get_bool() {
  if (type == TypeNumber) {
    return int_value != 0;
  }
  else if (type == TypeBool) {
    return bool_value;
  }
  else {
    return true;
  }
}

std::string removeZero(std::string str) {
  // Count trailing zeros
  int i = str.length() - 1;

  while (str[i] == '0') i--;

  if (str[i] == '.') i--;

  // The erase function removes i characters
  // from given index (0 here)
  str.erase(i + 1, str.length());

  return str;
}

std::string ShimmerLiteral::get_str() {
  if (type == TypeNumber) {
    return removeZero(std::to_string(int_value));
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
#warning try to fix this -1 line
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
