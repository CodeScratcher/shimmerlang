# Shimmer Source Code
This is the folder containing the source code
for the Shimmer programming language.

## eval.cpp
The implementation of the evaluator which takes a node tree and runs it.

## eval.h
The declarations for the evaluator.

## lexer.cpp
The implementation of the lexer which takes a
program and turns it into a sequence of tokens.

## lexer.h
The declarations for the lexer.

## parser.cpp
The implementation of the parser which takes a
sequence of tokens and turns it into a node tree.

## parser.h
The declarations for the parser.

## ShimmerClasses.cpp
The implementation of the various classes used
throughout the REPL to make the code cleaner.

## ShimmerClasses.h
The declarations for ShimmerClasses.cpp.

## shimmerlang.cpp
The main cpp program that starts off processing and program
flow and also delegates jobs down to the other files.
**NOTE: this file is called by [main.sh].**

## text_effects.h
The header with macros for printing colored and styled text.
**NOTE: Only works on Unix-based systems.**

[main.sh]: https://repl.it/@doineednumbers/shimmerlang#main.sh