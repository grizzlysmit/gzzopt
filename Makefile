# the compiler to use.
CC=g++
#CC=clang++
# options I'll pass to the compiler.
CFLAGS=-c -Wall -std=c++1y

all: example example0 calc calc2

example: gzzopt.o example.o
	$(CC) gzzopt.o example.o -o example

example0: gzzopt.o example0.o
	$(CC) gzzopt.o example0.o -o example0

calc: gzzopt.o calc.o
	$(CC) gzzopt.o calc.o -o calc

calc2: gzzopt.o calc2.o
	$(CC) gzzopt.o calc2.o -o calc2

gzzopt.o: gzzopt.cpp
	$(CC) $(CFLAGS) gzzopt.cpp

example.o: example.cpp
	$(CC) $(CFLAGS) example.cpp

example0.o: example0.cpp
	$(CC) $(CFLAGS) example0.cpp

calc.o: calc.cpp
	$(CC) $(CFLAGS) calc.cpp

calc2.o: calc2.cpp
	$(CC) $(CFLAGS) calc2.cpp

clean:
	rm -rf *.o example example0 calc calc2
