#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "errors.h"
#include "linenoise.h"
#include "prompt.h"

char* prompt(const char* text) {
#ifdef USE_LINENOISE
  return linenoise(text);
#else
  char* mem = (char*) malloc(151);

  if (mem == NULL || mem == nullptr) {
    throw_error(-1, "Could not allocate memory for command");
    exit(1);
  }

  printf("%s", text);

  if (!fgets(mem, 150, stdin)) {
    throw_error(-1, "There was an error while trying to get the command");
    exit(1);
  }

  mem[strcspn(mem, "\n")] = '\0';
  return mem;
#endif
}
