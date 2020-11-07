#ifndef SHIMMER_CLASSES_H
#define SHIMMER_CLASSES_H

#include <string>
#include <vector>
#include <any>
#include <unordered_map>

typedef Scope std::unordered_map<std::string, DotLiteral>;

enum DotTypes {
  TypeString,
  TypeInt,
  TypeBool,
  TypeFunc,
  TypeId
};

typedef enum { NONE, LITERAL, STATEMENT, IDENTIFIER } ParamType;

class ShimmerScope;
class DotTree;
class DotIdentifier;
class DotLiteral;
class DotStatement;
class ShimmerParam;
class DotToken {
  public:
    std::string contents;
    std::string token_type;
    DotToken();
    int parsed_contents;
    int get_parsed_contents();
    std::string get_token_type();
    bool is_of_type(std::string type);
    std::string to_string();
};

class DotLParen : public DotToken  {
  public:
    DotLParen();
};

class DotRParen : public DotToken  {
  public:
    DotRParen();
};
class DotIDLiteralSign : public DotToken  {
  public:
    DotIDLiteralSign();
};
class DotIdentifier : public DotToken {
  public:
    DotIdentifier(std::string content);
};

class DotString : public DotToken {
  public:
    DotString(std::string content);
};

class DotInt : public DotToken {
  public:
    DotInt(std::string content);
};

class DotLBrace : public DotToken  {
  public:
    DotLBrace();
};

class DotRBrace : public DotToken  {
  public:
    DotRBrace();
};

class DotComma : public DotToken  {
  public:
    DotComma();
};
class DotStatement {
  public:
    std::vector<ShimmerParam> params;
    std::string identifier;
    DotStatement(std::string ident, std::vector<ShimmerParam> param);
    DotStatement();
    std::vector<ShimmerParam> get_params();
    std::string get_identifier();
    void set_identifier(std::string ident);
    void set_params(std::vector<ShimmerParam> param);
    DotLiteral eval();
};

class DotTree {
  public:
    DotTree(std::vector<DotStatement> statement);
    DotTree();
    std::vector<DotStatement> statements;
    std::vector<DotStatement> get_tree();
};

class DotLiteral {
  public:
    DotLiteral();
    int type;
    std::string str_value;
    bool bool_value;
    int int_value;
    DotTree func_value;
    DotIdentifier id_value;
    DotLiteral(int val);
    DotLiteral(DotIdentifier val);
    DotLiteral(std::string val);
    DotLiteral(DotTree function);
    int get_type();
    int get_int();
    DotTree get_func();
    bool get_bool();
    DotIdentifier get_id();
    std::string get_str();
};

class ShimmerParam {
  public:
    ShimmerParam(DotLiteral literal_value);
    ShimmerParam(DotStatement statement_value);
    ShimmerParam(DotIdentifier identifier_value);

    ParamType param_type = NONE;
    ParamType get_param_type();

    DotLiteral literal_val;
    DotStatement statement_val;
    DotIdentifier identifier_val;

    DotLiteral get_literal_val();
    DotStatement get_statement_val();
    DotIdentifier get_identifier_val();
};

class ShimmerScope {
  ShimmerScope* upper_scope;
  Scope current_scope;
  ShimmerScope(Scope cur_scope);
  ShimmerScope(ShimmerScope* up_scope, Scope cur_scope);
  ShimmerScope(); // Default constructor
  DotLiteral get_variable_by_name(std::string var_name);
};

#endif