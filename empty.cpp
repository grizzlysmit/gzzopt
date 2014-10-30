/*
 * =====================================================================================
 *
 *       Filename:  empty.cpp
 *
 *    Description:  a gzzopts program that allows an empty comand line 
 *
 *        Version:  1.0
 *        Created:  Friday 2014-10-31 03:24:47
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

    std::cout << cnt << std::endl;
    return 0;
}
