/*
 * =====================================================================================
 *
 *       Filename:  count.cpp
 *
 *    Description:  and gzzopts example using an incrementing option
 *
 *        Version:  1.0
 *        Created:  Thursday 2014-10-30 18:11:27
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Francis Grizzly Smit (FGJS), grizzlysmit@smit.id.au
 *   Organization:  Me
 *
 * =====================================================================================
 */


#include "gzzopt.hpp"
#include <string>


int main(int argc, char* argv[]){
    using namespace gzzopts;
    int cnt = 0;
    bool help = false;
    std::string s;

    Opts opt{OptionSpec(help, "show this help", "help", 'h'),
              OptionSpec([&cnt]() -> bool { return ++cnt; }, "increment the number of times to repeat", "count", 'c' ).set_multi(true), 
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

    for(int i = 0; i < cnt; i++) std::cout << '`' << s << "' ";
    std::cout << std::endl;
    return 0;
}
