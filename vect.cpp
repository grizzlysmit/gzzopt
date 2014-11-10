/*
 * =====================================================================================
 *
 *       Filename:  example.cpp
 *
 *    Description:  example use of gzzopts 
 *
 *        Version:  1.0
 *        Created:  Tuesday 2014-10-14 19:35:21
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

enum class Opp  { none, add, sub, mult, div, pow };

enum class Opp2 { none, echo, ls };

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

std::string to_str(Opp2 opp){
    using namespace std;
    switch(opp){
        case(Opp2::none): return "Opp2::none"s;
        case(Opp2::echo): return "Opp2::echo"s;
        case(Opp2::ls): return "Opp2::ls"s;
        default:
            return "Erroneous value how'd that happen"s;
    }
}

bool set_opp2(Opp2 &opp2, const std::string value){
    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tvalue == " << value << "\t__func__ == " << __func__ << std::endl;
    if(value == "echo"){
        opp2 = Opp2::echo;
    }else if(value == "ls"){
        opp2 = Opp2::ls;
    }else{
        return false;
    }
    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
    return true;
}

bool mydescribe(){
    std::cerr << "\t[--help|-h]           : show this help\n\n"
              << "\t[--file=file|-f file] : File\n"
              << "\t[--verbose|-v]        : proceed verbosely\n"
              << "\t[--quiet|-q]          : proceed quietly\n"
              << "\tadd|sub|mult|div|pow  : apply op to x and y\n"
              << "\t<x>                   : a double value\n"
              << "\t<y>                   : a double value\n\n"
              << "\techo|ls               : echo or ls all args\n"
              << "\t[vect]...                : a list of strings\n\n"
              << "\tlist                  : list all args\n"
              << "\t[v]...                : a list of doubles\n\n"
              << "\tinstall               : install somestuff\n"
              << "\tinstall info          : get install info\n"
              << "\t<pacakage>...         : a list of packages to install or get info on\n" << std::endl;
    return std::cerr.good();
}

int main(int argc, char *argv[]){
    using namespace gzzopts;
    std::string file;
    double x, y;
    Opp opp = Opp::none;
    Opp2 opp2 = Opp2::none;
    bool quiet = false, help = false, install = false, lst = false, info = false;
    std::vector<std::string> vect;
    std::vector<double> v;
    function_str verb = [&quiet](const std::string& val) -> bool { quiet = false; return !quiet; };
    Opts opt2{literal([&opp2](const std::string& n) -> bool { return set_opp2(opp2, n); }, "echo or ls all args", "echo|ls").set_no_more_opts(true),
              strvect(vect, "a list of strings", "vect"),
                };
    Opts opt3{literal([&lst]() -> bool { return lst = true; }, "list all args", "list"),
              positional(v, "a list of doubles", "v").set_manditory(true),
                };
    Opts opt5{literal(info, "get install info", "info").set_no_more_opts(true),
              strvect(vect, "a list of packages to get info on", "pacakage").set_manditory(true),
                };
    Opts opt4{OptionSpec{&opt5, },
              literal(install, "install somestuff", "install").set_no_more_opts(true),
              strvect(vect, "a list of packages to install", "pacakage").set_manditory(true),
                };
    Opts opt1{ OptionSpec(file, "File", "file", 'f').set_expects_arg(true), 
                      OptionSpec(verb, "proceed verbosely", "verbose", 'v' ), 
                      OptionSpec([&quiet](const std::string& val) -> bool { return quiet = true; }, "proceed quietly", "quiet", 'q' ), 
                      /*
                      positional([&opp]() -> bool { opp = Opp::add;  return true; }, "add x and y", "add"),
                      positional([&opp]() -> bool { opp = Opp::sub;  return true; }, "add x and y", "sub"),
                      positional([&opp]() -> bool { opp = Opp::mult; return true; }, "add x and y", "mult"),
                      positional([&opp]() -> bool { opp = Opp::div;  return true; }, "add x and y", "div"),
                      positional([&opp]() -> bool { opp = Opp::pow;  return true; }, "add x and y", "pow"),
                      // */
                      literal([&opp](const std::string& n) -> bool { return set_opp(opp, n); }, "apply op to x and y", "add|sub|mult|div|pow").set_no_more_opts(true),
                      positional(x, "a double value", "x").set_manditory(true),
                      positional(y, "a double value", "y").set_manditory(true),
                    };
    //*
    Opts opt{
                      OptionSpec{&opt1, &opt2, &opt3, &opt4, },
                      OptionSpec(help, "show this help", "help", 'h'),
           };
    //*/

    OptionParser p(argc, argv, opt);
    if(!p.parse()){
        std::cerr << "!p.parse() == true" << std::endl;
        p.fullusage();
        //return 1;
    }else if(help){
        p.fullusage();
    }
    //p.set_discribeusage([]() -> bool { return mydescribe(); });
    //p.fullusage();
    std::cout << "help == " << std::boolalpha << help << std::endl;
    std::cout << "quiet == " << std::boolalpha << quiet << std::endl;
    std::cout << "install == " << std::boolalpha << install << std::endl;
    std::cout << "lst == " << std::boolalpha << lst << std::endl;
    std::cout << "info == " << std::boolalpha << info << std::endl;
    std::cout << "opp == " << std::boolalpha << to_str(opp) << std::endl;
    std::cout << "opp2 == " << std::boolalpha << to_str(opp2) << std::endl;
    std::cout << "file == " << std::boolalpha << file << std::endl;
    std::cout << "x == " << std::boolalpha << x << std::endl;
    std::cout << "y == " << std::boolalpha << y << std::endl;
    std::cout << "vect == [";
    for(auto e : vect) std::cout << std::boolalpha << e << ", ";
    std::cout << "]" << std::endl;
    std::cout << "v == [";
    for(auto e : v) std::cout << std::boolalpha << e << ", ";
    std::cout << "]" << std::endl;
}

