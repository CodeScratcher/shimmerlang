#include <vector>
#include "ShimmerClasses.h"

DotTree parse(std::vector<DotToken>);
void print_debug_info(DotTree x);
const char* param_recursive_str(ShimmerParam to_convert);