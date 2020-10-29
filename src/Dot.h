#include <string>
#include <vector>
#include <any>
#ifndef DOT_LANG_CLASSES
#define DOT_LANG_CLASSES
enum DotTypes {
  TypeString,
  TypeInt,
  TypeBool,
  TypeFunc
};
class DotToken;
class DotTree;
class DotIdentifier;
class DotRBrace;
class DotLBrace;
class DotLiteral;
class DotRParen;
class DotLParen;
class DotStatement;
class DotString;
class DotInt;
class DotToken {
  public:
    std::string contents;
    std::string token_type;
    DotToken();
    int getParsedInt();
    std::string get_token_type();
    std::string toString();
    std::string getContents();
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
    int parsedContents;
    int getParsedContents();
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
    std::vector<DotLiteral> params;
    std::string identifier;
    DotStatement(std::string ident, std::vector<DotLiteral> param);
    DotStatement();
    std::string get_identifier();
    void set_identifier(std::string ident);
    DotLiteral eval();
};
class DotTree {
  public:
    DotTree(std::vector<DotStatement> statement);
    DotTree();
    std::vector<DotStatement> statements;
    std::vector<DotStatement> getTree();
};
class DotLiteral {
  public:
    int type;
    std::string str_value;
    bool bool_value;
    int int_value;
    DotTree func_value;
    DotLiteral(int val);
    DotLiteral(std::string val);
    DotLiteral(DotTree function);
    DotLiteral(DotStatement statement);
    int getType();
    int get_int();
    DotTree get_func();
    bool get_bool();
    std::string get_str();
};
#endif