#!/bin/bash

/bin/bash print_options.sh
echo "ATTENTION: I MAY HAVE BROKEN SOMETHING, TRY RUNNING NG.SHMR"
read -p "Which option? [cC/dD/tT/rR/eE] " answer

compile () {
  export CXX=/usr/bin/clang++
  if [ ! -d build ]; then mkdir build; fi;
  cd build;
  read -p "What version? " VER;
  cmake -DBUILD_NUMBER="$VER" ../src;
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
  cmake -DBUILD_NUMBER="$VER" -DCMAKE_BUILD_TYPE=Debug ../src;
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

elif [[ $answer == "t" ]]; then
    build/shimmerlang test.shmr;

elif [[ $answer == "T" ]]; then
  if compile; then
    build/shimmerlang test.shmr;
  fi

elif [[ $answer == "r" ]]; then
    build/shimmerlang;

elif [[ $answer == "R" ]]; then
  if compile; then
    build/shimmerlang;
  fi

elif [[ $answer == "e" ]]; then
  build/shimmerlang;

elif [[ $answer == "E" ]]; then
  build/shimmerlang;
fi
