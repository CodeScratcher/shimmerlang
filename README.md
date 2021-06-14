# Shimmer
This is the source code for the `shimmer`
programming language.
See src/src.md for more details.

## About
TODO

## TODO
TODO

## Building
The main.sh file should handle it.
If you want to do it manually, try this:
```shell
$ cmake -DBUILD_NUMBER="x.y" src
$ build/shimmerlang
```

## Debugging
Using `gdb`, type in the commands
`break main`, `run`,
and then the debugging function.
* Type in `call lex_test()` to test the lexer.
* Type in `call parse_test()` to test the parser.
