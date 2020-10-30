#!/bin/bash
echo "Dot is a language partially inspired by lisp"

echo ""
echo "What do you want to do?"
echo "c = Compile, d = Debug, r = Run,"
echo "e = compile and dEbug, u = compile and rUn"
echo ""
read -p "Which option? [cdreu] " answer

if [[ $answer == "c" ]] || [[ $answer == "C" ]]; then
	make
  #g++ -Isrc -c -Wall -Werror -fpic src/DotClasses.cpp -o dot.o
  #g++ -shared dot.o -o build/libdot.so
fi

if [[ $answer == "r" ]] || [[ $answer == "R" ]]; then
	build/shimmerlang
fi

if [[ $answer == "d" ]] || [[ $answer == "D" ]]; then
  gdb build/shimmerlang
fi

if [[ $answer == "u" ]] || [[ $answer == "U" ]]; then
  make
  build/shimmerlang
fi

if [[ $answer == "e" ]] || [[ $answer == "E" ]]; then
  make
  gdb build/shimmerlang
fi