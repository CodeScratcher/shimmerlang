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

typedef enum { NONE, LITERAL, STATEMENT, IDENTIFIER } ParamType;

class ShimmerScope;
class DotTree;
class DotIdentifier;
class DotLiteral;
class DotStatement;
class ShimmerParam;

typedef std::unordered_map<std::string, DotLiteral> Scope;

class DotToken {
  public:
    DotToken();

    std::string contents;
    int parsed_contents;
    std::string token_type;

    std::string get_contents();
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

class DotLBrace : public DotToken  {
  public:
    DotLBrace();
};

class DotRBrace : public DotToken  {
  public:
    DotRBrace();
};

class DotIDLiteralSign : public DotToken  {
  public:
    DotIDLiteralSign();
};

class DotComma : public DotToken  {
  public:
    DotComma();
};

class DotInt : public DotToken {
  public:
    DotInt(std::string content);
};

class DotString : public DotToken {
  public:
    DotString(std::string content);
};

class DotIdentifier : public DotToken {
  public:
    DotIdentifier(std::string content);
};

class DotStatement {
  public:
    DotStatement();
    DotStatement(std::string ident, std::vector<ShimmerParam> param);

    std::vector<ShimmerParam> params;
    std::string identifier;

    std::vector<ShimmerParam> get_params();
    std::string get_identifier();

    void set_params(std::vector<ShimmerParam> param);
    void set_identifier(std::string ident);

    DotLiteral eval();
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
    DotLiteral();
    DotLiteral(int val);
    DotLiteral(std::string val);
    DotLiteral(DotTree val);
    DotLiteral(DotIdentifier val);

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

class ShimmerParam {
  public:
    ShimmerParam(DotLiteral literal_value);
    ShimmerParam(DotStatement statement_value);
    ShimmerParam(DotIdentifier identifier_value);

    ParamType param_type = NONE;
    ParamType get_param_type();
    bool is_of_type(ParamType type);

    DotLiteral literal_val;
    DotStatement statement_val;
    DotIdentifier identifier_val;

    DotLiteral get_literal_val();
    DotStatement get_statement_val();
    DotIdentifier get_identifier_val();
};

class ShimmerScope {
  public:
    ShimmerScope(); // Default constructor
    ShimmerScope(Scope cur_scope);
    ShimmerScope(ShimmerScope* up_scope, Scope cur_scope);

    ShimmerScope* upper_scope;
    Scope current_scope;

    DotLiteral get_variable_by_name(std::string var_name);
};

#endif