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

As you can see *calc* uses one argument spec *add|sub|mult|div|pow* where as *calc2* does the same thing using one command line per *add*, through to *pow*, as a result to avoid duplicate lines in the usage message; I used *p.set_compact(true);* in *calc2.cpp*.
All of the programs correspond to a *.cpp* file see the *Makefile* for details.

####A brief explanation of the format of a program using gxxopts####

The format of a program is as follows in <code>main(int argc, char \*argv[])</code> or similar, define one or more Opt objects, the elements of the Opt object are OptionSpec objects, these can be entered directly or via the convenience functions *positionnal* and *literal* which produce OptionSpec objects with extra options turned on a *positional* is just a positional argument, and a *literal* is a literal string to expect, in addition OptionSpec has a initializer\_list constructor <code>OptionSpec(std::initializer\_list&lt;Opts\*&gt; opts)</code> which take a list of pointers to Opt objects (had to be pointers as Opts is an incomplete type at the time) so you can produce tree like structures. The constructor for Opt itself is a initializer\_list <code>Opts(std::initializer\_list&lt;OptionSpec&gt; lst)</code> so you can specify arbitrarily many elements.

The OptionSpec elements are of the form variable/setter function, description a long opt name and an optional short opt char, and some optional boolean parameters to set things like positional multi etc use the convenience functions instead there are also setter functions that can be chained, these all take a single bool parameter and set the field their name suggests.

Then when you have combined all this into one Opt object see *calc.cpp* and *call2.cpp*; construct a OptionParser object as follows <code>OptionParser p(argc, argv, opt);</code> then you can set a few parameters on the OptionParser object or not then call p.parse() this will return true on a error free run and false of error; then if parse returned true just use the variables that where set, the variables can be std::list&lt;T&gt; and std::vector&lt;T&gt; types as well so you can have slurpy arrays ala perl6 too; bellow is some sample code to illustrate:

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
    function_str verb = [&quiet](const std::string& val) -> bool { quiet = false; return !quiet; };
    Opts opt1{OptionSpec(help, "show this help", "help", 'h'),
                      OptionSpec(verb, "proceed verbosely", "verbose", 'v' ), 
                      OptionSpec([&quiet](const std::string& val) -> bool { return quiet = true; }, "proceed quietly", "quiet", 'q' ), 
                      literal([&opp](const std::string& n) -> bool { return set_opp(opp, n); }, "apply op to x and y", "add|sub|mult|div|pow").set_no_more_opts(true),
                      positional(x, "a double value", "x").set_manditory(true),
                      positional(y, "a double value", "y").set_manditory(true),
                    };
   Opts opt{
                      OptionSpec{&opt1, },
                      OptionSpec(help, "show this help", "help", 'h').set_manditory(true).set_cut(true),
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



######See Also:######

*calc.cpp* and *calc2.cpp* for a full example; and *example.cpp* for an example of how convoluted you can get. And *example0.cpp* for an example of a flawed spec reread above for why it's flawed.

