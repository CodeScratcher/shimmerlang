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

typedef enum { NONETYPE, LITERAL, STATEMENT, IDENTIFIER } ParamType;

class DotIdentifier;
class DotStatement;
class DotTree;
class DotLiteral;
class ShimmerParam;
class ShimmerScope;

typedef std::unordered_map<std::string, DotLiteral> Scope;

class DotToken {
  public:
    DotToken();
    int line_number;
    std::string contents;
    int parsed_contents;
    std::string token_type;

    int get_line_number();
    std::string get_contents();
    int get_parsed_contents();
    std::string get_token_type();

    bool is_of_type(std::string type);
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

    ParamType param_type = NONETYPE;
    ParamType get_param_type();
    bool is_of_type(ParamType type);

    DotLiteral*  literal_val;
    DotStatement* statement_val;
    DotIdentifier identifier_val;

    DotLiteral get_literal_val();
    DotStatement get_statement_val();
    DotIdentifier get_identifier_val();
};

class DotStatement {
  public:
    DotStatement();
    DotStatement(ShimmerParam exp, std::vector<ShimmerParam> param);

    std::vector<ShimmerParam> params;
    ShimmerParam expr;
    // TODO: add identifier value

    std::vector<ShimmerParam> get_params();
    ShimmerParam get_expr();
    void get_identifier(); // TODO

    void set_params(std::vector<ShimmerParam> param);
    void set_expr(ShimmerParam exp);
    void set_identifier(); // TODO

    DotLiteral eval(ShimmerScope* scope);

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

class DotLiteral {
  public:
    explicit DotLiteral();
    explicit DotLiteral(int line, int val);
    explicit DotLiteral(int line, std::string val);
    explicit DotLiteral(int line, DotTree val);
    explicit DotLiteral(int line, DotIdentifier val);

    int type;
    int int_value;
    bool bool_value;
    std::string str_value;
    DotTree func_value;
    DotIdentifier id_value;

    int get_type();
    int get_int();
    bool get_bool();
    std::string get_str();
    DotTree get_func();
    DotIdentifier get_id();
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

#endif