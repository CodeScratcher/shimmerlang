.PHONY: build lib
build:
	clang++ -Isrc -D DEBUG -g src/DotClasses.cpp src/eval.cpp src/dotlang.cpp src/parser.cpp src/lexer.cpp -o build/dotlang;

lib:
	g++ -Isrc -c -Wall -Werror -fpic src/DotClasses.cpp -o dot.o;
	g++ -shared dot.o -o build/libdot.so;
