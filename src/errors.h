#include <stdexcept>

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
};

class KeyboardInterruptError : public BaseError {
  public:
    KeyboardInterruptError(std::string what_, int line_);
    KeyboardInterruptError(std::string what_, std::string line_);
    KeyboardInterruptError(std::string what1, std::string what2, int line_);
    KeyboardInterruptError(std::string what1, std::string what2, std::string line_);
};

class MissingTokenError : public BaseError {
  public:
    MissingTokenError(std::string what_, int line_);
    MissingTokenError(std::string what_, std::string line_);
    MissingTokenError(std::string what1, std::string what2, int line_);
    MissingTokenError(std::string what1, std::string what2, std::string line_);
};

class UnclosedPairError : public MissingTokenError {
  public:
    UnclosedPairError(std::string what_, int line_);
    UnclosedPairError(std::string what_, std::string line_);
    UnclosedPairError(std::string what1, std::string what2, int line_);
    UnclosedPairError(std::string what1, std::string what2, std::string line_);
}

class UnclosedParenError : public UnclosedPairError {
  public:
    UnclosedParenError(std::string what_, int line_);
    UnclosedParenError(std::string what_, std::string line_);
    UnclosedParenError(std::string what1, std::string what2, int line_);
    UnclosedParenError(std::string what1, std::string what2, std::string line_);
};

class UnclosedBraceError : public UnclosedPairError {
  public:
    UnclosedBraceError(std::string what_, int line_);
    UnclosedBraceError(std::string what_, std::string line_);
    UnclosedBraceError(std::string what1, std::string what2, int line_);
    UnclosedBraceError(std::string what1, std::string what2, std::string line_);
};

class UnclosedStringError : public UnclosedPairError {
  public:
    UnclosedStringError(std::string what_, int line_);
    UnclosedStringError(std::string what_, std::string line_);
    UnclosedStringError(std::string what1, std::string what2, int line_);
    UnclosedStringError(std::string what1, std::string what2, std::string line_);
};

class MissingCommaError : public MissingTokenError {
  public:
    MissingCommaError(std::string what_, int line_);
    MissingCommaError(std::string what_, std::string line_);
    MissingCommaError(std::string what1, std::string what2, int line_);
    MissingCommaError(std::string what1, std::string what2, std::string line_);
};

class MissingIDLSError : public MissingTokenError {
  public:
    MissingIDLSError(std::string what_, int line_);
    MissingIDLSError(std::string what_, std::string line_);
    MissingIDLSError(std::string what1, std::string what2, int line_);
    MissingIDLSError(std::string what1, std::string what2, std::string line_);
};

class UnexpectedTokenError : public BaseError {
  public:
    ExtraTokenError(std::string what_, int line_);
    ExtraTokenError(std::string what_, std::string line_);
    ExtraTokenError(std::string what1, std::string what2, int line_);
    ExtraTokenError(std::string what1, std::string what2, std::string line_);
};

class ExtraClosedPairError : public UnexpectedTokenError {
  public:
    ExtraClosedPairError(std::string what_, int line_);
    ExtraClosedPairError(std::string what_, std::string line_);
    ExtraClosedPairError(std::string what1, std::string what2, int line_);
    ExtraClosedPairError(std::string what1, std::string what2, std::string line_);
};

class ExtraClosedParenError : public ExtraClosedPairError {
  public:
    ExtraClosedParenError(std::string what_, int line_);
    ExtraClosedParenError(std::string what_, std::string line_);
    ExtraClosedParenError(std::string what1, std::string what2, int line_);
    ExtraClosedParenError(std::string what1, std::string what2, std::string line_);
};

class ExtraClosedBraceError : public ExtraClosedPairError {
  public:
    ExtraClosedBraceError(std::string what_, int line_);
    ExtraClosedBraceError(std::string what_, std::string line_);
    ExtraClosedBraceError(std::string what1, std::string what2, int line_);
    ExtraClosedBraceError(std::string what1, std::string what2, std::string line_);
};

class ExtraClosedStringError : public ExtraClosedPairError {
  public:
    ExtraClosedStringError(std::string what_, int line_);
    ExtraClosedStringError(std::string what_, std::string line_);
    ExtraClosedStringError(std::string what1, std::string what2, int line_);
    ExtraClosedStringError(std::string what1, std::string what2, std::string line_);
};

class UnexpectedCommaError : public UnexpectedTokenError {
  public:
    UnexpectedCommaError(std::string what_, int line_);
    UnexpectedCommaError(std::string what_, std::string line_);
    UnexpectedCommaError(std::string what1, std::string what2, int line_);
    UnexpectedCommaError(std::string what1, std::string what2, std::string line_);
};

class UnexpectedIDLSError : public UnexpectedTokenError {
  public:
    UnexpectedIDLSError(std::string what_, int line_);
    UnexpectedIDLSError(std::string what_, std::string line_);
    UnexpectedIDLSError(std::string what1, std::string what2, int line_);
    UnexpectedIDLSError(std::string what1, std::string what2, std::string line_);
};

void throw_error(std::string msg, std::string line);
void throw_error(std::string msg, std::string msg2, std::string line);
void throw_error(std::string msg, int line);
void throw_error(std::string msg, std::string msg2, int line);
