#!/bin/bash

echo -e "\e[1;4;31m"
echo "ATTENTION: In ShimmerClasses.cpp, I tried to fix the line number issue."
echo "But then, I realized that DotLiteral doesn't inherit from DotToken."
echo "I wanted to make it inherit, but I wasn't sure if I should do that, so I didn't."
echo ""
echo "So this is my plan:"
echo "So the problem now is that we need to get a line number from std::vector<> params"
echo "in the DotStatement class. To do that, we get a line number from the DotLiteral returned by"
echo ".get_literal_val(). Note that if you attempt to compile, you will get compilation errors"
echo "complaining about DotLiteral not having a .get_line_number() method."
echo -e "\e[0m"

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
