# Shimmer
To learn about the source code, see src/src.md.

NOTE: since Shimmer is a WIP, some things described by this file
may be buggy, in the todo stage, nonexistent, or not completely accurate.

We will try to update this file as often as possible.
For now, anything crossed out means definitely WIP.

## About
### Introduction
Shimmer is a language that revolves around functions.
Almost everything is a function in Shimmer - even operators!
Consider the following:
```
print("Hello, world!\n")
print(add(sub(10, mul(3, div(8, 2))), 5)) ; 10 - (3 * 8 / 2) + 5
```

### Syntax
Except for comments, everything in Shimmer is of the form:
```
func_name(optional_params)(optional_args)
```
Note that the `(optional_args)` part is optional:
```
func_name(optional_params)
```
would also be valid.

#### Identifiers
Using the above example, `func_name` must be an identifier.
An identifier is simply a token, possibly preceded by `$` (the ID literal sign),
that contains only characters from `[a-zA-Z_\-]` (alphanumerical characters, underscores, and dashes)
but may only start with anything characters from `[a-zA-Z_]` (same as before but no numbers and dashes).

#### Literals
`optional_params` and `optional_args`, if present, may be either identifiers or literals.
A literal is any of the following:

* strings: `"Hello, world!\n"` - delimited by single `'` or double `"` quotes
* numbers: `123`, `-1`, `0`, `98.76` - characters from `[\-0-9.]` (minus signs, numbers, periods)
  * numbers may contain only one period each
* booleans: `true`, `false` - self-explanatory, I hope
* functions: `(args) {body}` - `args` may be identifiers or literals, body is composed of 1 or more statements

#### Comments
Block comments start with `*#` and end with `#*`, no nesting.
Line comments start with `;` (semicolons).
Block comments may contain line comments, and vice versa if the block comment is on one line.
Line comments may contain block comment delimiters.

```
*#
Block comment
#*

; Line comment

*#
; Line comment in block comment
#*

*# Block comments can be one line #*

; *# One-line block comment in line comment #*
```

### Functions
Shimmer does not have many built-in functions, with the intention that
one must either write functions using what they have, or import some library
functions. For example, Shimmer has no `if()` function because its
functionality already exists, in the form of the `if_else()` function.

Also, Shimmer lacks loop constructs such as `while`, `for`, and `do while`.
This is because you can use recursion instead. Compare this C code:
```c
for (int i = 0; i < 10; ++i) {
  printf("Hello, world!\n");
}
```
with the equivalent Shimmer code:
```
def(
  $loop,
  (i, max) {
    if_else(
      lt(i, max),      ; ... i < 10 ...
      () {
        print("Hello, world!\n")   ; Same as previous program
        loop(add(i, 1))            ; This is the ++i part
      },
      () {}
    )()
  }
)

loop(1, 10) ; for (int i = 0; i < 10; ...
```
You can use this concept to create any program. However, it sometimes
helps to solve something iteratively, ~~so there is a library that adds loops~~.

Here's a (growing) list of the functions in Shimmer.
* `print(...)`
  * prints all of its arguments and nothing more
* `log(...)` - may be renamed in the future
  * prints all of its arguments separated by spaces, with a newline at the end
* `read(file?)`
  * returns contents of `file` if it exists or one line from `stdin` if called with no arguments.
* `write(file, contents)` - may be merged with `print()` in the future
  * creates `file` if nonexistent, then replaces the file contents (if any) with `contents`
* `append(file, contents)`
  * creates `file` if nonexistent, then appends `contents` to the end of the file
* `add(...)`
  * returns sum of arguments
* `sub(x, y)`
  * returns `x - y`
* `mul(...)`
  * returns product of arguments
* `div(x, y)`
  * returns `x / y` or throws an error if `y == 0`
* `and(...)`
  * ~~returns `true` if all arguments are truthy and `false` otherwise~~
* `or(...)`
  * ~~returns `false` if all arguments are falsy and `true` otherwise~~
* `not(x)`
  * returns `true` if `x` is falsy and `false` otherwise
* `lt(x, y)`
  * returns `true` if `x < y` and `false` otherwise
* `gt(x, y)`
  * returns `true` if `x > y` and `false` otherwise
* `eq(x, y)`
  * returns `true` if `x == y` and `false` otherwise
* `num(x)`
  * returns the number form of `x`
* `str(x)`
  * returns the string form of `x`
* `bool(x)`
  * returns the bool form of `x`
* `get_char(x, i)`
  * returns the `i`th character of `x`
* `set_char(x, i, c)`
  * sets the `i`th character of `x` to `c`
* `insert(x, i, s)`
  * inserts string `s` into `x` at position `i`
* `remove(x, i, n)`
  * removes `n` characters from `x` starting from position `i`
* `concat(x, y)`
  * returns `x` and `y` joined together
* `slice(x, i, j?)`
  * returns chracters in `x` from position `i` to `j` (or the end of `x`, if `j` is not given)
* `def(id, val)`
  * creates a variable called `id` with the value `val`
* `set(id, val)`
  * sets the variable `id` to have the value `val`
* `get(id)`
  * returns the value referenced by `id`
    * `def($foo, 5) def($bar, foo) def($baz, $foo)`
      * then `eq(foo, 5)` and `eq(bar, 5)` but `eq(get(baz), 5)`
      * since `baz` **refers** to `foo`
* `if_else(cond, if_true, if_false)`
  * returns `if_true` if `cond` is truthy and `if_false` otherwise
* `import(lib)`
  * evaluates the contents of file `lib` and then `cons`es its scope onto the current scope

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
