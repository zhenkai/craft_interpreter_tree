lox: main.o
	clang++ main.o error.o token.o scanner.o -o lox

main.o: main.cpp error.cpp token.cpp scanner.cpp
	clang++ -std=c++17 -c main.cpp error.cpp token.cpp scanner.cpp

clean: 
	rm *.o lox
