/*
 * =====================================================================================
 *
 *       Filename:  gzzopt.hpp
 *
 *    Description:  my go at get opts
 *
 *        Version:  1.0
 *        Created:  Tuesday 2014-10-14 18:25:30
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Francis Grizzly Smit (FGJS), grizzlysmit@smit.id.au
 *   Organization:  Me
 *
 * =====================================================================================
 */

#include <string>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <list>
#include <vector>
#include <initializer_list>
#include <functional>
#include <tuple>

namespace gzzopts {
    class OptError : public std::exception {
        private:
            std::string _msg;
        public:
            OptError(std::string msg) : _msg(msg) {};
    };
    class NoVaribleProvided : public OptError {
        public:
            NoVaribleProvided(std::string msg) : OptError(msg) {};
    };
    class NoFunctionProvided : public OptError {
        public:
            NoFunctionProvided(std::string msg) : OptError(msg) {};
    };
    class basic_var {
        protected:
            bool is_set = false;
        public:
            bool is_set_var() { return is_set; };
            virtual bool set_value(const std::string& val) = 0;
            virtual basic_var* copy() = 0;
            virtual ~basic_var() {};
            virtual const bool is_novar() const { return false; };
            const bool isset() const { return is_set; };
    };
    template<class T>
        class Var : public basic_var {
            private:
                using value_type = T;
                T* var = nullptr;
            public:
                Var(T &v) : var(&v) { };
                virtual bool set_value(const std::string& val){
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here\tval == " << val << std::endl;
                    if(!var) throw NoVaribleProvided("No Variable provided");
                    std::istringstream is(val);
                    is >> *var;
                    is_set = true;
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here\tis.eof() == " << is.eof() << std::endl;
                    return is.eof();
                };
                virtual basic_var* copy(){
                    return new Var(*var);
                };
                virtual ~Var(){
                    //if(var) delete var;
                };
        };
    template<>
        class Var<bool> : public basic_var {
            private:
                using value_type = bool;
                bool* var = nullptr;
                //bool is_set = false;
            public:
                Var(bool &v) : var(&v) { };
                virtual bool set_value(const std::string& val){
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                    if(!var) throw NoVaribleProvided("No Variable provided");
                    *var = true;
                    is_set = true;
                    return true;
                };
                virtual basic_var* copy(){
                    return new Var(*var);
               };
                virtual ~Var(){
                    //if(var) delete var;
                };
        };
    template<class T>
        class Var<std::list<T>> : public basic_var {
            private:
                using value_type = std::list<T>;
                value_type* var = nullptr;
                //bool is_set = false;
            public:
                Var(value_type &v) : var(&v) { };
                virtual bool set_value(const std::string& val){
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                    if(!var) throw NoVaribleProvided("No Variable provided");
                    std::istringstream is(val);
                    T t;
                    is >> t;
                    var->push_back(t);
                    is_set = true;
                    return is.eof();
                };
                virtual basic_var* copy(){
                    return new Var(*var);
               };
                virtual ~Var(){
                    //if(var) delete var;
                };
        };
    template<class T>
        class Var<std::vector<T>> : public basic_var {
            private:
                using value_type = std::vector<T>;
                value_type* var = nullptr;
               //bool is_set = false;
            public:
                Var(value_type &v) : var(&v) { };
                virtual bool set_value(const std::string& val){
                    //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                    if(!var) throw NoVaribleProvided("No Variable provided");
                    std::istringstream is(val);
                    T t;
                    is >> t;
                    var->push_back(t);
                    is_set = true;
                    return is.eof();
                };
                virtual basic_var* copy(){
                    return new Var(*var);
               };
                virtual ~Var(){
                    //if(var) delete var;
                };
        };
    using function_void = std::function<bool()>;
    using function_str  = std::function<bool(const std::string& )>;
    class NullVar : public basic_var {
        private:
        public:
            NullVar() {};
            virtual bool set_value(const std::string&){
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                //if(!func) throw NoFunctionProvided("No Function provided");
                return true;
            };
            virtual basic_var* copy(){
                return new NullVar;
            };
            virtual ~NullVar(){ };
            virtual const bool is_novar() const { return true; };
    };
    class Func0 : public basic_var {
        private:
            function_void func = nullptr;
        public:
            Func0(function_void f) : func(f) {};
            virtual bool set_value(const std::string&){
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                if(!func) throw NoFunctionProvided("No Function provided");
                return func();
            };
            virtual basic_var* copy(){
                return new Func0(func);
            };
            virtual ~Func0(){ };
    };
    class Func : public basic_var {
        private:
            function_str func = nullptr;
        public:
            Func(function_str f) : func(f) {};
            virtual bool set_value(const std::string& val){
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here" << std::endl;
                if(!func) throw NoFunctionProvided("No Function provided");
                return func(val);
            };
            virtual basic_var* copy(){
                return new Func(func);
            };
            virtual ~Func(){ };
    };
    class Opts;
    class OptionSpec {
        private:
            friend class Opts;
            basic_var   *var = nullptr;
            std::string _description;
            std::string _long;
            char        _short = '\0';
            bool        _expects_arg = false;
            bool        _multi = false;
            bool        _manditory = false;
            bool        _integer = false;
            bool        _positional = false;
            bool        _literal = false;
            bool        _cut = false;
            std::vector<Opts*> _rest;
        public:
            OptionSpec(OptionSpec&& other)
                : _description(other._description), _long(other._long), _short(other._short), _expects_arg(other._expects_arg), _multi(other._multi), _manditory(other._manditory), _integer(other._integer), _positional(other._positional), _literal(other._literal), _cut(other._cut)
            {
                //std::cerr << __FILE__ << "[" << __LINE__ << "] got here: var == " << var << " other.var == " << other.var << " _long == " << _long << std::endl;
                std::swap(var, other.var);
                //std::cerr << __FILE__ << "[" << __LINE__ << "] got here: var == " << var << " other.var == " << other.var << " _long == " << _long << std::endl;
                //std::swap(_rest.data(), other._rest.data());
                //*
                //std::cerr << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
                _rest.erase(_rest.begin(), _rest.end());
                //std::cerr << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
                for(auto r : other._rest){
                    //std::cerr << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
                    _rest.push_back(r);
                    //std::cerr << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
                }
                //*/
            };
            OptionSpec(const OptionSpec& other)
                : var(other.var->copy()), _description(other._description), _long(other._long), _short(other._short), _expects_arg(other._expects_arg), _multi(other._multi), _manditory(other._manditory),
                _integer(other._integer), _positional(other._positional), _literal(other._literal), _cut(other._cut), _rest(other._rest) {
                //std::cerr << __FILE__ << "[" << __LINE__ << "] got here: var == " << var << " other.var == " << other.var << " _long == " << _long << std::endl;
            };
            OptionSpec& operator=(OptionSpec&& rhs){
                //std::cerr << __FILE__ << "[" << __LINE__ << "] got here: var == " << var << " rhs.var == " << rhs.var << std::endl;
                std::swap(var, rhs.var);
                //std::cerr << __FILE__ << "[" << __LINE__ << "] got here: var == " << var << " rhs.var == " << rhs.var << std::endl;
                _description = rhs._description;
                _long = rhs._long;
                _short = rhs._short;
                _expects_arg = rhs._expects_arg;
                _multi = rhs._multi;
                _manditory = rhs._manditory;
                _integer = rhs._integer;
                _positional = rhs._positional;
                //std::swap(_rest, rhs._rest);
                //std::cerr << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
                _rest.erase(_rest.begin(), _rest.end());
                //std::cerr << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
                for(auto r : rhs._rest){
                    //std::cerr << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
                    _rest.push_back(r);
                    //std::cerr << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
                }
                std::swap(_literal, rhs._literal);
                std::swap(_cut, rhs._cut);
                return *this;
            };
            template<class T>
                OptionSpec(T& v, const std::string desc, const std::string long_opt, const char short_opt = '\0', bool multi = false, bool manditory = false, bool integer = false, bool positional = false)
                  : var(new Var<T>(v)), _description(desc), _long(long_opt), _short(short_opt), _expects_arg(false), _multi(multi), _manditory(manditory), _integer(integer), _positional(positional), _literal(false), _cut(false) {
                    //
                };
            template<class T>
                OptionSpec(T& v, const char* desc, const char* long_opt, const char short_opt = '\0', bool multi = false, bool manditory = false, bool integer = false, bool positional = false)
                  : var(new Var<T>(v)), _description(desc), _long(long_opt), _short(short_opt), _expects_arg(false), _multi(multi), _manditory(manditory), _integer(integer), _positional(positional), _literal(false), _cut(false) {
                    //
                };
            OptionSpec(bool& v, const char* desc, const char* long_opt, const char short_opt = '\0', bool positional = false, bool manditory = false)
              : var(new Var<bool>(v)), _description(desc), _long(long_opt), _short(short_opt), _expects_arg(false), _multi(false), _manditory(manditory), _integer(false), _positional(positional), _literal(false), _cut(false) {
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here: var == " << var << " _long == " << _long << std::endl;
            };
            OptionSpec(bool& v, const std::string desc, const std::string long_opt, const char short_opt = '\0', bool positional = false, bool manditory = false)
              : var(new Var<bool>(v)), _description(desc), _long(long_opt), _short(short_opt), _expects_arg(false), _multi(false), _manditory(manditory), _integer(false), _positional(positional), _literal(false), _cut(false) {
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here: var == " << var << " _long == " << _long << std::endl;
            };
            template<class T>
                OptionSpec(std::list<T>& v, const char* desc, const char* long_opt, const char short_opt = '\0', bool positional = false, bool manditory = false)
                  : var(new Var<std::list<T>>(v)), _description(desc), _long(long_opt), _short(short_opt), _expects_arg(false), _multi(true), _manditory(manditory), _integer(false), _positional(positional), _literal(false), _cut(false) {
                };
            template<class T>
                OptionSpec(std::list<T>& v, const std::string desc, const std::string long_opt, const char short_opt = '\0', bool positional = false, bool manditory = false)
                  : var(new Var<std::list<T>>(v)), _description(desc), _long(long_opt), _short(short_opt), _expects_arg(false), _multi(true), _manditory(manditory), _integer(false), _positional(positional), _literal(false), _cut(false) {
                };
            template<class T>
                OptionSpec(std::vector<T>& v, const char* desc, const char* long_opt, const char short_opt = '\0', bool positional = false, bool manditory = false)
                  : var(new Var<std::vector<T>>(v)), _description(desc), _long(long_opt), _short(short_opt), _expects_arg(false), _multi(true), _manditory(manditory), _integer(false), _positional(positional), _literal(false), _cut(false) {
                };
            template<class T>
                OptionSpec(std::vector<T>& v, const std::string desc, const std::string long_opt, const char short_opt = '\0', bool positional = false, bool manditory = false)
                  : var(new Var<std::vector<T>>(v)), _description(desc), _long(long_opt), _short(short_opt), _expects_arg(false), _multi(true), _manditory(manditory), _integer(false), _positional(positional), _literal(false), _cut(false) {
                };
            OptionSpec(function_void f, const char* desc, const char* long_opt, const char short_opt = '\0', bool multi = false, bool manditory = false)
              : var(new Func0(f)), _description(desc), _long(long_opt), _short(short_opt), _expects_arg(false), _multi(multi), _manditory(manditory), _integer(false), _positional(false), _literal(false), _cut(false) {
                //
            };
            OptionSpec(function_void f, const std::string desc, const std::string long_opt, const char short_opt = '\0', bool multi = false, bool manditory = false)
              : var(new Func0(f)), _description(desc), _long(long_opt), _short(short_opt), _expects_arg(false), _multi(multi), _manditory(manditory), _integer(false), _positional(false), _literal(false), _cut(false) {
                //
            };
            OptionSpec(function_str f, const char* desc, const char* long_opt, const char short_opt = '\0', bool multi = false, bool manditory = false)
              : var(new Func(f)), _description(desc), _long(long_opt), _short(short_opt), _expects_arg(false), _multi(multi), _manditory(manditory), _integer(false), _positional(false), _literal(false), _cut(false) {
                //
            };
            OptionSpec(function_str f, const std::string desc, const std::string long_opt, const char short_opt = '\0', bool multi = false, bool manditory = false)
              : var(new Func(f)), _description(desc), _long(long_opt), _short(short_opt), _expects_arg(false), _multi(multi), _manditory(manditory), _integer(false), _positional(false), _literal(false), _cut(false) {
                //
            };
            OptionSpec(std::initializer_list<Opts*> opts) : var(new NullVar)/*, _rest(opts)*/ {
                //std::cerr << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
                _rest.erase(_rest.begin(), _rest.end());
                //std::cerr << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
                for(Opts* o : opts){
                    //std::cerr << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
                    _rest.push_back(o);
                    //std::cerr << __FILE__ << "[" << __LINE__ << "] got here" << std::endl;
                }
            };
            ~OptionSpec(){
                //std::cerr << __FILE__ << "[" << __LINE__ << "] got here: var == " << var << "  _long == " << _long << std::endl;
                //if(var) delete var;
            };
            basic_var* get_var() {
                //std::cerr << __FILE__ << "[" << __LINE__ << "] got here: var == " << var << "  _long == " << _long << std::endl;
                return var;
            };
            const std::string description() const { return _description; };
            const std::string long_opt() const {
                //std::cerr << __FILE__ << "[" << __LINE__ << "] got here: var == " << var << "  _long == " << _long << std::endl;
                return _long;
            };
            const char short_opt() const { return _short; };
            const bool expects_arg() const { return _expects_arg; };
            OptionSpec& set_expects_arg(const bool expects) { _expects_arg = expects; return *this; };
            const bool multi() const { return _multi; };
            const bool integer() const { return _integer; };
            const bool manditory() const { return _manditory; };
            OptionSpec& set_manditory(bool mand) { _manditory = mand; return *this; };
            const bool positional() const { return _positional; };
            OptionSpec& set_positional(bool pos) { _positional = pos; return *this; };
            const bool literal() const { return _literal; };
            OptionSpec& set_literal(bool lit) { _literal = lit; return *this; };
            const bool cut() const { return _cut; };
            OptionSpec& set_cut(bool ct) { _cut = ct; return *this; };
            const std::vector<Opts*>& rest() const {
                return _rest;
            };
            const bool is_novar() const { return var->is_novar(); };
    }; // class OptionSpec  //
    template<class T>
        OptionSpec positional(T& v, const std::string desc, const std::string name, bool multi = false){
            return OptionSpec(v, desc, name, '\0', multi).set_positional(true).set_manditory(true);
        };
    template<class T>
        OptionSpec positional(std::list<T>& v, const char* desc, const char* name){
            return OptionSpec(v, desc, name, '\0', true).set_positional(true).set_manditory(false);
        };
    template<class T>
        OptionSpec positional(std::vector<T>& v, const char* desc, const char* name){
            return OptionSpec(v, desc, name, '\0', true).set_positional(true).set_manditory(false);
        };
    extern OptionSpec positional(bool& v, const char* desc, const char* name);
    extern OptionSpec positional(bool& v, const std::string desc, const std::string& name);
    extern OptionSpec positional(function_void f, const std::string desc, const std::string& name);
    extern OptionSpec positional(function_str f, const std::string desc, const std::string& name);
    template<class T>
        OptionSpec literal(T& v, const std::string desc, const std::string name, bool multi = false){
            return OptionSpec(v, desc, name, '\0', multi).set_positional(true).set_manditory(true).set_literal(true).set_cut(true);
        };
    extern OptionSpec literal(bool& v, const char* desc, const char* name);
    extern OptionSpec literal(bool& v, const std::string desc, const std::string& name);
    extern OptionSpec literal(function_void f, const std::string desc, const std::string& name);
    extern OptionSpec literal(function_str f, const std::string desc, const std::string& name);
    class OptionParser;
    using str_pair_vec = std::vector<std::tuple<std::string, std::string>>;
    class Opts {
        public:
        private:
            std::vector<OptionSpec> specs;
            std::string progname;
        public:
            Opts(Opts&&) = default;
            Opts(Opts&) = default;
            //Opts(OptionSpec&&) { };
            Opts(std::initializer_list<OptionSpec> lst) : specs(lst){
                //
            };
            bool parse(OptionParser* op, std::string progname, std::vector<std::string> args, std::vector<std::string>::size_type& i, bool no_more_opts, bool inner);
            bool inner_parse(OptionParser* op, std::string progname, char o, std::vector<std::string> args, std::vector<std::string>::size_type& i);
            bool usage(std::string progname) const;
            bool discribeusage(str_pair_vec& out, std::string::size_type &width, const bool seg_desc) const;
    }; // class Opts //
    extern bool USAGE(const std::string& progname, const Opts opts);
    extern bool DISCRIBEUSAGE(const Opts opts, const bool seg_desc);
    class OptionParser {
        private:
            std::string progname;
            std::vector<std::string> args;
            Opts opts;
            std::vector<std::string> _bad_opt_lst;
            bool _stored_result = true;
            function_str  _usage         = [this](const std::string& progname) -> bool { return USAGE(progname, this->opts); };
            function_void _discribeusage = [this]()                            -> bool { return DISCRIBEUSAGE(this->opts, this->_segment_discription); };
            function_void _where         = []()                                -> bool { std::cerr << "\n\nwhere:" << std::endl; return std::cerr.good(); };
            function_str  _fullusage     = [this](const std::string& progname) -> bool { return _usage(progname) && _where() &&  _discribeusage(); };
            bool _segment_discription    = true;
            Opts* _winner = nullptr;
        public:
            OptionParser(OptionSpec&&) = delete;
            OptionParser(OptionParser&&) = delete;
            OptionParser(int argc, char *argv[], Opts opt) : progname(argv[0]), opts(opt) {
                if(argc > 1){
                    args.insert(args.begin(), &argv[1], argv + argc);
                }

            };
            bool parse();
            bool usage();
            bool fullusage();
            bool discribeusage();
            void set_usage(function_str f) { _usage = f; };
            void set_fullusage(function_str f) { _fullusage = f; };
            void set_where(function_void f) { _where = f; };
            void set_discribeusage(function_void f) { _discribeusage = f; };
            const bool segment_discription() const { return _segment_discription; };
            void set_segment_discription(bool seg_desc) { _segment_discription = seg_desc; };
            void set_bad_opt_lst(std::string opt_name, bool result){
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\topt_name == " << opt_name << "\tresult == " << std::boolalpha << result << "\t_stored_result == " << _stored_result << std::endl;
                _bad_opt_lst.push_back(opt_name);
                _stored_result = _stored_result && result;
                //std::cerr << __FILE__ << '[' << __LINE__ << "]\tgot here:\topt_name == " << opt_name << "\tresult == " << std::boolalpha << result << "\t_stored_result == " << _stored_result << std::endl;
            };
            bool stored_result() const { return _stored_result; };
            void set_stored_result(bool result) { _stored_result = _stored_result && result; };
            const Opts* winner() const { return _winner; };
            void set_winner(Opts* o) { _winner = o; };
    }; // class OptionParser //
} // namespace gzzopts //
