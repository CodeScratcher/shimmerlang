.PHONY: build lib
SRC_DIR = src
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
CXXFLAGS := -Isrc -D DEBUG -g -o build/shimmerlang
build:
	g++ $(CXXFLAGS) $(SRC_FILES)

lib:
	g++ -Isrc -c -Wall -Werror -fpic src/ShimmerClasses.cpp -o dot.o;
	g++ -shared dot.o -o build/libshimmer.so;
  