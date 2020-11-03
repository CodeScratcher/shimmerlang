#!/bin/bash
echo -e "\e[92m((Shimmer is) (((a language) (partially inspired)) by lisp))\e[0m"
echo -e "\e[92mmkExcited(butWith(itsOwnSyntax))\e[0m"
echo -e "\n\e[4;96mWhat do you want to do?\e[0m\n\n\e[93m c = compile (C = same thing),\n d = debug   (D = also compile),\n r = run     (R = also compile),\n anything else = cancel\e[0m\n"
read -p "Which option? [cC/dD/rR] " answer

if [[ $answer == "c" ]] || [[ $answer == "C" ]]; then
	make
  #g++ -Isrc -c -Wall -Werror -fpic src/DotClasses.cpp -o dot.o
  #g++ -shared dot.o -o build/libdot.so
fi

if [[ $answer == "r" ]]; then
	build/shimmerlang
fi

if [[ $answer == "d" ]]; then
  gdb build/shimmerlang
fi

if [[ $answer == "R" ]]; then
  if make; then
    build/shimmerlang;
  fi
fi

if [[ $answer == "D" ]]; then
  if make; then
    gdb build/shimmerlang
  fi
fi