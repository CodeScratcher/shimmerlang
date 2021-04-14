#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cctype>

#define DEBUG
#if defined BENCHMARK || defined DEBUG
#include <fstream>
#include <sstream>
#endif

#include "ShimmerClasses.h"
#include "errors.h"
#include "text_effects.h"
#include "lexer.h"

/*
std::vector<ShimmerToken> lex(std::string str) {
  std::vector<ShimmerToken> to_return;
  ShimmerToken this_token;

  std::string current_token_contents = "";

  chtype string_watch_out_for;
  State now_in = NONE;
  int current_line = 1;

  for (int i = 0; i < str.length(); i++) {
    chtype ch = str.at(i);

    if (now_in == NONE) {
      if (ch == '/') {
        now_in = CMNT1;
        continue;
      }
      else if (ch == '"' || ch == '\'') {
        now_in = STR;
        string_watch_out_for = ch;
        continue;
      }
      else if (std::regex_search(std::string(1, ch), std::regex("[a-zA-Z_]"))) {
        std::cout << "FOUND IDENTIFIER!!!\n";
        now_in = ID;
        current_token_contents.push_back(ch);
        continue;
      }
    }
    else if (now_in == CMNT1) {
      if (ch == '*') {
        now_in = CMNT2;
        continue;
      }
      else {
        throw_error("Missing asterisk at comment starting delimiter.", current_line);
      }
    }
    else if (now_in == CMNT2) {
      if (ch == '*') {
        now_in = CMNT3;
        continue;
      }
    }
    else if (now_in == CMNT3) {
      if (ch == '/') {
        now_in = NONE;
        continue;
      }
      else {
        throw_error("Missing slash at comment ending delimiter.", current_line);
      }
    }
    else if (now_in == STR) {
      if (ch == string_watch_out_for) {
        this_token = make_token(now_in, current_token_contents, current_line);
        now_in = NONE;
        current_token_contents = "";
        to_return.push_back(this_token);
      }
      else if (ch == '\\') {
        current_token_contents.push_back(esc_seq(str.at(++i)));
      }
      else {
        current_token_contents.push_back(ch);
      }
    }
    else if (ch == ' ' || ch == '\t' || ch == '\n') {
      if (ch == '\n') {
        ++current_line;
      }

      if (current_token_contents != " ") {
        now_in = NONE;
        this_token = make_token(now_in, current_token_contents, current_line);
        to_return.push_back(this_token);
      }

      current_token_contents = "";
      continue;
    }
    else if (ch == '(') {
      
    }
    else if (now_in == ID) {
      std::cout << "CONTINUING IDENTIFIER!!!\n";
      if (std::regex_search(std::string(1, ch), std::regex("[a-zA-Z0-9_]"))) {
        current_token_contents.push_back(ch);
      }
    }
    else {
      now_in = NONE;

      if (current_token_contents != "") {
        this_token = make_token(now_in, current_token_contents, current_line);
        now_in = NONE;
        to_return.push_back(this_token);
      }

      current_token_contents = "";
      continue;
    }

    // if (ch == '\'' || ch == '"') {
    //   if (now_in == NONE) {
    //     now_in = STR;
    //     string_watch_out_for = ch;
    //   }
    //   else if (now_in == STR && ch == string_watch_out_for) {

    //     this_token = make_token(now_in, current_token_contents, current_line);
    //     now_in = NONE;
    //     current_token_contents = "";
    //     to_return.push_back(this_token);
    //   }
    //   else {
    //     current_token_contents.push_back(ch);
    //   }
    // }
    // else if (ch == '(') {
    //   if (current_token_contents != "") {
    //     this_token = make_token(now_in, current_token_contents, current_line);
    //     now_in = NONE;
    //     to_return.push_back(this_token);
    //   }

    //   current_token_contents = "";
    //   this_token = ShimmerLParen(current_line);
    //   to_return.push_back(this_token);
    // }
    // else if (ch == ')') {
    //   if (current_token_contents != "") {
    //     this_token = make_token(now_in, current_token_contents, current_line);
    //     now_in = NONE;
    //     to_return.push_back(this_token);
    //   }
  
    //   current_token_contents = "";
    //   this_token = ShimmerRParen(current_line);
    //   to_return.push_back(this_token);
    // }
    // else if (std::regex_search(std::string(1, ch), std::regex("[a-zA-Z_]")) && now_in == NONE) {
    //   now_in = ID;
    //   current_token_contents.push_back(ch);
    // }
    // else if (std::regex_search(std::string(1, ch), std::regex("[a-zA-Z0-9_]")) && now_in == ID) {
    //   //std::cout << "Continuing identifier with char: " << ch << "\n";
    //   current_token_contents.push_back(ch);
    //   //std::cout << "Current contents: " << current_token_contents << "\n";
    // }
    // else if (std::regex_search(std::string(1, ch), std::regex("[0-9]")) && now_in != STR && now_in != ID) {
    //   now_in = INT;
    //   current_token_contents.push_back(ch);
    // }
    // else if (ch == '$') {
    //   if (current_token_contents != "") {
    //     this_token = make_token(now_in, current_token_contents, current_line);
    //     now_in = NONE;
    //     to_return.push_back(this_token);
    //   }

    //   current_token_contents = "";
    //   this_token = ShimmerIDLiteralSign(current_line);
    //   to_return.push_back(this_token);
    // }
    // else if (ch == '{') {
    //   if (current_token_contents != "") {
    //     this_token = make_token(now_in, current_token_contents, current_line);
    //     now_in = NONE;
    //     to_return.push_back(this_token);
    //   }

    //   current_token_contents = "";
    //   this_token = ShimmerLBrace(current_line);
    //   to_return.push_back(this_token);
    // }
    // else if (ch == '}') {
    //   if (current_token_contents != "") {
    //     this_token = make_token(now_in, current_token_contents, current_line);
    //     now_in = NONE;
    //     to_return.push_back(this_token);
    //   }

    //   current_token_contents = "";
    //   this_token = ShimmerRBrace(current_line);
    //   to_return.push_back(this_token);
    // }
    // else if (ch == ',') {
    //   if (current_token_contents != "") {
    //     this_token = make_token(now_in, current_token_contents, current_line);
    //     now_in = NONE;
    //     to_return.push_back(this_token);
    //   }

    //   current_token_contents = "";
    //   this_token = ShimmerComma(current_line);
    //   to_return.push_back(this_token);
    // }
    // else {
    //   std::string suspect;

    //   if (ch < ' ') {
    //     suspect = "char code in DEC: \"" + std::to_string((int) ch) + "\"";
    //   }
    //   else {
    //     suspect = "\"" + std::string(1, ch) + "\"";
    //   }

    //   throw_error("Unknown or unexpected character: ", suspect, current_line);
    // }
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
    throw_error("Missing slash at comment ending delimiter.", current_line);
  }

  lex_to_str(to_return);

  return to_return;
}*/

std::vector<ShimmerToken> lex(std::string str) {
  str.push_back(' '); // For closing tokens

  int current_line = 1;
  LexerState now_in = NONE;
  chtype str_delim;

  ShimmerToken this_token;
  std::string this_token_contents;
  std::vector<ShimmerToken> tokens;

  for (long i = 0; i < str.length(); i++) {
    std::cout << "Iteration #" << std::to_string(i) << ", ";
    std::cout << "current_line == " << std::to_string(current_line) << ".\n";

    chtype ch = str.at(i);

    if (now_in == UNKNOWN) {
      std::string suspect;

      if (!isprint(ch)) {
        suspect = "char code in DEC: \"" + std::to_string((int) ch) + "\"";
      }
      else {
        suspect = "\"" + std::string(1, ch) + "\"";
      }

      throw_error("Unknown or unexpected character: ", suspect, current_line);
    }
    else if (now_in == COMMENT) {
      if (ch == '\n') {
        current_line++;
        continue;
      }
    }
    else if (now_in == NONE) {
      if (isspace(ch)) {
        continue;
      }
      else if (ch == ';') {
        std::cout << "Found one-line comment!\n";
        now_in = COMMENT;
      }
      else if (ch == '"' || ch == '\'') {
        std::cout << "Now starting a string with delimiter: " << ch << "\n";
        now_in = STR;
        str_delim = ch;
      }
      else if (std::regex_search(std::string(1, ch), std::regex("[a-zA-Z_\\-]"))) {
        std::cout << "Now starting an identifier with char: " << ch << "\n";
        now_in = ID;
        this_token_contents.push_back(ch);
      }
      else if (std::regex_search(std::string(1, ch), std::regex("[0-9]"))) { // TODO: add hexadecimal support
        std::cout << "Now starting an int with char: " << ch << "\n";
        now_in = INT;
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
      std::cout << "Continuing string with char: " << ch << "\n";
      if (ch == str_delim) {
        std::cout << "Found delimeter " << ch << " for string " << this_token_contents << "\n";
        now_in = NONE;

        tokens.push_back(ShimmerString(current_line, this_token_contents));
      }
      else {
        this_token_contents.push_back(ch);
      }
    }
    else if (now_in == ID) {
      if (std::regex_search(std::string(1, ch), std::regex("[a-zA-Z0-9_\\-]"))) {
        std::cout << "Continuing identifier with char: " << ch << "\n";
        this_token_contents.push_back(ch);
        std::cout << "Current contents: " << this_token_contents << "\n";
      }
      else {
        tokens.push_back(ShimmerIdentifier(current_line, this_token_contents));
        this_token_contents = "";

        now_in = NONE;
        i--; // Rescan character to start new token
        continue;
      }
    }
    else if (now_in == INT) {
      if (std::regex_search(std::string(1, ch), std::regex("[0-9]"))) {
        std::cout << "Continuing int with char: " << ch << "\n";
        this_token_contents.push_back(ch);
        std::cout << "Current contents: " << this_token_contents << "\n";
      }
      else {
        tokens.push_back(ShimmerInt(current_line, this_token_contents));
        this_token_contents = "";

        now_in = NONE;
        i--; // Rescan character to start new token
        continue;
      }
    }
    else if (isspace(ch)) {
      if (now_in == ID) {
        tokens.push_back(ShimmerIdentifier(current_line, this_token_contents));
        this_token_contents = "";
      }
      else if (now_in == INT) {
        tokens.push_back(ShimmerInt(current_line, this_token_contents));
        this_token_contents = "";
      }

      now_in = NONE;
      i--;
    }
    else {
      now_in = UNKNOWN;
      i--;
      continue;
    }
  }

  if (now_in == STR) {
    throw_error("Unclosed string: ", this_token_contents, current_line);
  }

  lex_to_str(tokens);

  return tokens;
}

ShimmerToken make_token(LexerState now_in, std::string current_token_contents, int current_line) {
  if (now_in == STR)   return ShimmerString(current_line, current_token_contents);
  if (now_in == INT)   return ShimmerInt(current_line, current_token_contents);
  if (now_in == ID)    return ShimmerIdentifier(current_line, current_token_contents);
  if (now_in == NONE)  return ShimmerString(current_line, "");
  throw_error("Internal error: Illegal state: ", str_repr(now_in), current_line);
}

std::string str_repr(LexerState ls) {
  switch (ls) {
    case NONE: return "NONE";
    case ID:   return "ID";
    case INT:  return "INT";
    case STR:  return "STR";
    default:   throw_error(
      "Internal error: Cannot find str repr of illegal lexer state: ", 
      std::to_string(ls)
    );
  }
}

chtype esc_seq(chtype ch) {
  switch (ch) {
    case 'n':  return '\n';
    case 'r':  return '\r';
    case '\\': return '\\';
    case 't':  return '\t';
    case 'b':  return '\b';
    case 'f':  return '\f';
    case 'v':  return '\v';
    case 'a':  return '\a';
    case '\'': return '\'';
    case '"':  return '"';
    default:   throw_error("Invalid escape sequence.", -1);
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
