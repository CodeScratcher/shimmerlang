#include <stdexcept>
#include <string>
#include "errors.h"

BaseError::BaseError(std::string what_, std::string line_)
                     : std::runtime_error {construct_helper(what_, line_)} {
  // Empty
}

BaseError::BaseError(std::string what_, int line_)
                     : std::runtime_error {construct_helper(what_, line_)} {
  // Empty
}

BaseError::BaseError(std::string what1, std::string what2, std::string line_)
                     : std::runtime_error {construct_helper(what1, what2, line_)} {
  // Empty
}

BaseError::BaseError(std::string what1, std::string what2, int line_)
                     : std::runtime_error {construct_helper(what1, what2, line_)} {
  // Empty
}

std::string BaseError::what() {
  return m_what;
}

std::string BaseError::line_as_string() {
  return std::to_string(m_line);
}

int BaseError::line() {
  return m_line;
}

std::string BaseError::construct_helper(std::string what_, std::string line_) {
  m_line = std::stoi(line_);
  m_what = line_ + ":\n\t" + what_;

  return m_what;
}

std::string BaseError::construct_helper(std::string what_, int line_) {
  m_line = line_;
  m_what = line_as_string() + ":\n\t" + what_;

  return m_what;
}

std::string BaseError::construct_helper(std::string what1, std::string what2, 
                                        std::string line_) {
  m_line = std::stoi(line_);
  m_what = line_ + ":\n\t" + what1 + what2;

  return m_what;
}

std::string BaseError::construct_helper(std::string what1, std::string what2, int line_) {
  m_line = line_;
  m_what = line_as_string() + ":\n\t" + what1 + what2;

  return m_what;
}

void throw_error(std::string line, std::string msg) {
  throw std::runtime_error(line + ":\n\t" + msg);
}

void throw_error(std::string line, std::string msg, std::string msg2) {
  throw std::runtime_error(line + ":\n\t" + msg + msg2);
}

void throw_error(int line, std::string msg) {
  throw std::runtime_error(std::to_string(line) + ":\n\t" + msg);
}

void throw_error(int line, std::string msg, std::string msg2) {
  throw std::runtime_error(std::to_string(line) + ":\n\t" + msg + msg2);
}