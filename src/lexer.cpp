#include <iostream>
#include <string>
#include <vector>
#include <regex>
#define DEBUG
#if defined BENCHMARK || defined DEBUG
#include <fstream>
#include <sstream>
#endif
#include "ShimmerClasses.h"
#include "errors.h"
#include "text_effects.h"
#include "lexer.h"

std::vector<ShimmerToken> lex(std::string str) {
  std::vector<ShimmerToken> to_return;
  ShimmerToken this_token;
  std::string current_token_contents = "";
  char string_watch_yout_for;
  State now_in = NONE;
  int current_line = 1;

  for (int i = 0; i < str.length(); i++) {
    char ch = str.at(i);

    if (ch == '/') {
      now_in = CMNT1;
      continue;
    }
    else if (now_in == CMNT1) {
      if (ch != '*') {
        throw_error("Missing asterisk at comment starting delimiter.", current_line);
      }

      now_in = CMNT2;
      continue;
    }
    else if (now_in == CMNT2) {
      now_in = CMNT3;
      continue;
    }
    else if (now_in == CMNT3) {
      if (ch != '*') {
        throw_error("Missing asterisk at comment ending delimiter.", current_line);
      }

      now_in = CMNT4;
      continue;
    }
    else if (now_in == CMNT4) {
      if (ch != '/') {
        throw_error("Missing slash at comment ending delimiter.", current_line);
      }

      now_in = NONE;
      continue;
    }
    else if (now_in != STR && (ch == ' ' || ch == '\t' || ch == '\n')) {
      if (ch == '\n') {
        current_line++;
      }

      if (current_token_contents != "") {
        this_token = make_token(now_in, current_token_contents, current_line);
        now_in = NONE;
        to_return.push_back(this_token);
      }

      current_token_contents = "";
      continue;
    }

    if (ch == '\'' || ch == '"') {
      if (now_in == NONE) {
        now_in = STR;
        string_watch_out_for = ch;
      }
      else if (now_in == STR && ch == string_watch_out_for) {
        this_token = make_token(now_in, current_token_contents, current_line);
        now_in = NONE;
        current_token_contents = "";
        to_return.push_back(this_token);
      }
      else {
        current_token_contents.push_back(ch);
      }
    }
    else if (now_in == STR) {
      current_token_contents.push_back(ch);
    }
    else if (ch == '(') {
      if (current_token_contents != "") {
        this_token = make_token(now_in, current_token_contents, current_line);
        now_in = NONE;
        to_return.push_back(this_token);
      }

      current_token_contents = "";
      this_token = ShimmerLParen(current_line);
      to_return.push_back(this_token);
    }
    else if (ch == ')') {
      if (current_token_contents != "") {
        this_token = make_token(now_in, current_token_contents, current_line);
        now_in = NONE;
        to_return.push_back(this_token);
      }
  
      current_token_contents = "";
      this_token = ShimmerRParen(current_line);
      to_return.push_back(this_token);
    }
    else if (std::regex_search(std::string(1, ch), std::regex("[a-zA-Z_]")) && now_in == NONE) {
      now_in = ID;
      current_token_contents.push_back(ch);
    }
    else if (std::regex_search(std::string(1, ch), std::regex("[a-zA-Z0-9_]")) && now_in == ID) {
      //std::cout << "Continuing identifier with char: " << ch << "\n";
      current_token_contents.push_back(ch);
      //std::cout << "Current contents: " << current_token_contents << "\n";
    }
    else if (std::regex_search(std::string(1, ch), std::regex("[0-9]")) && now_in != STR && now_in != ID) {
      now_in = INT;
      current_token_contents.push_back(ch);
    }
    else if (ch == '$') {
      if (current_token_contents != "") {
        this_token = make_token(now_in, current_token_contents, current_line);
        now_in = NONE;
        to_return.push_back(this_token);
      }
      current_token_contents = "";
      this_token = ShimmerIDLiteralSign(current_line);
      to_return.push_back(this_token);
    }
    else if (ch == '{') {
      if (current_token_contents != "") {
        this_token = make_token(now_in, current_token_contents, current_line);
        now_in = NONE;
        to_return.push_back(this_token);
      }

      current_token_contents = "";
      this_token = ShimmerLBrace(current_line);
      to_return.push_back(this_token);
    }
    else if (ch == '}') {
      if (current_token_contents != "") {
        this_token = make_token(now_in, current_token_contents, current_line);
        now_in = NONE;
        to_return.push_back(this_token);
      }

      current_token_contents = "";
      this_token = ShimmerRBrace(current_line);
      to_return.push_back(this_token);
    }
    else if (ch == ',') {
      if (current_token_contents != "") {
        this_token = make_token(now_in, current_token_contents, current_line);
        now_in = NONE;
        to_return.push_back(this_token);
      }

      current_token_contents = "";
      this_token = ShimmerComma(current_line);
      to_return.push_back(this_token);
    }
    else {
      std::string suspect;

      if (ch < ' ') {
        suspect = "char code in DEC: \"" + std::to_string((int) ch) + "\"";
      }
      else {
        suspect = "\"" + std::string(1, ch) + "\"";
      }

      throw_error("Unknown or unexpected character: ", suspect, current_line);
    }
  }

  if (now_in == STR) {
    throw_error("Unclosed string: ", current_token_contents, current_line);
  }
  else if (now_in == CMNT1) {
    throw_error("Missing asterisk at comment starting delimiter.", current_line);
  }
  else if (now_in == CMNT2) {
    throw_error("Missing comment ending delimiter.", current_line);
  }
  else if (now_in == CMNT3) {
    throw_error("Missing asterisk at comment ending delimiter.", current_line);
  }
  else if (now_in == CMNT4) {
    throw_error("Missing slash at comment ending delimiter.", current_line);
  }

  return to_return;
}

ShimmerToken make_token(State now_in, std::string current_token_contents, int current_line) {
  if (now_in == STR)   return ShimmerString(current_line, current_token_contents);
  if (now_in == INT)   return ShimmerInt(current_line, current_token_contents);
  if (now_in == ID)    return ShimmerIdentifier(current_line, current_token_contents);
  if (now_in == NONE)  return ShimmerString(current_line, "");
  throw_error("Internal error: Illegal state: ", str_repr(now_in), current_line);
}

std::string str_repr(State state) {
  switch (state) {
    case NONE:
      return "NONE";

    case ID:
      return "ID";

    case INT:
      return "INT";

    case STR:
      return "STR";

    default:
      throw_error("Internal error: Illegal state: ", std::to_string(state), "unknown");
  }
}

#ifdef DEBUG

const char* lex_to_str(std::vector<ShimmerToken> lexed) {
  std::string str;
  std::cout << tc::bold << "       Name\t\t\tType\t\t\tLine\n" << tc::reset;

  for(ShimmerToken i : lexed) {
    std::string token_contents = i.get_contents();
    std::string token_type = i.get_token_type();
    int token_line = i.get_line();

    int token_contents_length = token_contents.length();
    int token_type_length = token_type.length();

    std::string token_line_str = std::to_string(token_line);

    str.append(token_contents);

    int num_of_tabs;

    std::cout << "\t   " << token_contents;

    num_of_tabs = -((token_contents_length + 8) / 4) + 6;
    for (int i = 0; i < num_of_tabs; i++) {
      std::cout << "\t";
    }

    std::cout << token_type;

    num_of_tabs = -((token_type_length + 8) / 4) + 6;
    for (int i = 0; i < num_of_tabs; i++) {
      std::cout << "\t";
    }

    std::cout << token_line_str << "\n";
  }

  return str.c_str();
}

const char* lex_test() {
  std::ifstream file;
  file.open("test.shmr");

  if (!file) {
    std::cout << "Error while opening file.";
    return "Error while opening file.";
  }
  else {
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::vector<ShimmerToken> lexed = lex(buffer.str());
    std::cout << "Lexed successfully. Entered testing.\n\n";
    return lex_to_str(lexed);
  }
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