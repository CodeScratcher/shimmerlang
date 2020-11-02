# Styleguide for shimmer:
## Naming
* For constants use ```UPPERCASE_WITH_UNDERSCORES_IF_NECESSARY```
* For classes use ```PascalCaseWithoutUnderscores```
* For variables use ```snake_case_with_no_uppercase```
* For functions use ```snake_case_with_no_uppercase``` as well

## Braces
* Keep braces on the same line as the function/class, as in ```void foo() {```
* Even if you can make 1 line blocks, like ```if (foo == 5) bar();```,
  only do that if you can keep it under one screen, and use braces

## Macros and typedefs
* Only use them if it makes the code cleaner.

## Whitespace
* Indentation is harmful and bad for code (JK, use spaces when possible)

## Pointers
* Pointers should be like this normally: ```int* foo(void* x)```
* For pointers to function use ```void (*hello)()```

## Line length
* Always keep lines shorter than a screen.
## Expressions
* Always put spaces around operators: ```foo + bar * baz```
* Always use parentheses if it helps readability: ```(foo + bar) * baz```