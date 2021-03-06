#ifndef ERRORS_H
#define ERRORS_H

#include <cstdarg>
#include <stdexcept>
#include <string>

class BaseError : public std::runtime_error {
  public:
    BaseError(std::string what_, std::string line_);
    BaseError(std::string what_, int line_);
    BaseError(std::string what1, std::string what2, std::string line_);
    BaseError(std::string what1, std::string what2, int line_);

    std::string m_what;
    std::string what();

    int m_line;
    int line();
    std::string line_as_string();

  private:
    std::string construct_helper(std::string what_, std::string line_);
    std::string construct_helper(std::string what_, int line_);
    std::string construct_helper(std::string what1, std::string what2, std::string line_);
    std::string construct_helper(std::string what1, std::string what2, int line_);
};

class KeyboardInterruptError : public BaseError {};
class MissingTokenError : public BaseError {};
class UnclosedPairError : public MissingTokenError {};
class UnclosedParenError : public UnclosedPairError {};
class UnclosedBraceError : public UnclosedPairError {};
class UnclosedStringError : public UnclosedPairError {};
class MissingCommaError : public MissingTokenError {};
class MissingIDLSError : public MissingTokenError {};
class UnexpectedTokenError : public BaseError {};
class ExtraClosedPairError : public UnexpectedTokenError {};
class ExtraClosedParenError : public ExtraClosedPairError {};
class ExtraClosedBraceError : public ExtraClosedPairError {};
class ExtraClosedStringError : public ExtraClosedPairError {};
class UnexpectedCommaError : public UnexpectedTokenError {};
class UnexpectedIDLSError : public UnexpectedTokenError {};

void throw_error(std::string line, std::string msg);
void throw_error(std::string line, std::string msg, std::string msg2);
void throw_error(int line, std::string msg);
void throw_error(int line, std::string msg, std::string msg2);

void _throw_error(int line, std::string format, ...);
void _throw_error(std::string line, std::string format, ...);

#endif
