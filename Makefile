lox: main.o
	clang++ *.o -o lox

main.o: main.cpp components/*.cpp
	clang++ -std=c++17 -c main.cpp components/*.cpp

clean: 
	rm *.o lox
