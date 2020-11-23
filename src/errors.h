#include <stdexcept>

void throw_error(std::string msg, std::string line);
void throw_error(std::string msg, std::string msg2, std::string line);
void throw_error(std::string msg, int line);
void throw_error(std::string msg, std::string msg2, int line);