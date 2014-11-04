This is the gzzopt option parsing library
=========================================

Author: Francis Grizzly Smit
----------------------------

## Contents

- [Introduction](#introduction)
- [Count](#count)
    - [A brief explanation of the format of a program using gxxopts](#a-brief-explanation-of-the-format-of-a-program-using-gxxopts)
- [Making the Library a Library](#making-the-library-a-library)
- [Bad.cpp](#bad)

<!-- end toc -->



### Introduction ###

The library is contained in the files *gzzopt.hpp* and *gzzopt.cpp*, (see below for how to create libgzzopt.a and libgzzopt.so) there are also 7 example programs *calc, calc2, count, bad, empty, example0,* and *example*, *example* is a complex example of just what complex a set of command lines you can achieve; *example0* shows how you can go wrong the gotcha is that *example0* tries to have the command line:
<pre>
...
./example0 install &lt;pacakage&gt;...
./example0 install info &lt;pacakage&gt;...
</pre>

the problem is *install* shadows *info* and info is treated as a package argument where as *example* uses: 
<pre>
...
./example install &lt;pacakage&gt;...
./example info &lt;pacakage&gt;...
</pre>

This works fine, try each; *calc* and *calc2* are the same as each other, except for how there arguments are specified, to wit:
<pre>
21:26:00 grizzlysmit@rakbat:~/Projects/C++/gzzoption$ ./calc --help
./calc --help|-h
./calc [--help|-h] [--verbose|-v] [--quiet|-q] add|sub|mult|div|pow &lt;x&gt; &lt;y&gt;


where:
	--help|-h            : show this help
	                       
	[--help|-h]          : show this help
	[--verbose|-v]       : proceed verbosely
	[--quiet|-q]         : proceed quietly
	add|sub|mult|div|pow : apply op to x and y
	&lt;x&gt;                  : a double value
	&lt;y&gt;                  : a double value
	                       

21:29:41 grizzlysmit@rakbat:~/Projects/C++/gzzoption$ ./calc2 --help
./calc2 [--help|-h]
./calc2 [--verbose|-v] [--quiet|-q] add &lt;x&gt; &lt;y&gt;
./calc2 [--verbose|-v] [--quiet|-q] sub &lt;x&gt; &lt;y&gt;
./calc2 [--verbose|-v] [--quiet|-q] mult &lt;x&gt; &lt;y&gt;
./calc2 [--verbose|-v] [--quiet|-q] div &lt;x&gt; &lt;y&gt;
./calc2 [--verbose|-v] [--quiet|-q] pow &lt;x&gt; &lt;y&gt;


where:
	[--help|-h]    : show this help
	                 
	[--verbose|-v] : proceed verbosely
	[--quiet|-q]   : proceed quietly
	add            : apply op to x and y
	&lt;x&gt;            : a double value
	&lt;y&gt;            : a double value
	                 
	sub            : apply op to x and y
	                 
	mult           : apply op to x and y
	                 
	div            : apply op to x and y
	                 
	pow            : apply op to x and y
	                 
</pre>

As you can see *calc* uses one argument spec *add|sub|mult|div|pow* where as *calc2* does the same thing using one command line per *add*, through to *pow*, as a result to avoid duplicate lines in the usage message; I used *p.set_compact(true);* in *calc2.cpp*.
All of the programs correspond to a *.cpp* file see the *Makefile* for details.

### Count ###

*count* shows a simple example of an incrementing integer option see *count.cpp*, this is done with an incrementing function, this is the best solution for this problem, I could do it by adding still more helper classes and special constructors and it gets progressively more ugly, so I bailed for something which will have fewer problems, and I think is an eloquent solution. see code below, or read *count.cpp*:

<pre>
   <code>
#include "gzzopt.hpp"
#include &lt;string&gt;


int main(int argc, char\* argv[]){
    using namespace gzzopts;
    int cnt = 0;
    bool help = false;
    std::string s;

    Opts opt{OptionSpec(help, "show this help", "help", 'h'),
              OptionSpec([&cnt]() -&gt; bool { return ++cnt; },
                      "increment the number of times to repeat",
                                  "count", 'c' ).set_multi(true), 
              strvalue(s, "a string", "s").set_manditory(true),
            };

    OptionParser p(argc, argv, opt);
    // parse away //
    if(!p.parse()){
        p.fullusage();
        return 1;
    }
    /////////////////////////////////////////////////////////////
    //                                                         //
    //             Use the variables that where set            //
    //                                                         //
    /////////////////////////////////////////////////////////////
    if(help){
        p.fullusage();
        return 0;
    }

    for(int i = 0; i &lt; cnt; i++) std::cout &lt;&lt; '`' &lt;&lt; s &lt;&lt; "' ";
    std::cout &lt;&lt; std::endl;
    return 0;
}
   </code>
</pre>

###### NB #######
Have to use *strvalue* to allow arbitary strings, should just been a variant of *positional* but my compiler goes to the **template** version instead. :P


#### A brief explanation of the format of a program using gxxopts ####

The format of a program is as follows in <code>main(int argc, char \*argv[])</code> or similar, define one or more Opt objects, the elements of the Opt object are OptionSpec objects, these can be entered directly or via the convenience functions *positionnal* and *literal* which produce OptionSpec objects with extra options turned on a *positional* is just a positional argument, and a *literal* is a literal string to expect, in addition OptionSpec has a initializer\_list constructor <code>OptionSpec(std::initializer\_list&lt;Opts\*&gt; opts)</code> which take a list of pointers to Opt objects (had to be pointers as Opts is an incomplete type at the time) so you can produce tree like structures. The constructor for Opt itself is a initializer\_list <code>Opts(std::initializer\_list&lt;OptionSpec&gt; lst)</code> so you can specify arbitrarily many elements.

The OptionSpec elements are of the form variable/setter function, description, a long opt name and an optional short opt char, and some optional boolean parameters to set things like positional multi etc use the convenience functions instead there are also setter functions that can be chained, these all take a single bool parameter and set the field their name suggests.

Then when you have combined all this into one Opt object see *calc.cpp* and *call2.cpp*; construct a OptionParser object as follows <code>OptionParser p(argc, argv, opt);</code> then you can set a few parameters on the OptionParser object or not; then call p.parse(), this will return true on a error free run and false of error; then if parse returned true just use the variables that where set, the variables can be std::list&lt;T&gt; and std::vector&lt;T&gt; types as well so you can have slurpy arrays ala perl6 too; bellow is some sample code to illustrate:

<pre>
   <code>
#include "gzzopt.hpp"
#include &lt;string&gt;
#include &lt;cmath&gt;

enum class Opp  { none, add, sub, mult, div, pow };

...

int main(int argc, char \*argv[]){
    using namespace gzzopts;
    double x, y;
    Opp opp = Opp::none;
    bool quiet = true, help = false;
    function_str verb = [&quiet](const std::string& val) -> bool
                                                { quiet = false; return !quiet; };
    Opts opt1{OptionSpec(help, "show this help", "help", 'h'),
                      OptionSpec(verb, "proceed verbosely", "verbose", 'v' ), 
                      OptionSpec([&quiet](const std::string& val) -> bool
                              { return quiet = true; },
                                           "proceed quietly", "quiet", 'q' ), 
                      literal([&opp](const std::string& n) -> bool
                              { return set_opp(opp, n); },
                                   "apply op to x and y",
                                   "add|sub|mult|div|pow").set_no_more_opts(true),
                      positional(x, "a double value", "x").set_manditory(true),
                      positional(y, "a double value", "y").set_manditory(true),
                    };
   Opts opt{
                      OptionSpec{&opt1, },
                      OptionSpec(help, "show this help",
                              "help", 'h').set_manditory(true).set_cut(true),
           };
   
    OptionParser p(argc, argv, opt);
    // parse away //
    if(!p.parse()){
        p.fullusage();
        return 1;
    }
    /////////////////////////////////////////////////////////////
    //             Use the variables that where set            //
    /////////////////////////////////////////////////////////////
    if(help){
        p.fullusage();
        return 0;
    }
    switch(opp){
        case(Opp::none):
            std::cout &lt;&lt; "nothing to do" &lt;&lt; std::endl;
            break;
        case(Opp::add):
            if(!quiet) std::cout &lt;&lt; x &lt;&lt; " + " &lt;&lt; y &lt;&lt; " == ";
            std::cout &lt;&lt; (x + y) &lt;&lt; std::endl;
            break;
        case(Opp::sub):
            if(!quiet) std::cout &lt;&lt; x &lt;&lt; " - " &lt;&lt; y &lt;&lt; " == ";
            std::cout &lt;&lt; (x - y) &lt;&lt; std::endl;
            break;
        case(Opp::mult):
            if(!quiet) std::cout &lt;&lt; x &lt;&lt; " * " &lt;&lt; y &lt;&lt; " == ";
            std::cout &lt;&lt; (x * y) &lt;&lt; std::endl;
            break;
        case(Opp::div):
            if(!quiet) std::cout &lt;&lt; x &lt;&lt; " / " &lt;&lt; y &lt;&lt; " == ";
            std::cout &lt;&lt; (x / y) &lt;&lt; std::endl;
            break;
        case(Opp::pow):
            if(!quiet) std::cout &lt;&lt; x &lt;&lt; "^" &lt;&lt; y &lt;&lt; " == ";
            std::cout &lt;&lt; std::pow(x, y) &lt;&lt; std::endl;
            break;
        default:
            std::cerr &lt;&lt; "how in heavens name did I get here" &lt;&lt; std::endl;
            return 1;
    }
    return 0;
}
   </code>
</pre>



###### See Also: ######

*calc.cpp* and *calc2.cpp* for a full example; and *example.cpp* for an example of how convoluted you can get. And *example0.cpp* for an example of a flawed spec reread above for why it's flawed.


###### NB ######
This code was compiled with the -std=c++1y option to the g++ compiler as I use some of the more recent features of the C++ Programming language.



### Making the Library a Library ###

To make the library a library on Linux  do this 
<pre>
   <code>g++ -shared -fPIC -o libgzzopt.so gzzopt.o</code> 
</pre>

I shall look at putting this in the make file when I learn to do plaform specific stuff in a make file.

To make a statically linked library of it under Linux etc al do this
<pre>
   <code>
   ar -cvq libgzzopt.a gzzopt.o
   </code>
</pre>

### Bad ###
The program bad **bad.cpp** is an example of a bad option to wit a ? for the short option this is picked up by the good() method.
