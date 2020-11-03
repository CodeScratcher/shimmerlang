#include <iostream>
#include <string>
#include <vector>
#include <regex>
#ifdef BENCHMARK
#include <fstream>
#include <sstream>
#endif
#include "ShimmerClasses.h"

DotToken make_token \
(bool string, bool integer, bool identifier, \
std::string current_token_contents);

std::vector<DotToken> lex(std::string str) {
  std::vector<DotToken> toReturn;
  DotToken dToken;
  std::string current_token_contents = "";
  char string_watch_out_for;
  State currently_in = NONE;
  for (int i = 0; i < str.length(); i++) {
    char ch = str.at(i);
    // std::cout << i << ": "<< ch << "\n";
    if (in_string && !(ch == string_watch_out_for)) {
      current_token_contents.push_back(ch);
    }
    else if (in_string && ch == string_watch_out_for) {
      dToken = make_token(in_string, in_int, in_identifier, current_token_contents);
      currently_in = NONE;
      current_token_contents = "";
      toReturn.push_back(dToken);
    }
    else if (ch == '(') {
      if (current_token_contents != "") {
        dToken = make_token(in_string, in_int, in_identifier, current_token_contents);
        currently_in = NONE;
        toReturn.push_back(dToken);
      }
      current_token_contents = "";
      dToken = DotLParen();
      toReturn.push_back(dToken);
    }
    else if (ch == ')') {
      if (current_token_contents != "") {
        dToken = make_token(in_string, in_int, in_identifier, current_token_contents);
        currently_in = NONE;
        toReturn.push_back(dToken);
      }
  
      current_token_contents = "";
      dToken = DotRParen();
      toReturn.push_back(dToken);
    }
    else if (std::regex_search(&ch, std::regex("[a-zA-Z]")) && \
             !(in_string || in_int || in_identifier)) {
     // std::cout << "Starting an identifier with char: " << ch << "\n";
      currently_in = ID;
      current_token_contents.push_back(ch);
    }
    else if (std::regex_search(&ch, std::regex("[a-zA-Z0-9]")) && in_identifier) {
      //std::cout << "Continuing identifier with char: " << ch << "\n";
      current_token_contents.push_back(ch);
      //std::cout << "Current contents: " << current_token_contents << "\n";
    }
    
    else if (ch == '\'') {
      currently_in = STR;
      string_watch_out_for = '\'';
    }
    else if (ch == '"') {
      currently_in = STR;
      string_watch_out_for = '"';
    }
    else if (std::regex_search(&ch, std::regex("[0-9]")) && !in_string && !in_identifier) {
      currently_in = INT;
      current_token_contents.push_back(ch);
    }
    else if (ch == '{') {
      if (current_token_contents != "") {
        dToken = make_token(in_string, in_int, in_identifier, current_token_contents);
        currently_in = NONE;
        toReturn.push_back(dToken);
      }
      current_token_contents = "";
      dToken = DotLBrace();
      toReturn.push_back(dToken);
    }
    else if (ch == '}') {
      if (current_token_contents != "") {
        dToken = make_token(in_string, in_int, in_identifier, current_token_contents);
        currently_in = NONE;
        toReturn.push_back(dToken);
      }
      current_token_contents = "";
      dToken = DotRBrace();
      toReturn.push_back(dToken);
    }
    else if (ch == ',') {
      if (current_token_contents != "") {
        dToken = make_token(in_string, in_int, in_identifier, current_token_contents);
        currently_in = NONE;
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

DotToken make_token\
(bool string, bool integer,\
bool identifier, std::string current_token_contents) {
  if(string) return DotString(current_token_contents);
  if(integer) return DotInt(current_token_contents);
  if(identifier) return DotIdentifier(current_token_contents);
  else return DotString("");
}

#ifdef DEBUG
const char* lex_test() {
  std::vector<DotToken> lexed = lex("print(add(5, 3))");
  std::cout << "Lexed successfully. Entered testing \n";
  std::string str;
  for(DotToken i : lexed) {
    str.append(i.get_contents());
    std::cout << "Token found: " << i.get_contents() << "\n";
  }
  return str.c_str();
}
#endif
#ifdef BENCHMARK
int main() {
  std::ifstream file;
    file.open("lexerbenchmark.dot");

    if (!file) {
      std::cout << "Error while opening file.";
    }
    else {
      std::stringstream buffer;
      buffer << file.rdbuf();
      lex(buffer.str());
    }
}
#endif