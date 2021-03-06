#ifndef SHIMMER_CLASSES_H
#define SHIMMER_CLASSES_H

// #define DEBUG

/* TODO: make data structures into structs and remove accessors and mutators */

#include <any>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

enum ShimmerTypes {
  TypeString,
  TypeNumber,
  TypeBool,
  TypeFunc,
  TypeId
};

class ShimmerIdentifier;
class ShimmerStatement;
class ShimmerUnclosedFunc;
class ShimmerLiteral;
class ShimmerExpr;
class ShimmerScope;
class LookupResult;

typedef enum { NONETYPE, LITERAL, STATEMENT, IDENTIFIER, FUNCTION } ParamType;
typedef std::unordered_map<std::string, ShimmerLiteral*> Scope;

// typedef int (*BIFNextFunc)(int line, int res, ShimmerExpr param);
// typedef int (*BIFCalcFunc)(int line, int op1, int op2);

typedef std::function<double(int, double, ShimmerLiteral*)>             BIFNextFunc;
typedef std::function<double(int, ShimmerLiteral*, ShimmerLiteral*)> BIFCalcFunc;

class ShimmerToken {
  public:
    ShimmerToken();
    int line;
    std::string contents;
    double parsed_contents;
    std::string token_type;

    int get_line();
    std::string get_contents();
    double get_parsed_contents();
    std::string get_token_type();

    bool is_of_type(std::string type);
    bool not_of_type(std::string type);
    std::string to_string();
};

class ShimmerLParen : public ShimmerToken  {
  public:
    ShimmerLParen(int line);
};

class ShimmerRParen : public ShimmerToken  {
  public:
    ShimmerRParen(int line);
};

class ShimmerLBrace : public ShimmerToken  {
  public:
    ShimmerLBrace(int line);
};

class ShimmerRBrace : public ShimmerToken  {
  public:
    ShimmerRBrace(int line);
};

class ShimmerIDLiteralSign : public ShimmerToken  {
  public:
    ShimmerIDLiteralSign(int line);
};

class ShimmerComma : public ShimmerToken  {
  public:
    ShimmerComma(int line);
};

class ShimmerNumber : public ShimmerToken {
  public:
    ShimmerNumber(int line, std::string content);
};

class ShimmerString : public ShimmerToken {
  public:
    ShimmerString(int line, std::string content);
};

class ShimmerIdentifier : public ShimmerToken {
  public:
    ShimmerIdentifier(int line, std::string content);
    ShimmerIdentifier();
};

class ShimmerExpr {
  public:
		ShimmerExpr();
    ShimmerExpr(ShimmerLiteral literal_value);
    ShimmerExpr(ShimmerStatement statement_value);
    ShimmerExpr(ShimmerIdentifier identifier_value);
    ShimmerExpr(ShimmerUnclosedFunc& func_value);
    //ShimmerExpr(const ShimmerExpr &param);

    ParamType param_type = NONETYPE;
    ParamType get_param_type();
    bool is_of_type(ParamType type);
    bool not_of_type(ParamType type);

    ShimmerLiteral* literal_val;
    ShimmerStatement* statement_val;
    ShimmerIdentifier identifier_val;
    ShimmerUnclosedFunc* func_val;

    ShimmerLiteral get_literal_val();
    ShimmerStatement get_statement_val();
    ShimmerIdentifier get_identifier_val();
    ShimmerUnclosedFunc get_func_val();
};

class ShimmerStatement {
  public:
    ShimmerStatement();
    ShimmerStatement(ShimmerExpr _expr, std::vector<ShimmerExpr> _params);
		ShimmerStatement(const ShimmerStatement &statement); 

    std::vector<ShimmerExpr> params;
    ShimmerExpr expr;
    // TODO: add identifier value

    std::vector<ShimmerExpr> get_params();
    ShimmerExpr get_expr();

    void set_params(std::vector<ShimmerExpr> param);
    void set_expr(ShimmerExpr exp);

    ShimmerLiteral eval(ShimmerScope* scope);
    LookupResult lookup_tables(ShimmerScope* scope);

  private:
    void error_on_wrong_num_params(int line, unsigned int min, unsigned int max, std::string msg);
    void error_on_missing_params(int line, unsigned int min, std::string msg);
    void error_on_extra_params(int line, unsigned int max, std::string msg);

    LookupResult math_func_var(std::string name, int line, double start,                       BIFNextFunc next);
    LookupResult math_func_dya(std::string name, int line, std::vector<ShimmerExpr> params, BIFCalcFunc calc);

    // template<typename... Types> void __error_on_wrong_num_params(int line, int min, int max, Types... args);
    // template<typename... Types> void __error_on_missing_params(int line, int min, Types... args);
    // template<typename... Types> void __error_on_extra_params(int line, int max, Types... args);
};

class BuiltinFuncs {
  public:
    static BIFNextFunc add();
    static BIFCalcFunc sub();
    static BIFNextFunc mul();
    static BIFCalcFunc div();
};

class ShimmerTree {
  public:
    ShimmerTree();
    ShimmerTree(std::vector<ShimmerExpr> statement);
    std::vector<ShimmerExpr> tree;
    std::vector<ShimmerExpr> get_tree();
};

class ShimmerScope {
  public:
    ShimmerScope(); // Default constructor
    ShimmerScope(Scope cur_scope);
    ShimmerScope(ShimmerScope* up_scope, Scope cur_scope);

    ShimmerScope* upper_scope;
    Scope current_scope;

    void declare_variable(std::string var_name, ShimmerLiteral val);
    ShimmerLiteral get_variable(std::string var_name);
    void set_variable(std::string var_name, ShimmerLiteral val);
    void cons_scope(ShimmerScope* _upper_scope);
};

class ShimmerUnclosedFunc {
	public:
		std::vector<ShimmerIdentifier> params;
    ShimmerTree tree;
		ShimmerUnclosedFunc(std::vector<ShimmerIdentifier> _params, ShimmerTree _tree);
    ShimmerUnclosedFunc() {}
};

class ShimmerClosedFunc {
	public:
		std::vector<ShimmerIdentifier> params;
    ShimmerTree tree;
    ShimmerScope* closed_scope;
		ShimmerClosedFunc(ShimmerUnclosedFunc to_close, ShimmerScope* _closed_scope);
    ShimmerClosedFunc() {}
};

class ShimmerLiteral {
  public:
    explicit ShimmerLiteral();
    explicit ShimmerLiteral(int line, double val);
    explicit ShimmerLiteral(int line, std::string val);
    explicit ShimmerLiteral(int line, ShimmerClosedFunc val);
    explicit ShimmerLiteral(int line, ShimmerIdentifier val);
    explicit ShimmerLiteral(int line, bool val);

    int type;
    double int_value;
    bool bool_value;
    std::string str_value;
    ShimmerIdentifier id_value;
    ShimmerClosedFunc func_value;

    int get_type();
    double get_num();
    bool get_bool();
    std::string get_str();
    ShimmerIdentifier get_id();
    ShimmerClosedFunc get_func();
};

class LookupResult {
  public:
    bool found;

    ShimmerLiteral value;

    LookupResult() {
      found = false;
    }

    LookupResult(ShimmerLiteral _value) {
      found = true;
      value = _value;
    }
};

#endif