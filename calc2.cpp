/*
 * =====================================================================================
 *
 *       Filename:  calc.cpp
 *
 *    Description:  calc use of gzzopts 
 *
 *        Version:  1.0
 *        Created:  wednessday 2014-10-29 09:58:21
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Francis Grizzly Smit (FGJS), grizzlysmit@smit.id.au
 *   Organization:  Me
 *
 * =====================================================================================
 */
#include "gzzopt.hpp"
#include <list>
#include <vector>
#include <string>
#include <cmath>

enum class Opp  { none, add, sub, mult, div, pow };

bool set_opp(Opp &opp, const std::string value){
    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tvalue == " << value << "\t__func__ == " << __func__ << std::endl;
    if(value == "add"){
        opp = Opp::add;
    }else if(value == "sub"){
        opp = Opp::sub;
    }else if(value == "mult"){
        opp = Opp::mult;
    }else if(value == "div"){
        opp = Opp::div;
    }else if(value == "pow"){
        opp = Opp::pow;
    }else{
        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
        return false;
    }
    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
    return true;
}

std::string to_str(Opp opp){
    using namespace std;
    switch(opp){
        case(Opp::none): return "Opp::none"s;
        case(Opp::add): return "Opp::add"s;
        case(Opp::sub): return "Opp::sub"s;
        case(Opp::mult): return "Opp::mult"s;
        case(Opp::div): return "Opp::div"s;
        case(Opp::pow): return "Opp::pow"s;
        default:
            return "Erroneous value how'd that happen"s;
    }
}

int main(int argc, char *argv[]){
    using namespace gzzopts;
    double x, y;
    Opp opp = Opp::none;
    bool quiet = true, help = false;
    function_str verb = [&quiet](const std::string& val) -> bool { quiet = false; return !quiet; };
    Opts opt1{
                      OptionSpec(verb, "proceed verbosely", "verbose", 'v' ), 
                      OptionSpec([&quiet](const std::string& val) -> bool { return quiet = true; }, "proceed quietly", "quiet", 'q' ), 
                      literal([&opp](const std::string& n) -> bool { return set_opp(opp, n); }, "apply op to x and y", "add").set_no_more_opts(true),
                      positional(x, "a double value", "x").set_manditory(true),
                      positional(y, "a double value", "y").set_manditory(true),
                    };
    Opts opt2{
                      OptionSpec(verb, "proceed verbosely", "verbose", 'v' ), 
                      OptionSpec([&quiet](const std::string& val) -> bool { return quiet = true; }, "proceed quietly", "quiet", 'q' ), 
                      literal([&opp](const std::string& n) -> bool { return set_opp(opp, n); }, "apply op to x and y", "sub").set_no_more_opts(true),
                      positional(x, "a double value", "x").set_manditory(true),
                      positional(y, "a double value", "y").set_manditory(true),
                    };
    Opts opt3{
                      OptionSpec(verb, "proceed verbosely", "verbose", 'v' ), 
                      OptionSpec([&quiet](const std::string& val) -> bool { return quiet = true; }, "proceed quietly", "quiet", 'q' ), 
                      literal([&opp](const std::string& n) -> bool { return set_opp(opp, n); }, "apply op to x and y", "mult").set_no_more_opts(true),
                      positional(x, "a double value", "x").set_manditory(true),
                      positional(y, "a double value", "y").set_manditory(true),
                    };
    Opts opt4{
                      OptionSpec(verb, "proceed verbosely", "verbose", 'v' ), 
                      OptionSpec([&quiet](const std::string& val) -> bool { return quiet = true; }, "proceed quietly", "quiet", 'q' ), 
                      literal([&opp](const std::string& n) -> bool { return set_opp(opp, n); }, "apply op to x and y", "div").set_no_more_opts(true),
                      positional(x, "a double value", "x").set_manditory(true),
                      positional(y, "a double value", "y").set_manditory(true),
                    };
    Opts opt5{
                      OptionSpec(verb, "proceed verbosely", "verbose", 'v' ), 
                      OptionSpec([&quiet](const std::string& val) -> bool { return quiet = true; }, "proceed quietly", "quiet", 'q' ), 
                      literal([&opp](const std::string& n) -> bool { return set_opp(opp, n); }, "apply op to x and y", "pow").set_no_more_opts(true),
                      positional(x, "a double value", "x").set_manditory(true),
                      positional(y, "a double value", "y").set_manditory(true),
                    };
    //*
    Opts opt{
                      OptionSpec{&opt1, &opt2, &opt3, &opt4, &opt5, },
                      OptionSpec(help, "show this help", "help", 'h'),
           };
    //*/

    OptionParser p(argc, argv, opt);
    if(!p.parse()){
        p.fullusage();
        return 1;
    }
    if(help){
        p.fullusage();
        return 0;
    }
    //std::cout << "quiet == " << std::boolalpha << quiet << std::endl;
    //std::cout << "opp == " << std::boolalpha << to_str(opp) << std::endl;
    //std::cout << "x == " << std::boolalpha << x << std::endl;
    //std::cout << "y == " << std::boolalpha << y << std::endl;
    switch(opp){
        case(Opp::none):
            std::cout << "nothing to do" << std::endl;
            break;
        case(Opp::add):
            if(!quiet) std::cout << x << " + " << y << " == ";
            std::cout << (x + y) << std::endl;
            break;
        case(Opp::sub):
            if(!quiet) std::cout << x << " - " << y << " == ";
            std::cout << (x - y) << std::endl;
            break;
        case(Opp::mult):
            if(!quiet) std::cout << x << " * " << y << " == ";
            std::cout << (x * y) << std::endl;
            break;
        case(Opp::div):
            if(!quiet) std::cout << x << " / " << y << " == ";
            std::cout << (x / y) << std::endl;
            break;
        case(Opp::pow):
            if(!quiet) std::cout << x << "^" << y << " == ";
            std::cout << std::pow(x, y) << std::endl;
            break;
        default:
            std::cerr << "how in heavens name did I get here" << std::endl;
            return 1;
    }
    return 0;
}

