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

std::vector<DotToken> lex(std::string str) {
  std::vector<DotToken> to_return;
  DotToken this_token;
  std::string current_token_contents = "";
  char string_watch_out_for;
  State now_in = NONE;

  for (int i = 0; i < str.length(); i++) {
    char ch = str.at(i);
    std::cout << i << ": "<< ch << "\n";
    if (now_in != STR && (ch == ' ' || ch == '\t' || ch == '\n')) {
      if (current_token_contents != "") {
        this_token = make_token(now_in, current_token_contents);
        now_in = NONE;
        to_return.push_back(this_token);
      }
  
      current_token_contents = "";
      continue;
    }
    if (now_in == STR) {
      if (ch == string_watch_out_for) {
        this_token = make_token(now_in, current_token_contents);
        now_in = NONE;
        current_token_contents = "";
        to_return.push_back(this_token);
      }
      else {
        current_token_contents.push_back(ch);
      }
    }
    else if (ch == '(') {
      if (current_token_contents != "") {
        this_token = make_token(now_in, current_token_contents);
        now_in = NONE;
        to_return.push_back(this_token);
      }

      current_token_contents = "";
      this_token = DotLParen();
      to_return.push_back(this_token);
    }
    else if (ch == ')') {
      if (current_token_contents != "") {
        this_token = make_token(now_in, current_token_contents);
        now_in = NONE;
        to_return.push_back(this_token);
      }
  
      current_token_contents = "";
      this_token = DotRParen();
      to_return.push_back(this_token);
    }
    else if (std::regex_search(&ch, std::regex("[a-zA-Z_]")) && now_in == NONE) {
     // std::cout << "Starting an identifier with char: " << ch << "\n";
      now_in = ID;
      current_token_contents.push_back(ch);
    }
    else if (std::regex_search(&ch, std::regex("[a-zA-Z0-9_]")) && now_in == ID) {
      //std::cout << "Continuing identifier with char: " << ch << "\n";
      current_token_contents.push_back(ch);
      //std::cout << "Current contents: " << current_token_contents << "\n";
    }
    else if ((ch == '\'' || ch == '"') && now_in == NONE) {
      now_in = STR;
      std::cout << current_token_contents;
      string_watch_out_for = ch;
    }
    else if (std::regex_search(&ch, std::regex("[0-9]")) && now_in != STR && now_in != ID) {
      now_in = INT;
      current_token_contents.push_back(ch);
    }
    else if (ch == '$') {
      if (current_token_contents != "") {
        this_token = make_token(now_in, current_token_contents);
        now_in = NONE;
        to_return.push_back(this_token);
      }
      current_token_contents = "";
      this_token = DotIDLiteralSign();
      to_return.push_back(this_token);
    }
    else if (ch == '{') {
      if (current_token_contents != "") {
        this_token = make_token(now_in, current_token_contents);
        now_in = NONE;
        to_return.push_back(this_token);
      }

      current_token_contents = "";
      this_token = DotLBrace();
      to_return.push_back(this_token);
    }
    else if (ch == '}') {
      if (current_token_contents != "") {
        this_token = make_token(now_in, current_token_contents);
        now_in = NONE;
        to_return.push_back(this_token);
      }

      current_token_contents = "";
      this_token = DotRBrace();
      to_return.push_back(this_token);
    }
    else if (ch == ',') {
      if (current_token_contents != "") {
        this_token = make_token(now_in, current_token_contents);
        now_in = NONE;
        to_return.push_back(this_token);
      }

      current_token_contents = "";
      this_token = DotComma();
      to_return.push_back(this_token);
    }
    else {
      std::string suspect = "this shouldn't be printed; there was an internal error";

      if (ch < ' ') {
        suspect = "char code DEC " + std::to_string((int) ch);
      }
      else {
        suspect = std::string(1, ch);
      }

      std::string message = "Unknown or unexpected character <" + suspect + ">";
      throw std::runtime_error(message);
    }
  }

  if (now_in == STR) {
    throw std::runtime_error(std::string("Unclosed string: ") + current_token_contents);
  }

  return to_return;
}

DotToken make_token(State now_in, std::string current_token_contents) {
  if(now_in == STR)  return DotString(current_token_contents);
  if(now_in == INT)  return DotInt(current_token_contents);
  if(now_in == ID)   return DotIdentifier(current_token_contents);
  if(now_in == NONE) return DotString("");
  else throw std::runtime_error("Internal error: Illegal state: " + std::to_string(now_in));
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
  std::vector<DotToken> lexed = lex("print(\"A string with bad '\")");
  std::cout << "Lexed successfully. Entered testing.\n\n";
  return lex_to_str(lexed);
}

#endif

#ifdef BENCHMARK

int main() {
  std::ifstream file;
    file.open("lexerbenchmark.shmr");

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