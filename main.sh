#!/bin/bash

/bin/bash print_options.sh

read -p "Which option? [cC/dD/tT/rR/sS] " answer
echo "================================"

compile () {
  export CXX=/usr/bin/clang++
  if [ ! -d build ]; then mkdir build; fi;
  cd build;
  read -p "What version? " VER;

  if [ "$1" == "D" ]; then
    echo "=== Compiling in 'Debug' mode ==="
    sleep 1
    cmake -DBUILD_NUMBER="$VER" -DCMAKE_BUILD_TYPE=Debug ../src;

  else
    echo "=== Compiling normally ==="
    sleep 1
    cmake -DBUILD_NUMBER="$VER" ../src;
  fi

  make;
  export BASE=$?
  cd ..;
  return "$BASE";
}

if [[ $answer == "" ]]; then
  echo "=== Canceled ==="

elif [[ $answer == "c" ]] || [[ $answer =~ [A-Z] ]]; then
  echo "=== Compiling Shimmerlang ==="
  compile

  if [ "$?" != "0" ]; then
    echo "=== Failed! ==="
    exit 1
  fi

  if [[ $answer == "c" ]]; then
    exit 0

  fi
fi

answer="$(tr [A-Z] [a-z] <<< "$answer")"

if [[ $answer == "d" ]]; then
  echo "=== Debugging Shimmerlang ==="
  gdb build/shimmerlang

elif [[ $answer == "t" ]]; then
  echo "=== Running the test file shmr/test.shmr ==="
  build/shimmerlang shmr/test.shmr

elif [[ $answer == "r" ]]; then
  read -p "Which file? " thefile

  echo "=== Running file $thefile ==="
  sleep 1
  build/shimmerlang $thefile

  if [ "$?" -ne "0" ]; then
    echo "Could not find file $thefile, trying shmr/$thefile..."
    sleep 1

    build/shimmerlang "shmr/$thefile"

  fi

  if [ "$?" -ne "0" ]; then
    echo "main.sh: error: could not find the file specified or there was an error while running the file."

  fi

elif [[ $answer == "s" ]]; then
  echo "=== Shimmerlang Shell ==="
  sleep 1
  build/shimmerlang

else
  echo "main.sh: error: unknown command: $answer."

fi
