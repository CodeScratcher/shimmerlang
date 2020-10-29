#!/bin/bash
echo Dot is a language partially inspired by lisp
read -p "Do you want to compile? [y/n] " answer
if [[ $answer == "Y" ]] || [[ $answer == "y" ]]; then
	make
  #g++ -Isrc -c -Wall -Werror -fpic src/DotClasses.cpp -o dot.o
  #g++ -shared dot.o -o build/libdot.so
else
	build/dotlang example.dot
fi