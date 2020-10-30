#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "ShimmerClasses.h"

DotToken make_token(bool string, bool integer, bool identifier, std::string current_token_contents);

std::vector<DotToken> lex(std::string str) {
  std::vector<DotToken> toReturn;
  DotToken dToken;
  std::string current_token_contents = "";
  bool in_string = false;
  char string_watch_out_for;
  bool in_int = false;
  bool in_identifier = false;
  for (int i = 0; i < str.length(); i++) {
    char ch = str.at(i);
    if (in_string && !(ch == string_watch_out_for)) {
      current_token_contents.append(&ch);
    }
    else if (ch == '(') {
     if (current_token_contents != "") {
      dToken = make_token(in_string, in_int, in_identifier, current_token_contents);
      in_string = false;
      in_int = false;
      in_identifier = false;
      toReturn.push_back(dToken);
      }
      current_token_contents = "";
      dToken = DotLParen();
      toReturn.push_back(dToken);
    }
    else if (ch == ')') {
    if (current_token_contents != "") {
      dToken = make_token(in_string, in_int, in_identifier, current_token_contents);
      in_string = false;
      in_int = false;
      in_identifier = false;
      toReturn.push_back(dToken);
      }
      current_token_contents = "";
      dToken = DotRParen();
      toReturn.push_back(dToken);
    }
    else if (std::regex_match(&ch, std::regex("[a-zA-Z]")) && \
             !(in_string | in_int | in_identifier)) {
      in_identifier = true;
      current_token_contents.append(&ch);
    }
    else if (ch == '\'') {
      in_string = true;
      string_watch_out_for = '\'';
    }
    else if (ch == '"') {
      in_string = true;
      string_watch_out_for = '"';
    }
    else if (std::regex_match(&ch, std::regex("[a-zA-Z0-9]")) && in_identifier) {
      current_token_contents.append(&ch);
    }
    else if (std::regex_match(&ch, std::regex("[0-9]")) && !in_string && !in_identifier) {
      in_int = true;
      current_token_contents.append(&ch);
    }
    else if (ch == '{') {
      if (current_token_contents != "") {
        dToken = make_token(in_string, in_int, in_identifier, current_token_contents);
        in_string = false;
        in_int = false;
        in_identifier = false;
        toReturn.push_back(dToken);
      }
      current_token_contents = "";
      dToken = DotLBrace();
      toReturn.push_back(dToken);
    }
    else if (ch == '}') {
      if (current_token_contents != "") {
        dToken = make_token(in_string, in_int, in_identifier, current_token_contents);
        in_string = false;
        in_int = false;
        in_identifier = false;
        toReturn.push_back(dToken);
      }
      current_token_contents = "";
      dToken = DotRBrace();
      toReturn.push_back(dToken);
    }
    else if (ch == ',') {
      if (current_token_contents != "") {
        dToken = make_token(in_string, in_int, in_identifier, current_token_contents);
        in_string = false;
        in_int = false;
        in_identifier = false;
        toReturn.push_back(dToken);
      }
      current_token_contents = "";
      dToken = DotComma();
      toReturn.push_back(dToken);
    }
    else {
    
    }
  }
  return toReturn;
}

DotToken make_token(bool string, bool integer, bool identifier, std::string current_token_contents) {
  if(string) return DotString(current_token_contents);
  if(integer) return DotInt(current_token_contents);
  if(identifier) return DotIdentifier(current_token_contents);
  else throw std::runtime_error("This should be unreachable");
}

#ifdef DEBUG
const char* lex_test() {
  std::vector<DotToken> lexed = lex("print('hello world')");
  std::string str;
  for(DotToken i : lexed) {
    str.append(i.get_contents());
    std::cout << "Token found: " << i.get_contents() << "\n";
  }
  return str.c_str();
}
#endif
