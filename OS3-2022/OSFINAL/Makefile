CC = g++
CFLAGS = -Wall -g

main: main2.o process.o
	$(CC) $(CFLAGS) -o main main2.o process.o
main2.o: main2.cpp process.hpp
	$(CC) $(CFLAGS) -c main2.cpp

process.o: process.hpp

clean:
		rm -f main
		rm -f main2.o
		rm -f process.o
 