#!/bin/bash

/bin/bash print_options.sh
read -p "Which option? [cC/dD/tT/rR] " answer

if [[ $answer == "c" ]] || [[ $answer == "C" ]]; then
	make
  #g++ -Isrc -c -Wall -Werror -fpic src/DotClasses.cpp -o dot.o
  #g++ -shared dot.o -o build/libdot.so

elif [[ $answer == "d" ]]; then
  gdb build/shimmerlang

elif [[ $answer == "D" ]]; then
  if make; then
    gdb build/shimmerlang
  fi

else

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
fi
