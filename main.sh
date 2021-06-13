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

else
  if [[ $answer =~ [A-Z] ]]; then
    if [[ $answer == "D" ]]; then
      d_compile

    else
      compile

    fi
  fi

  answer="$(tr [A-Z] [a-z] <<< "$answer")"

  if [[ $answer == "d" ]]; then
    gdb build/shimmerlang

  elif [[ $answer == "t" ]]; then
    build/shimmerlang shmr/test.shmr

  elif [[ $answer == "r" ]]; then
    build/shimmerlang

  elif [[ $answer == "e" ]]; then
    read -p "Which file? " thefile
    build/shimmerlang $thefile

    if [ "$?" -ne "0" ]; then
      echo "Could not find file $thefile, trying shmr/$thefile..."
      sleep 1

      build/shimmerlang "shmr/$thefile"
    fi

    if [ "$?" -ne "0" ]; then
      echo "main.sh: error: could not find the file specified."

    fi

  else
    echo "main.sh: error: unknown command: $answer."

  fi
fi
