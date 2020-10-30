#!/bin/bash
echo Dot is a language partially inspired by lisp

read -p "Do you want to compile? [y/n] " answer
if [[ $answer == "Y" ]] || [[ $answer == "y" ]]; then
	make
  #g++ -Isrc -c -Wall -Werror -fpic src/DotClasses.cpp -o dot.o
  #g++ -shared dot.o -o build/libdot.so
fi

read -p "Do you want to run? [y/n] " answer
if [[ $answer == "Y" ]] || [[ $answer == "y" ]]; then
	build/shimmerlang
else
  echo "You can run a program using build/shimmerlang (program name)"
  read -p "Do you want to debug? [y/n] " answer
  if [[ $answer == "Y" ]] || [[ $answer == "y" ]]; then
    gdb build/shimmerlang
  else
    echo "You can debug using gdb build/shimmerlang"
  fi
fi