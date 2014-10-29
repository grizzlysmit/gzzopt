This is the start of the gzzopt option parsing library
======================================================

##Author: Francis Grizzly Smit##

The library (not a lib yet) is contained in the files *gzzopt.hpp* and *gzzopt.cpp*, there are also 4 example programs *calc, calc2, example0,* and *example*, *example* is a complex example of just what complex a set of command lines you can achieve; *example0* shows how you can go wrong the gotta is 
that *example0* tries to have the command line:
<pre>
...
./example0 install <pacakage>...
./example0 install info <pacakage>...
</pre>

the problem is *install* shadows *info* and info is treated as a package argument where as *example* uses: 
<pre>
...
./example install <pacakage>...
./example info <pacakage>...
</pre>

this works fine try each *calc* and *calc2* are the same except for how there arguments are specified, to wit:
<pre>
21:26:00 grizzlysmit@rakbat:~/Projects/C++/gzzoption$ ./calc --help
./calc --help|-h
./calc [--help|-h] [--verbose|-v] [--quiet|-q] add|sub|mult|div|pow <x> <y>


where:
	--help|-h            : show this help
	                       
	[--help|-h]          : show this help
	[--verbose|-v]       : proceed verbosely
	[--quiet|-q]         : proceed quietly
	add|sub|mult|div|pow : apply op to x and y
	<x>                  : a double value
	<y>                  : a double value
	                       

21:29:41 grizzlysmit@rakbat:~/Projects/C++/gzzoption$ ./calc2 --help
./calc2 [--help|-h]
./calc2 [--verbose|-v] [--quiet|-q] add <x> <y>
./calc2 [--verbose|-v] [--quiet|-q] sub <x> <y>
./calc2 [--verbose|-v] [--quiet|-q] mult <x> <y>
./calc2 [--verbose|-v] [--quiet|-q] div <x> <y>
./calc2 [--verbose|-v] [--quiet|-q] pow <x> <y>


where:
	[--help|-h]    : show this help
	                 
	[--verbose|-v] : proceed verbosely
	[--quiet|-q]   : proceed quietly
	add            : apply op to x and y
	<x>            : a double value
	<y>            : a double value
	                 
	sub            : apply op to x and y
	                 
	mult           : apply op to x and y
	                 
	div            : apply op to x and y
	                 
	pow            : apply op to x and y
	                 
</pre>

As you can see *calc* uses one argument spec *add|sub|mult|div|pow* where as *calc2* does the same thing using one command line per *add*, through to *pow*, as a result I used *p.set_compact(true);* to avoid duplicate lines in the usage message; in *calc2.cpp*.
All of the programs correspond to a *.cpp* file see the *Makefile* for details.


