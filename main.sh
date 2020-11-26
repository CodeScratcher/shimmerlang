#!/bin/bash

echo -e "\e[93m((Shimmer (is a language) (partially inspired)) by lisp)\e[0m"
echo -e "\e[92mmake_excited(but with(its own syntax!!!))\e[0m"
echo -e "\n\e[4;96mWhat do you want to do?\e[0m\n"
echo -e "\e[95m***NOTE: capitalized = also compile***\n\e[0m"
echo -e "\e[93m c = compile,\e[0m"
echo -e "\e[93m d = debug,\e[0m"
echo -e "\e[93m t = test,\e[0m"
echo -e "\e[93m r = run,\e[0m"
echo -e "\e[93m other = cancel\n\e[0m"
read -p "Which option? [cC/dD/tT/rR] " answer

if [[ $answer == "c" ]] || [[ $answer == "C" ]]; then
	make
  #g++ -Isrc -c -Wall -Werror -fpic src/DotClasses.cpp -o dot.o
  #g++ -shared dot.o -o build/libdot.so
fi

if [[ $answer == "d" ]]; then
  gdb build/shimmerlang
fi

if [[ $answer == "D" ]]; then
  if make; then
    gdb build/shimmerlang
  fi
fi

if [[ $answer == "t" ]]; then
	build/shimmerlang test.shmr;
fi

if [[ $answer == "T" ]]; then
  if make; then
    build/shimmerlang test.shmr;
  fi
fi

if [[ $answer == "r" ]]; then
	build/shimmerlang;
fi

if [[ $answer == "R" ]]; then
  if make; then
    build/shimmerlang;
  fi
fi
