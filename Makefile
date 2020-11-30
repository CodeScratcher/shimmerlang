SHELL := /bin/bash

.PHONY: build lib

SRC_DIR = src
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
SRC_FILES := $(filter-out $(SRC_DIR)/lexer_alt.cpp, $(SRC_FILES))
CXXFLAGS := -std=c++17 -Isrc -D DEBUG -g -o build/shimmerlang

.ONESHELL:
build:
	@echo -e "Compiling...\n";
	read -p "Which version? " VER;
	g++ $(CXXFLAGS) -D VER=\"$$VER\" $(SRC_FILES);

lib:
	@g++ -Isrc -c -Wall -Werror -fpic src/ShimmerClasses.cpp -o dot.o; \
	g++ -shared dot.o -o build/libshimmer.so;
