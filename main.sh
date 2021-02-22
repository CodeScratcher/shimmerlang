#!/bin/bash

/bin/bash print_options.sh
read -p "Which option? [cC/dD/tT/rR] " answer

compile () {
   export CXX=/usr/bin/clang++
   if [ ! -d build ]; then mkdir build; fi;
   cd build;
   read -p "What version? " VER;
   cmake DBUILD_NUMBER=$VER ../src;
   make;
   export BASE=$?
   cd ..;
   return $BASE;
}

d_compile () {
   export CXX=/usr/bin/clang++
   if [ ! -d build ]; then mkdir build; fi;
   cd build;
   read -p "What version? " VER;
   cmake -DBUILD_NUMBER=$VER -DCMAKE_BUILD_TYPE=Debug ../src;
   make;
   export BASE=$?
   cd ..;
   return $BASE;
}

if [[ $answer == "c" ]] || [[ $answer == "C" ]]; then
	compile
  #g++ -Isrc -c -Wall -Werror -fpic src/DotClasses.cpp -o dot.o
  #g++ -shared dot.o -o build/libdot.so

elif [[ $answer == "d" ]]; then
  gdb build/shimmerlang

elif [[ $answer == "D" ]]; then
  if d_compile; then
    gdb build/shimmerlang
  fi

else
  if [[ $answer == "t" ]]; then
    build/shimmerlang test.shmr;
  fi

  if [[ $answer == "T" ]]; then
    if compile; then
      build/shimmerlang test.shmr;
    fi
  fi

  if [[ $answer == "r" ]]; then
    build/shimmerlang;
  fi

  if [[ $answer == "R" ]]; then
    if compile; then
      build/shimmerlang;
    fi
  fi
fi
