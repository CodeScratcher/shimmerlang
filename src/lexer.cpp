#include <cctype>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#define DEBUG
#if defined BENCHMARK || defined DEBUG
#include <fstream>
#include <sstream>
#endif

#include "ShimmerClasses.h"
#include "errors.h"
#include "lexer.h"
#include "text_effects.h"
#include "utility.h"

std::vector<ShimmerToken> lex(std::string str) {
  str.push_back(' '); // For closing tokens at the end

  int current_line = 1;
  LexerState now_in = NONE;
  chtype str_delim;
  int block_comment_start = -1;

  ShimmerToken this_token;
  std::string this_token_contents;
  std::vector<ShimmerToken> tokens;

  for (std::string::size_type i = 0; i < str.length(); i++) {
    chtype ch = str.at(i);

    if (now_in == UNKNOWN) {
      std::string suspect;

      if (!isprint(ch)) {
        suspect = "char code in DEC: \"" + std::to_string((int) ch) + "\"";
      }
      else {
        suspect = "\"" + std::string(1, ch) + "\"";
      }

      throw_error(current_line, "Unknown or unexpected character: ", suspect);
    }
    else if (now_in == COMMENT) {
      if (ch == '\n') {
        current_line++;
        this_token_contents = "";
        now_in = NONE;
        continue;
      }
    }
    else if (now_in == BLCKCMNT_1) {
      this_token_contents = "";

      if (ch != '#') {
        throw_error(current_line, "Expected '#' character at block comment start delimiter");
      }
      else {
        now_in = BLCKCMNT_2;
      }
    }
    else if (now_in == BLCKCMNT_2) {
      if (ch == '\n') {
        current_line++;
      }
      else if (ch == '#') {
        now_in = BLCKCMNT_3;
      }
    }
    else if (now_in == BLCKCMNT_3) {
      if (ch == '*') {
        now_in = NONE;
      }
      else {
        now_in = BLCKCMNT_2;
      }
    }
    else if (now_in == NONE) {
      if (isspace(ch)) {
        if (ch == '\n') {
          current_line++;
        }

        continue;
      }
      else if (ch == ';') {
        now_in = COMMENT;
      }
      else if (ch == '*') {
        now_in = BLCKCMNT_1;
        block_comment_start = current_line;
      }
      else if (ch == '"' || ch == '\'') {
        now_in = STR;
        str_delim = ch;
      }
      else if (std::regex_search(std::string(1, ch), std::regex("[a-zA-Z_\\-]"))) {
        now_in = ID;
        this_token_contents.push_back(ch);
      }
      else if (std::regex_search(std::string(1, ch), std::regex("[0-9.]"))) {
        now_in = NUMBER;
        this_token_contents.push_back(ch);
      }
      else if (ch == '(') {
        tokens.push_back(ShimmerLParen(current_line));
        this_token_contents = "";
      }
      else if (ch == ')') {
        tokens.push_back(ShimmerRParen(current_line));
        this_token_contents = "";
      }
      else if (ch == '{') {
        tokens.push_back(ShimmerLBrace(current_line));
        this_token_contents = "";
      }
      else if (ch == '}') {
        tokens.push_back(ShimmerRBrace(current_line));
        this_token_contents = "";
      }
      else if (ch == ',') {
        tokens.push_back(ShimmerComma(current_line));
        this_token_contents = "";
      }
      else if (ch == '$') {
        tokens.push_back(ShimmerIDLiteralSign(current_line));
        this_token_contents = "";
      }
      else {
        now_in = UNKNOWN;
        i--; // Reparse the character as an unknown character
        continue;
      }
    }
    else if (now_in == STR) {
      if (ch == str_delim) {
        now_in = NONE;

        tokens.push_back(ShimmerString(current_line, this_token_contents));
      }
      else if (ch == '\\') {
        this_token_contents.push_back(escape(str.at(++i)));
        continue;
      }
      else {
        this_token_contents.push_back(ch);
      }
    }
    else if (now_in == ID) {
      if (std::regex_search(std::string(1, ch), std::regex("[a-zA-Z0-9_\\-]"))) {
        this_token_contents.push_back(ch);
      }
      else {
        tokens.push_back(ShimmerIdentifier(current_line, this_token_contents));
        this_token_contents = "";

        now_in = NONE;
        i--; // Rescan character to start new token
        continue;
      }
    }
    else if (now_in == NUMBER) {
      if (std::regex_search(std::string(1, ch), std::regex("[0-9.]"))) {
        this_token_contents.push_back(ch);
      }
      else {
        tokens.push_back(ShimmerNumber(current_line, this_token_contents));
        this_token_contents = "";

        now_in = NONE;
        i--; // Rescan character to start new token
        continue;
      }
    }
    else if (isspace(ch)) {
      if (ch == '\n') {
        current_line++;
      }

      if (now_in == ID) {
        tokens.push_back(ShimmerIdentifier(current_line, this_token_contents));
        this_token_contents = "";
      }
      else if (now_in == NUMBER) {
        tokens.push_back(ShimmerNumber(current_line, this_token_contents));
        this_token_contents = "";
      }

      now_in = NONE;
      i--;
      continue;
    }
    else {
      now_in = UNKNOWN;
      i--;
      continue;
    }
  }

  if (now_in == STR) {
    throw_error(current_line, "Unclosed string: ", this_token_contents);
  }
  else if (now_in == BLCKCMNT_2) {
    throw_error(block_comment_start, "Unclosed block comment");
  }

  return tokens;
}

std::string str_repr(LexerState ls) {
  switch (ls) {
    case NONE:   return "NONE";
    case ID:     return "ID";
    case NUMBER: return "NUMBER";
    case STR:    return "STR";
    default:     throw_error(
      -1,
      "Internal error: Cannot find str repr of illegal lexer state: ", 
      std::to_string(ls)
    );
  }
}

#ifdef DEBUG

const char* lex_to_str(std::vector<ShimmerToken> lexed) {
  int contents_length = 16, type_length = 24;

  std::cout << tc::bold;
  printf("       %-*s%-*s%s\n", contents_length, "Contents", type_length, "Type", "Line");
  std::cout << tc::reset;

  for (ShimmerToken i : lexed) {
    std::string token_contents = i.get_contents();
    std::string token_type = i.get_token_type();
    int token_line = i.get_line();

    token_contents = unescape(token_contents);

    printf(
      "       %-*s%-*s%d\n", 
      contents_length, token_contents.c_str(),
      type_length, token_type.c_str(), 
      token_line
    );
  }

  return "";
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

  return 0;
}

#endif
