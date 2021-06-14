# Shimmer
This is the source code for the `shimmer`
programming language.
See src/src.md for more details.

## About
Shimmer is a language that revolves around functions.
Almost everything is a function in Shimmer - even operators!
Consider the following:
```python
print("Hello, world!\n")
print(add(sub(10, mul(3, div(8, 2))), 5)) ; 10 - (3 * 8 / 2) + 5

*#
 # This is a block comment
 # It starts with *# and ends with # and *
 # ;;; Line comment inside block comment
 #*

;; ^ End of block comment (I am a line comment BTW)
```

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
