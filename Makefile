lox: main.o
	clang++ *.o -o lox

main.o: main.cpp components/*.cpp utils/*.cpp
	clang++ -std=c++20 -c main.cpp components/*.cpp utils/*.cpp

clean: 
	rm -f *.o lox
