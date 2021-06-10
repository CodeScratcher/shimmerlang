# Shimmer Source Code
This is the folder containing the source code 
for the Shimmer programming language.

## errors
Contains classes for custom exceptions which make error throwing simpler.
Also contains functions for throwing exceptions.

## eval
Takes a node tree and evaluates it. 
**NOTE: actual work happens in ShimmerClasses.cpp, in function `ShimmerStatement::eval`**

## lexer
Takes a program and turns it into a sequence of tokens.

## parser
Takes a sequence of tokens and turns it into a node tree.

## ShimmerClasses
Contains various classes used throughout the project to make the code cleaner.

## shimmerlang.cpp
The main program that starts off processing and program 
flow and also delegates jobs down to the other files. 
**NOTE: this file is called by [main.sh (repl.it)] / [main.sh (github.com)].**

## text_effects.h
The header with macros for printing colored and styled text. 
**NOTE: Works best on Unix-based systems.**

## tree_print
Utilities for pretty-printing trees, statements, and function parameters. 
Trees are printed sideways (directory-style).

[main.sh (repl.it)]: https://repl.it/@skiscratcher/shimmerlang#main.sh
[main.sh (github.com)]: https://github.com/CodeScratcher/shimmerlang/blob/master/main.sh
