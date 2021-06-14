#ifndef PROMPT_H
#define PROMPT_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#  warning You're using Windows?? Seriously??
#  warning You're gonna be missing some FEATURES then...
#endif

#if defined(__linux__) || defined(__unix__) || defined(__linux) || defined(__unix) || defined(_POSIX_VERSION)
#  define USE_LINENOISE
#endif

char* prompt(const char* text);

#endif
