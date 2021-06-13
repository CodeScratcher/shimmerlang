# TODO

* ## For 0.1 release
  * ### Major
    * All done!

  * ### Minor
    * string slicing: `slice(str, start[, end][, step])`

  * ### Patch
    * `std::out_of_range` errors
    * fsr `slice()` returns `true` when it should return `"blah"`
    * clean up code, starting with parser.cpp
      * continue where left off, line ~= 160
    * make it stop printing unnecessary x.0's
      * includes pretty printing
        * it shouldn't print: add\n\t3.000000\n\t4.000000

* ## For 0.2 release
  * ### Major
    * TCO
    * Foreign functions
    * arrays: turn strings into arrays to reduce duplication??
      * string is subclass of array
        * has all array funcs but also more like `get_char()`, `set_char()`, `insert()`, etc
    * hashmaps / objects
      * format:
        * `< key1: value1, key2: () { "value2" } >`

  * ### Minor
    * hex and binary support
    * split
    * bitwise operators

  * ### Patch
    * better table lookup
