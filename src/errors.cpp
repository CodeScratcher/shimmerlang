#include <stdexcept>
#include "errors.h"

typedef enum {
  TODO // TODO
} Error;

void throw_error(std::string msg, std::string line) {
  throw std::runtime_error(line + ":\n\t" + msg);
}

void throw_error(std::string msg, std::string msg2, std::string line) {
  throw std::runtime_error(line + ":\n\t" + msg + msg2);
}

void throw_error(std::string msg, int line) {
  throw std::runtime_error(std::to_string(line) + ":\n\t" + msg);
}

void throw_error(std::string msg, std::string msg2, int line) {
  throw std::runtime_error(std::to_string(line) + ":\n\t" + msg + msg2);
}