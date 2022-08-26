lox: main.o
	clang++ main.o -o lox

main.o: main.cpp
	clang++ -c main.cpp

clean: 
	rm *.o lox
