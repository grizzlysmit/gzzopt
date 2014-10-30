# the compiler to use.
CC=g++
#CC=clang++
# options I'll pass to the compiler.
CFLAGS=-c -Wall -std=c++1y -g

all: example example0 calc calc2 count

example: gzzopt.o example.o
	$(CC) gzzopt.o example.o -o example

example0: gzzopt.o example0.o
	$(CC) gzzopt.o example0.o -o example0

calc: gzzopt.o calc.o
	$(CC) gzzopt.o calc.o -o calc

calc2: gzzopt.o calc2.o
	$(CC) gzzopt.o calc2.o -o calc2

count: gzzopt.o count.o
	$(CC) gzzopt.o count.o -o count

gzzopt.o: gzzopt.cpp gzzopt.hpp
	$(CC) $(CFLAGS) gzzopt.cpp

example.o: example.cpp
	$(CC) $(CFLAGS) example.cpp

example0.o: example0.cpp
	$(CC) $(CFLAGS) example0.cpp

calc.o: calc.cpp
	$(CC) $(CFLAGS) calc.cpp

calc2.o: calc2.cpp
	$(CC) $(CFLAGS) calc2.cpp

count.o: count.cpp
	$(CC) $(CFLAGS) count.cpp

clean:
	rm -rf *.o example example0 calc calc2

