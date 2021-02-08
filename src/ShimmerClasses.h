#ifndef SHIMMER_CLASSES_H
#define SHIMMER_CLASSES_H

#include <string>
#include <vector>
#include <any>
#include <unordered_map>

enum DotTypes {
  TypeString,
  TypeInt,
  TypeBool,
  TypeFunc,
  TypeId
};

typedef enum { NONETYPE, LITERAL, STATEMENT, IDENTIFIER, FUNCTION } ParamType;

class DotIdentifier;
class DotStatement;
class ShimmerUnclosedFunc;
class DotLiteral;
class ShimmerParam;
class ShimmerScope;
class LookupResult;
typedef std::unordered_map<std::string, DotLiteral*> Scope;

class DotToken {
  public:
    DotToken();
    int line;
    std::string contents;
    int parsed_contents;
    std::string token_type;

    int get_line();
    std::string get_contents();
    int get_parsed_contents();
    std::string get_token_type();

    bool is_of_type(std::string type);
    bool not_of_type(std::string type);
    std::string to_string();
};

class DotLParen : public DotToken  {
  public:
    DotLParen(int line);
};

class DotRParen : public DotToken  {
  public:
    DotRParen(int line);
};

class DotLBrace : public DotToken  {
  public:
    DotLBrace(int line);
};

class DotRBrace : public DotToken  {
  public:
    DotRBrace(int line);
};

class DotIDLiteralSign : public DotToken  {
  public:
    DotIDLiteralSign(int line);
};

class DotComma : public DotToken  {
  public:
    DotComma(int line);
};

class DotInt : public DotToken {
  public:
    DotInt(int line, std::string content);
};

class DotString : public DotToken {
  public:
    DotString(int line, std::string content);
};

class DotIdentifier : public DotToken {
  public:
    DotIdentifier(int line, std::string content);
    DotIdentifier();
};

class ShimmerParam {
  public:
		ShimmerParam();
    ShimmerParam(DotLiteral& literal_value);
    ShimmerParam(DotStatement& statement_value);
    ShimmerParam(DotIdentifier identifier_value);
    ShimmerParam(ShimmerUnclosedFunc& func_value);
    ShimmerParam(const ShimmerParam &param); // will this work?

    ParamType param_type = NONETYPE;
    ParamType get_param_type();
    bool is_of_type(ParamType type);
    bool not_of_type(ParamType type);

    DotLiteral* literal_val;
    DotStatement* statement_val;
    DotIdentifier identifier_val;
    ShimmerUnclosedFunc* func_val;

    DotLiteral get_literal_val();
    DotStatement get_statement_val();
    DotIdentifier get_identifier_val();
    ShimmerUnclosedFunc get_func_val();
};

class DotStatement {
  public:
    DotStatement();
    DotStatement(ShimmerParam _expr, std::vector<ShimmerParam> _params);
		DotStatement(const DotStatement &statement); 

    std::vector<ShimmerParam> params;
    ShimmerParam expr;
    // TODO: add identifier value

    std::vector<ShimmerParam> get_params();
    ShimmerParam get_expr();

    void set_params(std::vector<ShimmerParam> param);
    void set_expr(ShimmerParam exp);

    DotLiteral eval(ShimmerScope* scope);
    LookupResult lookup_tables();

  private:
    void error_on_missing_params(int line, int min, std::string msg);
    void error_on_extra_params(int line, int max, std::string msg);
};

class DotTree {
  public:
    DotTree();
    DotTree(std::vector<DotStatement> statement);
    std::vector<DotStatement> tree;
    std::vector<DotStatement> get_tree();
};

class ShimmerScope {
  public:
    ShimmerScope(); // Default constructor
    ShimmerScope(Scope cur_scope);
    ShimmerScope(ShimmerScope* up_scope, Scope cur_scope);

    ShimmerScope* upper_scope;
    Scope current_scope;

    void declare_variable(std::string var_name, DotLiteral val);
    DotLiteral get_variable(std::string var_name);
    void set_variable(std::string var_name, DotLiteral val);
};

class ShimmerUnclosedFunc {
	public:
		std::vector<DotIdentifier> params;
    DotTree tree;
		ShimmerUnclosedFunc(std::vector<DotIdentifier> _params, DotTree _tree);
    ShimmerUnclosedFunc() {}
};

class ShimmerClosedFunc {
	public:
		std::vector<DotIdentifier> params;
    DotTree tree;
    ShimmerScope* closed_scope;
		ShimmerClosedFunc(ShimmerUnclosedFunc to_close, ShimmerScope* closed_scope);
};

class DotLiteral {
  public:
    explicit DotLiteral();
    explicit DotLiteral(int line, int val);
    explicit DotLiteral(int line, std::string val);
    // explicit DotLiteral(int line, ShimmerUnclosedFunc val);
    explicit DotLiteral(int line, DotIdentifier val);

    int type;
    int int_value;
    bool bool_value;
    std::string str_value;
    ShimmerUnclosedFunc* func_value;
  
    DotIdentifier id_value;

    int get_type();
    int get_int();
    bool get_bool();
    std::string get_str();
    // ShimmerUnclosedFunc get_func();
    DotIdentifier get_id();
};

class LookupResult {
  public:
    bool found;

    DotLiteral value;
    LookupResult() {
      found = false;
    }

    LookupResult(DotLiteral _value) {
      found = true;
      value = _value;
    }
};

#endif