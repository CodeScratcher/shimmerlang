#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "ShimmerClasses.h"
#include "text_effects.h"
#include "lexer.h"

Lexer::Lexer() {
  current_token_contents = "";
  now_in = NONE;
}

std::vector<DotToken> Lexer::lex(std::string str) {
  std::vector<DotToken> to_return;
  DotToken temp_token;

  for (int i = 0; i < str.length(); i++) {
    char ch = str.at(i);

    if (now_in != STR && (ch == ' ' || ch == '\t' || ch == '\n')) {
      finish_current_token();
      continue;
    }

    if (ch == '(') {
      if (currently_in_token()) {
        finish_current_token();
      }

      temp_token = DotLParen();
      to_return.push_back(temp_token);
    }
    else if (ch == ')') {
      if (currently_in_token()) {
        finish_current_token();
      }

      temp_token = DotRParen();
      to_return.push_back(temp_token);
    }
    else if (std::regex_search(&ch, std::regex("[a-zA-Z]")) && now_in == NONE) {
     // Starting identifier
      now_in = ID;
      current_token_contents.push_back(ch);
    }
    else if (std::regex_search(&ch, std::regex("[a-zA-Z0-9]")) && now_in == ID) {
      // Continuing identifier
      current_token_contents.push_back(ch);
    }
    else if (ch == '\'' || ch == '"') {
      // Starting string
      now_in = STR;
      string_delimiter = ch;
    }
    else if (now_in == STR) {
      // Continuing string
      if (ch == string_delimiter) {
        finish_current_token();
      }
      else {
        current_token_contents.push_back(ch);
      }
    }
    else if (std::regex_search(&ch, std::regex("[0-9]")) && now_in != STR && now_in != ID) {
      now_in = INT;
      current_token_contents.push_back(ch);
    }
    else if (ch == '$') {
      if (currently_in_token()) {
        finish_current_token();
      }

      temp_token = DotIDLiteralSign();
      to_return.push_back(temp_token);
    }
    else if (ch == '{') {
      if (currently_in_token()) {
        finish_current_token();
      }

      temp_token = DotLBrace();
      to_return.push_back(temp_token);
    }
    else if (ch == '}') {
      if (currently_in_token()) {
        finish_current_token();
      }

      temp_token = DotRBrace();
      to_return.push_back(temp_token);
    }
    else if (ch == ',') {
      if (currently_in_token()) {
        finish_current_token();
      }

      temp_token = DotComma();
      to_return.push_back(temp_token);
    }
    else {
      std::string suspect = "";

      if (ch < ' ') {
        suspect = "char code DEC " + std::to_string((int) ch);
      }
      else {
        suspect = std::string(&ch);
      }

      std::string message = "Unknown character <" + suspect + ">";
      throw std::runtime_error(message);
    }
  }

  return to_return;
}

bool Lexer::currently_in_token() {
  return current_token_contents != "";
}

void Lexer::finish_current_token() {
  to_return.push_back(make_token());
  now_in = NONE;
  clear_current_token();
}

void Lexer::clear_current_token() {
  current_token_contents = "";
}

DotToken Lexer::make_token() {
  if(now_in == STR)  return DotString(current_token_contents);
  if(now_in == INT)  return DotInt(current_token_contents);
  if(now_in == ID)   return DotIdentifier(current_token_contents);
  if(now_in == NONE) return DotString("");
  else throw std::runtime_error("illegal now_in state: " + std::to_string(now_in));
}

#ifdef DEBUG
const char* lex_to_str(std::vector<DotToken> lexed) {
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
const char* lex_test() {
  std::vector<DotToken> lexed = lex("print(add(4, div(6, 2)))");
  std::cout << "Lexed successfully. Entered testing.\n\n";
  return lex_to_str(lexed);
}
#endif
