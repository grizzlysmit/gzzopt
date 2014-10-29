/*
 * =====================================================================================
 *
 *       Filename:  gzzopt.cpp
 *
 *    Description:  get opts parser
 *
 *        Version:  1.0
 *        Created:  Tuesday 2014-10-14 19:33:28
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Francis Grizzly Smit (FGJS), grizzlysmit@smit.id.au
 *   Organization:  Me
 *
 * =====================================================================================
 */

#include "gzzopt.hpp"
#include <regex>
#include <iostream>
#include <algorithm>

gzzopts::OptionSpec gzzopts::positional(bool& v, const char* desc, const char* name){
    return OptionSpec(v, desc, name, '\0', true).set_positional(true).set_manditory(true);
};
gzzopts::OptionSpec gzzopts::positional(bool& v, const std::string desc, const std::string& name){
    return OptionSpec(v, desc, name, '\0', true).set_positional(true).set_manditory(true);
};
gzzopts::OptionSpec gzzopts::positional(function_void f, const std::string desc, const std::string& name){
    return OptionSpec(f, desc, name, '\0', false).set_positional(true).set_manditory(true);
};
gzzopts::OptionSpec gzzopts::positional(function_str f, const std::string desc, const std::string& name){
    return OptionSpec(f, desc, name, '\0', false).set_positional(true).set_manditory(true);
};

gzzopts::OptionSpec gzzopts::literal(bool& v, const char* desc, const char* name){
    return OptionSpec(v, desc, name, '\0', true).set_positional(true).set_manditory(true).set_literal(true).set_cut(true);
};
gzzopts::OptionSpec gzzopts::literal(bool& v, const std::string desc, const std::string& name){
    return OptionSpec(v, desc, name, '\0', true).set_positional(true).set_manditory(true).set_literal(true).set_cut(true);
};
gzzopts::OptionSpec gzzopts::literal(function_void f, const std::string desc, const std::string& name){
    return OptionSpec(f, desc, name, '\0', false).set_positional(true).set_manditory(true).set_literal(true).set_cut(true);
};
gzzopts::OptionSpec gzzopts::literal(function_str f, const std::string desc, const std::string& name){
    return OptionSpec(f, desc, name, '\0', false).set_positional(true).set_manditory(true).set_literal(true).set_cut(true);
};

bool gzzopts::Opts::parse(OptionParser* op, std::string progname, std::vector<std::string> args, std::vector<std::string>::size_type& i, bool no_more_opts, bool inner){
    using namespace std;
    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tinner == " << std::boolalpha << inner << std::endl;
    
    std::regex option_matcher("^--([[:alpha:]][-_[:alpha:]]+)(=(.*))?$|^-([a-zA-Z]+)$");

    std::regex option_specifier("^(([a-zA-Z]),)?([a-zA-Z][-_a-zA-Z]+)$");

    std::vector<std::tuple<std::vector<std::string>::size_type, std::vector<OptionSpec>::size_type>> backtrace;

    std::vector<OptionSpec>::size_type current = 0;

    bool res = true, break_loop = false;

    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
    for( ; i < args.size(); i++){
        if(!no_more_opts && args[i] == "--"s){
            no_more_opts = true;
            continue;
        }
        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tcurrent == " << current << "\tspecs.size() == " << specs.size() << std::endl;
        while(current < specs.size() && !specs[current].positional()){ // find the next positional //
            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tcurrent == " << current << "\ti == " << i << std::endl;
            if(specs[current].rest().size() > 0){
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tcurrent == " << current << "\ti == " << i << std::endl;
                backtrace.insert(backtrace.begin(), std::make_tuple(i, current));
            }
            if(specs[current].is_novar()){
                current++;
                continue;
            }
            current++;
        }
        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tcurrent == " << current << "\tspecs.size() == " << specs.size() << std::endl;
        if(current >= specs.size()){ // overshot the end of the current Opts specification so try the alternatives //
            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tthis == " << this << "\top->winner() == " << op->winner() << std::endl;
            if(this == op->winner()){
                std::cerr << "to many arguments for this command line: [";
                for(std::vector<std::string>::size_type m = i; m < args.size(); m++) std::cerr << args[m] << ", ";
                std::cerr << "]" << std::endl;
                return false; // too many args //
            }
        }
        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
        std::smatch result;
        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
        std::regex_match(args[i], result, option_matcher);
        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
        if(result.empty() || no_more_opts){
            // positional //
            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tcurrent == " << current << "\ti == " << i << std::endl;
            while(current < specs.size() && !specs[current].positional()){ // find the next positional //
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tcurrent == " << current << "\ti == " << i << std::endl;
                if(specs[current].rest().size() > 0){
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tcurrent == " << current << "\ti == " << i << std::endl;
                    backtrace.insert(backtrace.begin(), std::make_tuple(i, current));
                }
                if(specs[current].is_novar()){
                    current++;
                    continue;
                }
                current++;
            }
            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
            if(current >= specs.size()){ // overshot the end of the current Opts specification so try the alternatives //
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tthis == " << this << "\top->winner() == " << op->winner() << std::endl;
                if(this == op->winner()){
                    std::cerr << "to many arguments for this command line: [";
                    for(std::vector<std::string>::size_type m = i; m < args.size(); m++) std::cerr << args[m] << ", ";
                    std::cerr << "]" << std::endl;
                    return false; // too many args //
                }
                for(auto p : backtrace){
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                    std::vector<std::string>::size_type j;
                    std::vector<OptionSpec>::size_type k;
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                    std::tie(j, k) = p;
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tj == " << j << "k == " << k << std::endl;
                    for(Opts* o : specs[k].rest()){
                        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\to == " << o << std::endl;
                        res = o->parse(op, progname, args, j, no_more_opts, true);
                        if(res){
                            break_loop = true;
                        }
                    }
                    if(break_loop){
                        break_loop = false; // for next time //
                        break;
                    }
                }
                break;
                //return res;  // out of options //
            }
            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
            if(specs[current].positional()){
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tcurrent == " << current << std::endl;
                if(specs[current].literal()){
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tspecs[current].long_opt() == " << specs[current].long_opt() << std::endl;
                    std::regex literal("^(" + specs[current].long_opt() + ")$");
                    std::smatch result2;
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                    std::regex_match(args[i], result2, literal);
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                    if(result2.empty()){
                        // no match to expected literal //
                        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                        res = false;
                        for(auto p : backtrace){
                            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                            std::vector<std::string>::size_type j;
                            std::vector<OptionSpec>::size_type k;
                            std::tie(j, k) = p;
                            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tj == " << j << "k == " << k << std::endl;
                            for(Opts* o : specs[k].rest()){
                                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\to == " << o << std::endl;
                                res = o->parse(op, progname, args, j, no_more_opts, true);
                                if(res){
                                    break_loop = true;
                                    break;
                                }
                            }
                            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tj == " << j << "k == " << k << std::endl;
                            if(break_loop){
                                break_loop = false; // for next time //
                                break;
                            }
                        }
                        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tres == " << std::boolalpha << res << std::endl;
                        return res;
                    }else{
                        basic_var* var = specs[current].get_var();
                        if(var->set_value(args[i])){
                            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tspecs[current].long_opt() == " << specs[current].long_opt() << std::endl;
                            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tspecs[current].cut() == " << std::boolalpha << specs[current].cut() << std::endl;
                            if(specs[current].no_more_opts()){
                                no_more_opts = true;
                            }
                            if(specs[current].cut()){
                                op->set_winner(this);
                            }
                            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tthis == " << this << "\top->winner() == " << op->winner() << std::endl;
                            current++; // literals should never be multi so move to the next //
                            res = true;
                            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tinner == " << std::boolalpha << inner << std::endl;
                            continue;
                        }else{
                            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tinner == " << std::boolalpha << inner << std::endl;
                            return false; // failed to find the literal expected //
                        }
                        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tinner == " << std::boolalpha << inner << std::endl;
                    }
                }else{
                    // value of some kind //
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tinner == " << std::boolalpha << inner << std::endl;
                    basic_var* var = specs[current].get_var();
                    if(var->set_value(args[i])){
                        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tinner == " << std::boolalpha << inner << std::endl;
                        if(specs[current].no_more_opts()){
                            no_more_opts = true;
                        }
                        if(specs[current].cut()){
                            op->set_winner(this);
                        }
                        if(!specs[current].multi()) current++; // used up move to next arg if any //
                        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tinner == " << std::boolalpha << inner << std::endl;
                        if(current >= specs.size() && i + 1 < args.size()){ // overshot the end of the current Opts specification so try the alternatives //
                            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tinner == " << std::boolalpha << inner << std::endl;
                            for(auto p : backtrace){
                                std::vector<std::string>::size_type j;
                                std::vector<OptionSpec>::size_type k;
                                std::tie(j, k) = p;
                                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tinner == " << std::boolalpha << inner << "\tj == " << j << "\tk == " << k << std::endl;
                                for(Opts* o : specs[k].rest()){
                                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tinner == " << std::boolalpha << inner << "\to == " << o << std::endl;
                                    res = o->parse(op, progname, args, j, no_more_opts, true);
                                    if(res){
                                        break_loop = true;
                                        break;
                                    }
                                }
                                if(break_loop){
                                    break_loop = false; // for next time //
                                    break;
                                }
                            }
                            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tthis == " << this << "\top->winner() == " << op->winner() << std::endl;
                            if(this == op->winner()){
                                std::cerr << "to many arguments for this command line: [";
                                for(std::vector<std::string>::size_type m = i; m < args.size(); m++) std::cerr << args[m] << ", ";
                                std::cerr << "]" << std::endl;
                                res = false; // too many args //
                                op->set_stored_result(res);
                            }
                            return res;  // out of options //
                        }
                        continue;
                    }else{
                        return false; // failed to find the literal expected //
                    }
                }
            }else{ // positional but no spec found so check the alternative specs //
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                for(auto p : backtrace){
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                    std::vector<std::string>::size_type j;
                    std::vector<OptionSpec>::size_type k;
                    std::tie(j, k) = p;
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                    for(Opts* o : specs[k].rest()){
                        res = o->parse(op, progname, args, j, no_more_opts, true);
                        if(res){
                            break_loop = true;
                            break;
                        }
                    }
                    if(break_loop){
                        break_loop = false; // for next time //
                        break;
                    }
                }
                return res; // out of options //
            }
        }else{ // if(result.empty() || no_more_opts) //
            while(current < specs.size() && !specs[current].positional()){ // find the next positional //
                if(specs[current].rest().size() > 0){
                    backtrace.insert(backtrace.begin(), std::make_tuple(i, current));
                }
                if(specs[current].is_novar()){
                    current++;
                    continue;
                }
                current++;
            }
            // found an option arg --name or -char(s) arg //
            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
            if(args[i].substr(0, 2) == "--"s){
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                bool finished = false;
                std::string a = result[1];
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                std::vector<OptionSpec>::size_type j = 0;
                for( ; j < current; j++){
                    if(specs[j].is_novar() || specs[j].positional()){
                        continue;
                    }
                    if(specs[j].is_novar() || specs[j].positional()){
                        continue;
                    }
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                    if(a == specs[j].long_opt()){
                        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                        std::string value = result[3];
                        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                        if(value == ""s && specs[j].expects_arg()){
                            if(result[2] != "="s){
                                if(i + 1 < args.size()){
                                    i++;
                                    value = args[i];
                                }else{
                                    std::cerr << "no value for arg: --" << a << " " << value << std::endl;
                                    return false;
                                }
                            }
                        }
                        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tvalue == " << value << std::endl;
                        basic_var* var = specs[j].get_var();
                        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here: var == " << var << std::endl;
                        if(var->set_value(value)){
                            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                            res = true;
                            if(specs[j].no_more_opts()){
                                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                                no_more_opts = true;
                            }
                            if(specs[j].cut()){
                                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                                op->set_winner(this);
                            }
                        }else{
                            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                            if(specs[j].expects_arg()){
                                std::cerr << "bad value for arg: --" << a << "=" << value << std::endl;
                            }else{
                                std::cerr << "falied to set arg: --" << a << std::endl;
                            }
                            res = false;
                        }
                        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                        finished = true;
                        break;
                    }
                }
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                if(finished){
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tres == " << std::boolalpha << res << std::endl;
                    finished = false; // for next time //
                    if(i + 1 >= args.size()){
                        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                        return res;
                    }
                    op->set_stored_result(res);
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                    continue;
                }
                for(auto p : backtrace){
                    std::vector<std::string>::size_type j;
                    std::vector<OptionSpec>::size_type k;
                    std::tie(j, k) = p;
                    for(Opts* o : specs[k].rest()){
                        res = o->parse(op, progname, args, j, no_more_opts, true);
                        if(res){
                            break_loop = true;
                            break;
                        }
                    }
                    if(break_loop){
                        break_loop = false; // for next time //
                        break;
                    }
                }
                if(!inner){
                    op->set_bad_opt_lst("--"s + a, res);
                }
                return res; // out of options //
            }else if(args[i].substr(0, 1) == "-"s){
                //
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tresult.size() == " << result.size() << std::endl;
                //for(std::string r : result) std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tr == " << r << std::endl;
                std::string a = result[4];
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\ta == " << a << std::endl;
                bool finished = false;
                for(char o : a){
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\to == " << o << std::endl;
                    std::vector<OptionSpec>::size_type j = 0;
                    for( ; j < current; j++){
                        if(specs[j].is_novar() || specs[j].positional()){
                            continue;
                        }
                        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\to == -" << o << std::endl;
                        if(o == specs[j].short_opt()){
                            std::string value;
                            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\to == -" << o << "\tvalue == " << value << std::endl;
                            if(specs[j].expects_arg()){
                                if(i + 1 < args.size()){
                                    i++;
                                    value = args[i];
                                }else{
                                    std::cerr << "no value for arg: -" << o << " " << value << std::endl;
                                    return false;
                                }
                                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\to == -" << o << "\tvalue == " << value << std::endl;
                            }
                            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\to == -" << o << "\tvalue == " << value << std::endl;
                            basic_var* var = specs[j].get_var();
                            if(var->set_value(value)){
                                res = true;
                                finished = true;
                                if(specs[j].no_more_opts()){
                                    no_more_opts = true;
                                }
                                if(specs[j].cut()){
                                    op->set_winner(this);
                                }
                                break;
                            }else{
                                if(specs[j].expects_arg()){
                                    std::cerr << "bad value for arg: -" << o << " " << value << std::endl;
                                }else{
                                    std::cerr << "falied to set arg: -" << o << std::endl;
                                }
                                res = false;
                                finished = true;
                                break;
                            }
                        }
                    }
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\to == -" << o << "\tfinished == " << std::boolalpha << finished << std::endl;
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\to == -" << o << "\tres == " << std::boolalpha << res << std::endl;
                    if(finished){
                        finished = false; // for next time //
                        //if(i + 1 >= args.size()) return res;
                        //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\to == -" << o << std::endl;
                        op->set_stored_result(res);
                        continue;
                    }
                    if(j >= current){
                        for(auto p : backtrace){
                            std::vector<std::string>::size_type l;
                            std::vector<OptionSpec>::size_type k;
                            std::tie(l, k) = p;
                            for(Opts* opt : specs[k].rest()){
                                res = opt->inner_parse(op, progname, o, args, l);
                                if(res){
                                    break_loop = true;
                                    break;
                                }
                            }
                            if(break_loop){
                                break_loop = false; // for next time //
                                break;
                            }
                        }
                    }
                    if(!res){
                        op->set_bad_opt_lst("-"s + o, res);
                    }
                }
                if(inner) return res;
            }else{
                std::cerr << "error: never should have got here faulty algorithm " << args[i] << " doe not start with - or -- bailing" << std::endl;
                return false;
            }
        }
    } // for( ; i < args.size(); i++) //
    //current++;
    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tcurrent == " << current << "\ti == " << i << std::endl;
    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tspecs.size() == " << specs.size() << "\targs.size() == " << args.size() << std::endl;
    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tthis == " << this << "\top->winner() == " << op->winner() << std::endl;
    if(this == op->winner()){
        if(current >= specs.size() && i < args.size()){
            std::cerr << "to many arguments for this command line: [";
            for(std::vector<std::string>::size_type m = i; m < args.size(); m++) std::cerr << args[m] << ", ";
            std::cerr << "]" << std::endl;
            return false; // too many args //
        }
        if(current < specs.size() && i >= args.size()){
            res = true;
            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tres == " << res << "\ti == " << i << std::endl;
            while(current < specs.size()){
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tcurrent == " << current << "\ti == " << i << std::endl;
                while(current < specs.size() && !specs[current].positional()){ // find the next positional //
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tcurrent == " << current << "\ti == " << i << std::endl;
                    if(specs[current].is_novar()){
                        current++;
                        continue;
                    }
                    current++;
                }
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tcurrent == " << current << "\tspecs.size() == " << specs.size() << std::endl;
                //if(current < specs.size()) std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tspecs[current].manditory() == " << specs[current].manditory() << "\tspecs[current].get_var()->isset() == " << specs[current].get_var()->isset() << std::endl;
                //if(current < specs.size()) std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tspecs[current].long_opt() == " << specs[current].long_opt() << "\tspecs[current].get_var()->isset() == " << specs[current].get_var()->isset() << std::endl;
                if(current < specs.size() && specs[current].manditory() && !specs[current].get_var()->isset()){
                    std::cerr << "nmanditory arg: " << specs[current].long_opt() << " not found" << std::endl;
                    res = false;
                    op->set_stored_result(res);
                }
                current++;
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tcurrent == " << current << "\tres == " << std::boolalpha << res << std::endl;
            }
            //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tres == " << std::boolalpha << res << "\ti == " << i << std::endl;
            return res;
        }
    }
    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tres == " << std::boolalpha << res << "\tinner == " << inner << std::endl;
    //if(!inner && i >= args.size()) return true;
    if(i >= args.size()){
        return true;
    }else{
        if(current >= specs.size()){
            return true;
        }else{
            /*
            std::cerr << "to many arguments for this command line: [";
            for(std::vector<std::string>::size_type m = i; m < args.size(); m++) std::cerr << args[m] << ", ";
            std::cerr << "]" << std::endl;
            // */
            return false;
        }
    }
}

bool gzzopts::Opts::inner_parse(OptionParser* op, std::string progname, char o, std::vector<std::string> args, std::vector<std::string>::size_type& i){

    std::vector<std::tuple<std::vector<std::string>::size_type, std::vector<OptionSpec>::size_type>> backtrace;

    std::vector<OptionSpec>::size_type current = 0;

    /*
    if(specs[current].rest().size() > 0){
        backtrace.insert(backtrace.begin(), std::make_tuple(i, current));
    }
    // */
    while(current < specs.size() && !specs[current].positional()){ // find the next positional //
        if(specs[current].rest().size() > 0){
            backtrace.insert(backtrace.begin(), std::make_tuple(i, current));
        }
        if(specs[current].is_novar()){
            current++;
            continue;
        }
        current++;
    }
    std::vector<OptionSpec>::size_type j = 0;
    for( ; j < current; j++){
        if(specs[j].is_novar() || specs[j].positional()){
            continue;
        }
        if(o == specs[j].short_opt()){
            std::string value;
            if(specs[j].expects_arg()){
                    if(i + 1 < args.size()){
                        i++;
                        value = args[i];
                    }else{
                        std::cerr << "no value for arg: -" << o << " " << value << std::endl;
                        return false;
                    }
            }
            basic_var* var = specs[j].get_var();
            if(var->set_value(value)){
                if(specs[j].cut()){
                    op->set_winner(this);
                }
                return true;
            }else{
                if(specs[j].expects_arg()){
                    std::cerr << "bad value for arg: -" << o << " " << value << std::endl;
                }else{
                    std::cerr << "falied to set arg: -" << o << std::endl;
                }
                return false;
            }
        }
    }
    if(j >= current){
        for(auto p : backtrace){
            std::vector<std::string>::size_type l;
            std::vector<OptionSpec>::size_type k;
            std::tie(l, k) = p;
            for(Opts* opt : specs[k].rest()){
                if(opt->inner_parse(op, progname, o, args, l)) return true;
            }
        }
    }
    return false;
}

bool gzzopts::OptionParser::parse(){
    std::vector<std::string>::size_type i = 0;
    bool result = opts.parse(this, progname, args, i, false, false);
    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\tresult == " << std::boolalpha << result << "\t_stored_result == " << _stored_result << std::endl;
    if(!_stored_result){
        for(auto o : _bad_opt_lst){
            std::cerr << "option not found:\t" << o << std::endl;
        }
    }
    result = _stored_result && result;
    return result;
}

bool gzzopts::OptionParser::usage(){
    return _usage(progname);
}

bool gzzopts::USAGE(const std::string& progname, const Opts opts){
    return opts.usage(progname);
}

bool gzzopts::DISCRIBEUSAGE(const Opts opts, const bool seg_desc){
    str_pair_vec out;
    std::string::size_type width = 1;
    opts.discribeusage(out, width, seg_desc);
    for(auto p : out){
        std::string opt_spec, desc;
        std::tie(opt_spec, desc) = p;
        std::cerr << "\t" << std::setw(width) << std::left << opt_spec << " : " << std::setw(0) << desc << "\n";
    }
    std::cerr << std::endl;
    return true;
}

bool gzzopts::Opts::usage(std::string progname) const {
    std::ostringstream os;
    os << progname;
    std::vector<std::tuple<std::string, std::vector<OptionSpec>::size_type>> backtrace;
    for(std::vector<OptionSpec>::size_type i = 0; i < specs.size(); i++){
        if(specs[i].rest().size() > 0){
            backtrace.insert(backtrace.begin(), std::make_tuple(os.str(), i));
        }
        if(specs[i].is_novar()) continue;
        if(specs[i].positional()){
            if(specs[i].literal()){
                if(specs[i].manditory()){
                    os << " " << specs[i].long_opt();
                }else{
                    os << " [" << specs[i].long_opt() << ']';
                }
            }else{
                if(specs[i].manditory()){
                    os << " " << '<' << specs[i].long_opt() << '>';
                }else{
                    os << " " << '[' << specs[i].long_opt() << ']';
                }
            }
            if(specs[i].multi()) os << "...";
        }else{
            if(specs[i].expects_arg()){
                if(specs[i].manditory()){
                    os << " --" << specs[i].long_opt() << '=' << specs[i].long_opt() << (specs[i].short_opt()? (std::string("|-") + specs[i].short_opt() + " " + specs[i].long_opt()):std::string(""));
                }else{
                    os << " [--" << specs[i].long_opt() << '=' << specs[i].long_opt() << (specs[i].short_opt()? (std::string("|-") + specs[i].short_opt() + " " + specs[i].long_opt()):std::string("")) << ']';
                }
            }else{
                if(specs[i].manditory()){
                    os << " --" << specs[i].long_opt() << (specs[i].short_opt()? (std::string("|-") + specs[i].short_opt()):std::string(""));
                }else{
                    os << " [--" << specs[i].long_opt() << (specs[i].short_opt()? (std::string("|-") + specs[i].short_opt()):std::string("")) << ']';
                }
            }
            if(specs[i].multi()) os << "...";
        }
    } // for(std::vector<OptionSpec>::size_type i = 0; i < specs.size(); i++) //
    os << std::flush;
    std::cerr << os.str() << std::endl;
    if(backtrace.size() > 0){
        for(auto p : backtrace){
            std::string uptillnow;
            std::vector<OptionSpec>::size_type i;
            std::tie(uptillnow, i) = p;
            //std::cout << uptillnow << "\t" << i << std::endl;
            for(Opts* o : specs[i].rest()){
                o->usage(uptillnow);
            }
        }
    }
    return os.good();
}

bool gzzopts::OptionParser::fullusage(){
    return _fullusage(progname);
}
bool gzzopts::OptionParser::discribeusage(){
    return _discribeusage();
}

bool gzzopts::Opts::discribeusage(str_pair_vec& out, std::string::size_type &width, const bool seg_desc) const {
    using namespace std;
    std::vector<std::vector<OptionSpec>::size_type> backtrace;
    for(std::vector<OptionSpec>::size_type i = 0; i < specs.size(); i++){
        std::string opt_spec, opt_desc;
        if(specs[i].rest().size() > 0){
            backtrace.insert(backtrace.begin(), i);
        }
        if(specs[i].is_novar()) continue;
        if(specs[i].positional()){
            if(specs[i].literal()){
                if(specs[i].manditory()){
                    opt_spec = specs[i].long_opt();
                }else{
                    opt_spec = "\t[" + specs[i].long_opt() + ']';
                }
            }else{
                if(specs[i].manditory()){
                    opt_spec = '<' + specs[i].long_opt() + '>';
                }else{
                    opt_spec = '[' + specs[i].long_opt() + ']';
                }
            }
        }else{
            if(specs[i].expects_arg()){
                if(specs[i].manditory()){
                    opt_spec = "--"s + specs[i].long_opt() + '=' + specs[i].long_opt() + (specs[i].short_opt()? (std::string("|-") + specs[i].short_opt() + " " + specs[i].long_opt()):std::string(""));
                }else{
                    opt_spec = "[--"s + specs[i].long_opt() + '=' + specs[i].long_opt() + (specs[i].short_opt()? (std::string("|-") + specs[i].short_opt() + " " + specs[i].long_opt()):std::string("")) + "]"s;
                }
            }else{
                if(specs[i].manditory()){
                    opt_spec = "--"s + specs[i].long_opt() + (specs[i].short_opt()? (std::string("|-") + specs[i].short_opt()):std::string(""));
                }else{
                    opt_spec = "[--"s + specs[i].long_opt() + (specs[i].short_opt()? (std::string("|-") + specs[i].short_opt()):std::string("")) + "]"s;
                }
            }
        }
        if(specs[i].multi()) opt_spec += "..."s;
        width = std::max(width, opt_spec.length());
        opt_desc = specs[i].description();
        out.push_back(std::make_tuple(opt_spec, opt_desc));
    } // for(std::vector<OptionSpec>::size_type i = 0; i < specs.size(); i++) //
    if(seg_desc) out.push_back(std::make_tuple(""s, ""s));
    if(backtrace.size() > 0){
        for(auto i : backtrace){
            //std::cout << uptillnow << "\t" << i << std::endl;
            for(Opts* o : specs[i].rest()){
                o->discribeusage(out, width, seg_desc);
            }
        }
    }
    return true;
}

