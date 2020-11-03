#include <iostream>
#include <string>
#include <vector>
#include <regex>
#ifdef BENCHMARK
#include <fstream>
#include <sstream>
#endif
#include "ShimmerClasses.h"
#include "text_effects.h"
#include "lexer.h"

DotToken make_token \
(State now_in, \
std::string current_token_contents);

std::vector<DotToken> lex(std::string str) {
  std::vector<DotToken> toReturn;
  DotToken dToken;
  std::string current_token_contents = "";
  char string_watch_out_for;
  State now_in = NONE;
  for (int i = 0; i < str.length(); i++) {
    char ch = str.at(i);
    // std::cout << i << ": "<< ch << "\n";
    if (now_in == STR && !(ch == string_watch_out_for)) {
      if (ch == string_watch_out_for) {
        dToken = make_token(now_in, current_token_contents);
        now_in = NONE;
        current_token_contents = "";
        toReturn.push_back(dToken);
      }
      else {
        current_token_contents.push_back(ch);
      }
    }
    else if (ch == '(') {
      if (current_token_contents != "") {
        dToken = make_token(now_in, current_token_contents);
        now_in = NONE;
        toReturn.push_back(dToken);
      }

      current_token_contents = "";
      dToken = DotLParen();
      toReturn.push_back(dToken);
    }
    else if (ch == ')') {
      if (current_token_contents != "") {
        dToken = make_token(now_in, current_token_contents);
        now_in = NONE;
        toReturn.push_back(dToken);
      }
  
      current_token_contents = "";
      dToken = DotRParen();
      toReturn.push_back(dToken);
    }
    else if (std::regex_search(&ch, std::regex("[a-zA-Z]")) && now_in == NONE) {
     // std::cout << "Starting an identifier with char: " << ch << "\n";
      now_in = ID;
      current_token_contents.push_back(ch);
    }
    else if (std::regex_search(&ch, std::regex("[a-zA-Z0-9]")) && now_in == ID) {
      //std::cout << "Continuing identifier with char: " << ch << "\n";
      current_token_contents.push_back(ch);
      //std::cout << "Current contents: " << current_token_contents << "\n";
    }
    else if (ch == '\'' || ch == '"') {
      now_in = STR;
      string_watch_out_for = ch;
    }
    else if (std::regex_search(&ch, std::regex("[0-9]")) && \
             now_in != STR && now_in != ID) {
      now_in = INT;
      current_token_contents.push_back(ch);
    }
    else if (ch == '{') {
      if (current_token_contents != "") {
        dToken = make_token(now_in, current_token_contents);
        now_in = NONE;
        toReturn.push_back(dToken);
      }

      current_token_contents = "";
      dToken = DotLBrace();
      toReturn.push_back(dToken);
    }
    else if (ch == '}') {
      if (current_token_contents != "") {
        dToken = make_token(now_in, current_token_contents);
        now_in = NONE;
        toReturn.push_back(dToken);
      }

      current_token_contents = "";
      dToken = DotRBrace();
      toReturn.push_back(dToken);
    }
    else if (ch == ',') {
      if (current_token_contents != "") {
        dToken = make_token(now_in, current_token_contents);
        now_in = NONE;
        toReturn.push_back(dToken);
      }

      current_token_contents = "";
      dToken = DotComma();
      toReturn.push_back(dToken);
    }
  }
  return toReturn;
}

DotToken make_token(State now_in, std::string current_token_contents) {
  if(now_in == STR)  return DotString(current_token_contents);
  if(now_in == INT)  return DotInt(current_token_contents);
  if(now_in == ID)   return DotIdentifier(current_token_contents);
  if(now_in == NONE) return DotString("");
  else throw std::runtime_error("illegal state: " + std::to_string(now_in));
}

#ifdef DEBUG
const char* lex_test() {
  std::vector<DotToken> lexed = lex("print(add(5, 3))");
  std::cout << "Lexed successfully. Entered testing.\n\n";
  std::string str;
  std::cout << "       " << BOLD("Name") << "\t\t" << BOLD("Type \n");
  for(DotToken i : lexed) {
    std::string token_contents = i.get_contents();
    std::string token_type = i.get_token_type();
    int token_length = token_contents.length();

    std::string padding = "\t";
    str.append(token_contents);

    if (token_length < 4) {
      padding = "\t\t";
    }

    std::cout << "Found: " << token_contents << padding << token_type << "\n";
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