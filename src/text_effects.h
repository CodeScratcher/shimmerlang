#ifndef TEXT_EFFECTS_H
#define TEXT_EFFECTS_H

#if defined(unix) || defined(__unix__) || defined(__unix)

#define STYLE_CLEAR      "\x1b[0m"
#define STYLE_BOLD       "\x1b[1m"
#define STYLE_UNDERLINED "\x1b[4m"
#define STYLE_RED        "\x1b[31m"
#define STYLE_GREEN      "\x1b[32m"
#define STYLE_ORANGE     "\x1b[33m"
#define STYLE_YELLOW     "\x1b[93m"
#define STYLE_BLUE       "\x1b[34m"
#define STYLE_MAGENTA    "\x1b[35m"
#define STYLE_LAVENDER   "\x1b[95m"
#define STYLE_CYAN       "\x1b[36m"
#define STYLE_L_RED      "\x1b[91m"
#define STYLE_L_GREEN    "\x1b[92m"
#define STYLE_L_BLUE     "\x1b[94m"
#define STYLE_L_CYAN     "\x1b[96m"

#define BOLD(str)       STYLE_BOLD       << str << STYLE_CLEAR
#define UNDERLINED(str) STYLE_UNDERLINED << str << STYLE_CLEAR
#define RED(str)        STYLE_RED        << str << STYLE_CLEAR
#define GREEN(str)      STYLE_GREEN      << str << STYLE_CLEAR
#define ORANGE(str)     STYLE_ORANGE     << str << STYLE_CLEAR
#define YELLOW(str)     STYLE_YELLOW     << str << STYLE_CLEAR
#define BLUE(str)       STYLE_BLUE       << str << STYLE_CLEAR
#define MAGENTA(str)    STYLE_MAGENTA    << str << STYLE_CLEAR
#define CYAN(str)       STYLE_CYAN       << str << STYLE_CLEAR
#define L_RED(str)      STYLE_L_RED      << str << STYLE_CLEAR
#define L_GREEN(str)    STYLE_L_GREEN    << str << STYLE_CLEAR
#define L_BLUE(str)     STYLE_L_BLUE     << str << STYLE_CLEAR
#define L_MAGENTA(str)  STYLE_L_MAGENTA  << str << STYLE_CLEAR
#define L_CYAN(str)     STYLE_L_CYAN     << str << STYLE_CLEAR

#else

#define BOLD(str)       str
#define UNDERLINED(str) str
#define RED(str)        str
#define GREEN(str)      str
#define ORANGE(str)     str
#define YELLOW(str)     str
#define BLUE(str)       str
#define MAGENTA(str)    str
#define CYAN(str)       str
#define L_RED(str)      str
#define L_GREEN(str)    str
#define L_BLUE(str)     str
#define L_MAGENTA(str)  str
#define L_CYAN(str)     str

#endif

#endif