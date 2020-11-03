#include <vector>
#include <string>
#include "ShimmerClasses.h"

typedef enum {NONE, ID, INT, STR} State;

std::vector<DotToken> lex(std::string str);
