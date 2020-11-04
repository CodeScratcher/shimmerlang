#ifndef SHIMMER_CLASSES_H
#define SHIMMER_CLASSES_H

#include <string>
#include <vector>
#include <any>

enum DotTypes {
  TypeString,
  TypeInt,
  TypeBool,
  TypeFunc
};
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
    std::string get_contents();
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
    DotLiteral(int val);
    DotLiteral(std::string val);
    DotLiteral(DotTree function);
    int get_type();
    int get_int();
    DotTree get_func();
    bool get_bool();
    std::string get_str();
};
class ShimmerParam {
  public:
    DotLiteral literal_val;
    DotStatement statement_val;
    bool is_literal;
    ShimmerParam(DotLiteral literal_value);
    ShimmerParam(DotStatement statement_value);
    DotLiteral get_literal_val();
    DotStatement get_statement_val();
    bool get_is_literal();
};
#endif