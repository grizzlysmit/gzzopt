ifneq (,$(findstring /cygdrive/,$(PATH)))
    UNAME := Cygwin
else
ifneq (,$(findstring WINDOWS,$(PATH)))
    UNAME := Windows
else
    UNAME := $(shell uname -s)
endif
endif
# the compiler to use.
CC=g++
#CC=clang++
# options I'll pass to the compiler.
CFLAGS=-c -Wall -std=c++1y


all: example example0 calc calc2 count empty libs bad vect

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

empty: gzzopt.o empty.o
	$(CC) gzzopt.o empty.o -o empty

bad: gzzopt.o bad.o
	$(CC) gzzopt.o bad.o -o bad

vect: gzzopt.o vect.o
	$(CC) gzzopt.o vect.o -o vect

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

empty.o: empty.cpp
	$(CC) $(CFLAGS) empty.cpp

bad.o: bad.cpp
	$(CC) $(CFLAGS) bad.cpp

vect.o: vect.cpp
	$(CC) $(CFLAGS) vect.cpp

libs: libgzzopt.a libgzzopt.so

libgzzopt.a: gzzopt.o
ifeq ($(UNAME),Linux)
	ar -cvq libgzzopt.a gzzopt.o
else
	echo "don't know how to do static libs on $(UNAME)" 
endif

libgzzopt.so: gzzopt.o
ifeq ($(UNAME),Linux)
	$(CC) -shared -fPIC -o libgzzopt.so gzzopt.o 
else
	echo "don't know how to do dynamic libs on $(UNAME)" 
endif

INSTALL = /usr/bin/install
CCVERSION = $(shell $(CC) -dumpversion)
PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin
SRCDIR = $(PREFIX)/share/gzzopt
INCLUDEDIR = $(PREFIX)/include/c++/$(CCVERSION)
LIBDIR = $(PREFIX)/lib
exe := example example0 calc calc2 count empty

install: all
	$(INSTALL) -D libgzzopt.a $(LIBDIR)/libgzzopt.a
	$(INSTALL) -D libgzzopt.so $(LIBDIR)/libgzzopt.so
	$(INSTALL) -D example $(BINDIR)/example
	$(INSTALL) -D example0 $(BINDIR)/example0
	$(INSTALL) -D calc $(BINDIR)/calc
	$(INSTALL) -D calc2 $(BINDIR)/calc2
	$(INSTALL) -D count $(BINDIR)/count
	$(INSTALL) -D empty $(BINDIR)/empty
	$(INSTALL) -D example.cpp $(SRCDIR)/example.cpp
	$(INSTALL) -D example0.cpp $(SRCDIR)/example0.cpp
	$(INSTALL) -D calc.cpp $(SRCDIR)/calc.cpp
	$(INSTALL) -D calc2.cpp $(SRCDIR)/calc2.cpp
	$(INSTALL) -D count.cpp $(SRCDIR)/count.cppgzzoption_20141110-2_i386.deb
	$(INSTALL) -D empty.cpp $(SRCDIR)/empty.cpp
	$(INSTALL) -D bad.cpp $(SRCDIR)/bad.cpp
	$(INSTALL) -D vect.cpp $(SRCDIR)/vect.cpp
	$(INSTALL) -D Makefile $(SRCDIR)/Makefile
	$(INSTALL) -D README.md $(SRCDIR)/README.md
	$(INSTALL) -D gzzopt.hpp $(SRCDIR)/gzzopt.hpp
	$(INSTALL) -D gzzopt.hpp $(INCLUDEDIR)/gzzopt.hpp

uninstall:
	-rm $(LIBDIR)/libgzzopt.a
	-rm $(LIBDIR)/libgzzopt.so
	-rm $(BINDIR)/example
	-rm $(BINDIR)/example0
	-rm $(BINDIR)/calc
	-rm $(BINDIR)/calc2
	-rm $(BINDIR)/count
	-rm $(BINDIR)/empty
	-rm $(SRCDIR)/example.cpp
	-rm $(SRCDIR)/example0.cpp
	-rm $(SRCDIR)/calc.cpp
	-rm $(SRCDIR)/calc2.cpp
	-rm $(SRCDIR)/count.cpp
	-rm $(SRCDIR)/empty.cpp
	-rm $(SRCDIR)/bad.cpp
	-rm $(SRCDIR)/vect.cpp
	-rm $(SRCDIR)/Makefile
	-rm $(SRCDIR)/README.md
	-rm $(SRCDIR)/gzzopt.hpp
	-rm $(INCLUDEDIR)/gzzopt.hpp

.PHONY: install uninstall

clean:
	rm -rf *.o libgzzopt.so libgzzopt.a example example0 calc calc2 count empty bad vect 

