lox: main.o
	clang++ main.o error.o -o lox

main.o: main.cpp error.cpp
	clang++ -std=c++17 -c main.cpp error.cpp

clean: 
	rm *.o lox
