# Shimmer
This is the source code for the ```shimmer```
programming language.
See src/src.md for more details.

## Building
The main.sh file should contain it.
Run ```make``` and then ```build/shimmerlang```
if you want to do it manually.

## Debugging
Using ```gdb```, type in the commands
```break main```, ```run```,
and then the debugging function.
* Type in ```call lex_test()``` to test the lexer.
* Type in ```call parse_test()``` to test the parser.
