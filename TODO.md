# TODO

* rename .h to .hpp

* ## For 0.1 release
  * ### Major
    * All done!

  * ### Minor
    * Done

  * ### Patch
    * Done because we said so

* ## For 0.2 release
  * ### Major
    * negative number literals
    * split ShimmerClasses into 3
      * tokens.cpp
        * for ShimmerToken and its children
      * parts.cpp
        * ShimmerStatement ShimmerExpr ShimmerTree ShimmerLiteral
      * builtins.cpp
        * BIF LookupResult and class splitted from ShimmerStatement
    * TCO
    * Foreign functions
    * arrays: turn strings into arrays to reduce duplication??
      * string is subclass of array
        * has all array funcs but also more like `get_char()`, `set_char()`, `insert()`, etc
    * hashmaps / objects
      * format:
        * `< key1: value1, key2: () { "value2" } >`

  * ### Minor
    * clean up code, starting with parser.cpp
      * continue where left off, line ~= 225
    * hex and binary support
    * split
    * bitwise operators
    * maybe add `[, step]` param to `slice()`
    * reduce the large amount of duplication 
      * like wtf SC.h is almost 1000 lines long

  * ### Patch
    * better table lookup
