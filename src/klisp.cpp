#include<stdexcept>
#include<iostream>
#include<sstream>
#include<iomanip>
#include<cctype>
#include<memory>
#include<variant>
#include<vector>
#include<string>
#include<cmath>
#include<stack>
#include<map>
#include<any>


// -*-----------------------------------------------------------------*-
// -*- begin::namesapce::klisp                                       -*-
// -*-----------------------------------------------------------------*-
namespace klisp{
// -
class Object;

using Self = std::shared_ptr<Object>;

template<typename T>
using Vec = std::vector<T>;

template<typename K, typename V>
using HashMap = std::map<K, V>;

using Str = std::string;
using f64 = double;

// enum Val => Object
enum class Kind{
    Nil,
    List,
    Number,
    Symbol,
    //! @todo : Add support for String
    Fun,
};

// -------------
// -*- Error -*-
// -------------
class Error{
public:
    enum Kind {Default, SyntaxError, ValueError, TypeError, RuntimeError};

    explicit Error(const Str& msg) noexcept
    : m_kind{Error::Kind::Default}, m_message{msg} {}

    explicit Error(Error::Kind kind, const Str& msg) noexcept
    : m_kind{kind}, m_message{msg} {}
    ~Error() = default;

    Str describe(void) const{
        std::stringstream ss;
        switch(this->m_kind){
        case Error::Kind::Default:
            ss << "Error: ";
            break;
        case Error::Kind::TypeError:
            ss << "TypeError: ";
            break;
        case Error::Kind::ValueError:
            ss << "ValueError: ";
            break;
        case Error::Kind::SyntaxError:
            ss << "SyntaxError: ";
            break;
        case Error::Kind::RuntimeError:
            ss << "RuntimeError: ";
            break;
        }

        ss << this->m_message;

        return ss.str();
    }

private:
    Error::Kind m_kind;
    Str m_message;

};

// --------------
// -*- Object -*-
// --------------
class Callable;
// class Symbol;
// class String;

class Object{
public:
    explicit Object() noexcept
    : m_kind{Kind::Nil}
    , m_val{}
    {}

    explicit Object(Vec<Self> vec) noexcept
    : m_kind{Kind::List}
    , m_val{vec}
    {} // List

    explicit Object(Str sym) noexcept
    : m_kind{Kind::Symbol}
    , m_val{sym}
    {} // String | Symbol

    explicit Object(f64 num) noexcept
    : m_kind{Kind::Number}
    , m_val{num}
    {} // Number

    explicit Object(Callable callable) noexcept;

    Object(const Object& other) noexcept
    : m_kind{other.m_kind}
    , m_val{other.m_val}{}

    Object(Object&& other) noexcept
    : m_kind{std::move(other.m_kind)}
    , m_val{std::move(other.m_val)}
    {}

    Object& operator=(const Object& other) noexcept{
        if(this != &other){
            this->m_kind = other.m_kind;
            this->m_val = other.m_val;
        }
        return *this;
    }

    Object& operator=(Object&& other) noexcept{
        if(this != &other){
            this->m_kind = std::move(other.m_kind);
            this->m_val = std::move(other.m_val);
        }
        return *this;
    }

    ~Object() = default;

    Kind kind(void) const { return this->m_kind; }
    f64 as_float(void) const {
        if(this->m_kind == Kind::Number){
            return std::any_cast<f64>(this->m_val);
        }
        throw Error(
            Error::Kind::TypeError, "Cannot cast this object to a number."
        );
    }

    Str as_symbol(void) const{
        if(this->m_kind == Kind::Symbol){
            return std::any_cast<Str>(this->m_val);
        }
        throw Error(
            Error::Kind::TypeError, "Cannot cast this object to a symbol."
        );
    };

    Callable as_fun(void) const;

    Vec<Self> as_list() const {
        if(this->m_kind == Kind::List){
            return std::any_cast<Vec<Self>>(this->m_val);
        }
        throw Error(
            Error::Kind::TypeError, "Object is not a list."
        );
    }

    static Object toSymbol(bool val){
        return val ? Object::True() : Object::False();
    }

    Str str(void) const{
        return format(*this);
    }

    Str repr(void) const{
        std::stringstream ss;
        ss << "Object(" << format(*this) << ")";
        return ss.str();
    }

    static Object True(void){
        return Object("#t");
    }

    static Object False(void){
        return Object("#f");
    }

    // -
    bool is_false(void) const {
        if(this->m_kind == Kind::Symbol){
            auto xstr = this->as_symbol();
            return (xstr == "#f");
        }
        return false;
    }

    bool is_number(void) const {
        return this->m_kind == Kind::Number;
    }

    bool is_symbol(void) const{
        return this->m_kind == Kind::Symbol;
    }

    void value(f64& num) const{
        num = std::any_cast<f64>(this->m_val);
    }

    void value(Vec<Self>& vec) const{
        vec = std::any_cast<Vec<Self>>(this->m_val);
    }

    void value(Str& sym) const{
        sym = std::any_cast<Str>(this->m_val);
    }

    friend Str format(const Vec<Object>& vec){
        std::stringstream ss;
        ss << "(";
        for(const auto& item: vec){
            ss << " " << format(item);
        }
        ss << ")";
        return ss.str();
    }

    friend Str format(const Object& obj);

    friend void println(const Object& obj){
        std::cout << format(obj) << std::endl;
    }

private:
    Kind m_kind;
    std::any m_val;
};

class Env{
public:
    explicit Env() noexcept
    : m_bindings{}, m_parent{nullptr}{}

    explicit Env(Env* parent) noexcept
    : m_bindings{}, m_parent{parent}{}

    Env(const Env& env) noexcept
    : m_bindings{env.m_bindings}, m_parent{env.m_parent}{}

    Env& operator=(const Env& env) noexcept{
        if(this != &env){
            this->m_bindings = env.m_bindings;
            this->m_parent = env.m_parent;
        }
        return *this;
    }

    Env(Env&& env) noexcept
    : m_bindings{std::move(env.m_bindings)}
    , m_parent{std::move(env.m_parent)}
    {}

    Env& operator=(Env&& env) noexcept{
        if(this != &env){
            this->m_bindings = std::move(env.m_bindings);
            this->m_parent = std::move(env.m_parent);
        }
        return *this;
    }

    ~Env() = default;

    Str str(void) const{
        std::stringstream ss;
        ss << "{";
        for(const auto& [key, val]: this->m_bindings){
            ss << "  " << key << ": " << val.str() << "\n";
        }
        ss << "}" << std::endl;
        return ss.str();
    }


    Str repr(void) const{
        std::stringstream ss;
        ss << "Env" << this->str();
        return ss.str();
    }

    bool contains(Str name) const{
        auto entry = this->m_bindings.find(name);
        if(entry != this->m_bindings.end()){
            return true;
        }else{
            if(this->m_parent == nullptr){
                return false;
            }else{
                return this->m_parent->contains(name);
            }
        }
    }

    // -
    Object get(Str name) const{
        auto iter = this->m_bindings.find(name);
        if(iter != this->m_bindings.end()){
            return iter->second;
        }else{
            if(this->m_parent == nullptr){
                std::stringstream ss;
                ss << "undefined variable '" << name << "'";
                throw Error(Error::Kind::RuntimeError, ss.str());
            }
            return this->m_parent->get(name);
        }
    }

    void put(Str name, Object obj){
        auto iter = this->m_bindings.find(name);
        if(iter != this->m_bindings.end()){
            std::stringstream ss;
            ss << "Cannot defined variable '" << name
               << "'. It is already defined in this scope.";
            throw Error(Error::Kind::ValueError, ss.str());
        }
        this->m_bindings[name] = obj;
    }

    void update(Str name, Object obj){
        auto iter = this->m_bindings.find(name);
        if(iter == this->m_bindings.end()){
            std::stringstream ss;
            ss << "Cannot update value of variable '" << name
               << "'. It not defined in this scope.";
            throw Error(Error::Kind::ValueError, ss.str());
        }
        this->m_bindings[name] = obj;
    }

private:
    HashMap<Str, Object> m_bindings;
    Env* m_parent;
};


class Callable final{
public:
    using ArithFun = f64 (*)(f64, f64);
    using NullaryFun = Object(*)(void);
    using UnaryFun = Object (*)(Object);
    using BinaryFun = Object (*)(Object, Object);
    using VFun = Object(*)(Vec<Object>);

    using Fun = std::variant<ArithFun, NullaryFun, UnaryFun, BinaryFun, VFun>;

    explicit Callable(Vec<Object> params, Object body, Env env) noexcept
    : m_params{params}
    , m_body{body}
    , m_env{&env}
    , m_kind{FunKind::UDEF}
    , m_fun{}
    {}

    explicit Callable(NullaryFun fun) noexcept
    : m_params{}
    , m_body{}
    , m_env{}
    , m_kind{FunKind::NULLARY}
    , m_fun{fun}
    {}

    explicit Callable(UnaryFun fun) noexcept
    : m_params{}
    , m_body{}
    , m_env{}
    , m_kind{FunKind::UNARY}
    , m_fun{fun}
    {};

    explicit Callable(BinaryFun fun) noexcept
    : m_params{}
    , m_body{}
    , m_env{}
    , m_kind{FunKind::BINARY}
    , m_fun{fun}
    {}

    explicit Callable(ArithFun fun) noexcept
    : m_params{}
    , m_body{}
    , m_env{}
    , m_kind{FunKind::ARITH}
    , m_fun{fun}
    {}

    explicit Callable(VFun fun) noexcept
    : m_params{}
    , m_body{}
    , m_env{}
    , m_kind{FunKind::VFUN}
    , m_fun{fun}
    {}

    ~Callable() = default;

    Object operator()(Vec<Object> args){
        return this->call(args);
    }

    bool is_arith(void) const{ return this->m_kind == FunKind::ARITH; }
    bool is_nullary(void) const{ return this->m_kind == FunKind::NULLARY; }
    bool is_unary(void) const{ return this->m_kind == FunKind::UNARY; }
    bool is_binary(void) const{ return this->m_kind == FunKind::BINARY; }
    bool is_user_defined(void) const{
        return this->m_kind == FunKind::UDEF;
    }

    bool is_vfun(void) const{ return this->m_kind == FunKind::VFUN; }

    Str str(void) const{
        std::stringstream ss;
        if(this->is_user_defined()){
            ss << "(lambda (";
            for(const auto& param: this->m_params){
                ss << param.str() << " ";
            }
            ss << ")\n";
            ss << this->m_body.str() << ")";
        }else{
            ss << "<builtin: @ ";
            if(this->is_arith()){
                auto fn = std::get<ArithFun>(this->m_fun);
                ss << fn;
            }else if(this->is_binary()){
                auto fn = std::get<BinaryFun>(this->m_fun);
                ss << fn;
            }else if(this->is_nullary()){
                auto fn = std::get<NullaryFun>(this->m_fun);
                ss << fn;
            }else if(this->is_unary()){
                auto fn = std::get<UnaryFun>(this->m_fun);
                ss << fn;
            }else if(this->is_vfun()){}
            ss << ">";
        }
        return ss.str();
    }

protected:
    Object call(Vec<Object> argv);

private:
    Vec<Object> m_params;
    Object m_body;
    Env m_env;

    enum class FunKind {UDEF, NULLARY, UNARY, BINARY, ARITH, VFUN};

    FunKind m_kind;
    Fun m_fun;
};


Object::Object(Callable callable) noexcept
: m_kind{Kind::Fun}
, m_val{callable}
{}

Callable Object::as_fun(void) const{
    if(this->m_kind == Kind::Fun){
        return std::any_cast<Callable>(this->m_val);
    }
    throw Error(Error::Kind::TypeError, "Object is not a callable.");
}

// -*-
Str format(const Object& obj){
    switch(obj.kind()){
    case Kind::List:{
        auto vec = std::any_cast<Vec<Self>>(obj.m_val);
        std::stringstream ss;
        ss << "(";
        for(const auto& item: vec){
            ss << " " << format(*item);
        }
        ss << ")";
        return ss.str();
    }
    case Kind::Number:
        return std::to_string(std::any_cast<f64>(obj.m_val));
    case Kind::Symbol:
        return std::any_cast<Str>(obj.m_val);
    case Kind::Fun:{
            auto fun = std::any_cast<Callable>(obj.m_val);
            return fun.str();
        }
    default: // Nil
        return "";
    }
}

// -*-
class Klisp{
public:
    explicit Klisp() noexcept = default;
    ~Klisp() = default;

    static void run(void){
        runtime = initialize_runtime();
        repl(runtime);
    }

    static void repl(Env& env){
        while(true){
            std::cout << "\x1[32mklisp\x1b[m>> ";
            try{
                auto src = Klisp::read_expr();
                if(src == "quit" || src == ":q"){
                    std::exit(EXIT_SUCCESS);
                }
                repl_helper(src, env);
            }catch(const Error& err){
                std::cerr << err.describe() << std::endl;
            }catch(const std::runtime_error& err){
                std::cerr << err.what() << std::endl;
            }catch(...){
                std::cerr << "unknown error detected." << std::endl;
                std::abort();
            }
        }
    }

    // -
    static void repl_helper(Str src, Env& env){
        auto result = Klisp::eval(Klisp::parse(src), env);
        std::cout << "==> ";
        println(result);
    }

    static Object eval(Object obj, Env& env){
        Object result{};
        switch(obj.kind()){
        case Kind::Symbol:{
                Str name{};
                obj.value(name);
                result = env.get(name);
            }
            break;
        case Kind::Number:{
                f64 num{};
                obj.value(num);
                result = Object(num);
            }
            break;
        case Kind::List:{ // function call or special-form s-expression
                Vec<Self> vec{};
                obj.value(vec);
                if(vec.size() == 0){
                    result = Object();
                    break;
                }
                auto self = vec[0];
                Vec<Object> args{};
                for(auto iter=vec.begin()+1; iter != vec.end(); iter++){
                    args.push_back(*iter->get());
                }
                if(self->kind() != Kind::Symbol){
                    throw Error(Error::Kind::SyntaxError, "unknown list form.");
                }

                /**
                SPECIAL FORMS
                    quote, if, define, fun, progn,

                @todo: Support the following special forms
                    let, unquote, quasiquote, unquote-splicing,
                    var | setq, for, 
                */
                Str name{};
                self->value(name);
                if(name == "quote"){
                    result = args[0];
                }else if(name == "if"){
                    if(args.size() < 2){
                        throw Error(
                            Error::Kind::SyntaxError, "malformed `if` form."
                        );
                    }
                    auto test = args[0];
                    auto ok = args[1];
                    decltype(ok) alt{};
                    if(args.size() == 2){
                        alt = Object();
                    }else{
                        alt = args[2];
                    }
                    auto rv = Klisp::eval(test, env);
                    auto expr = !rv.is_false() ? ok : alt;

                    result = Klisp::eval(expr, env);
                }else if(name == "define"){
                    if(args.size() != 2){
                        throw Error(
                            Error::SyntaxError,
                            "invalid number of argument to `define` special form"
                        );
                    }
                    auto _key = args[0];
                    if(_key.kind() != Kind::Symbol){
                        throw Error(
                            Error::SyntaxError,
                            "malformed `define` special form. The first "
                            "argument must by a symbol."
                        );
                    }
                    auto key = _key.as_symbol();
                    auto val = Klisp::eval(args[1], env);
                    env.put(key, val);
                    result = Object::False();
                }else if(name == "progn"){
                    for(const auto& arg: args){
                        result = Klisp::eval(arg, env);
                    }
                }else if(name == "fun"){
                    //! @todo:
                    auto fname = args[0];
                    if(fname.kind() != Kind::Symbol){
                        throw Error(
                            Error::Kind::SyntaxError, "function name must be a symbol."
                        );
                    }
                    auto _params = args[1].as_list();
                    Vec<Object> params{};
                    for(const auto& self: _params){
                        params.push_back(*self.get());
                    }

                    auto body = args[2];
                    auto fun = Callable(params, body, env);
                }else{
                    // function call.
                    decltype(args) argv{};
                    for(const auto& arg: args){
                        argv.push_back(Klisp::eval(arg, env));
                    }

                    result = Klisp::call(name, argv);
                }
            }
            break;
        default:
            result = Object();
        }

        return result;
    }

    static Str read_expr(void){
        char c;
        std::cin >> c;
        if(c == '('){
            // std::cin.unget();
            std::stack<char> parens{};
            parens.push(c);
            Str sexpr{};
            sexpr += c;
            while(!parens.empty()){
                std::cin >> c;
                sexpr += c;
                if(c == ')'){ parens.pop(); }
                else if(c == '('){ parens.push(c); }
            };
            return sexpr;
        }else{
            Str sexpr{};
            std::getline(std::cin >> std::ws, sexpr);
            return sexpr;
        }
    }

    // -
    static Vec<Str> tokenize(const Str& src){
        Vec<Str> tokens{};
        auto begin = src.begin();
        auto end = src.end();
        int pos{0};

        // -
        auto skipWhitespace = [&]() mutable {
            if((begin + pos >= end)){
                return;
            }
            auto c = *(begin + pos);
            while(std::isspace(c)){
                ++pos;
                if((begin + pos) >= end){ break; }
                c = *(begin + pos);
            }
        };

        // -
        auto skipComment = [&]() mutable {
            if((begin + pos) >= end){ return; }
            auto c = *(begin + pos);
            if(c == ';'){
                while(c != '\n'){
                    ++pos;
                    c = *(begin + pos);
                }
            }
        };

        // -
        auto getToken = [&]() mutable -> Str {
            Str token{};
            skipWhitespace();
            skipComment();
            auto c = *(begin + pos);
            while(!std::isspace(c)){
                token += c;
                ++pos;
                if((begin + pos) >= end){ break; }
                c = *(begin + pos);
            }

            return token;
        };


        // -
        while((begin + pos) < end){
            auto c = *(begin + pos);
            if(c == ';'){
                skipComment();
            }
            auto tok = getToken();
            tokens.push_back(tok);
        }

        return tokens;
    }

    // -
    static Object parse(const Str& src){
        auto tokens = Klisp::tokenize(src);
        return read(tokens);
    }

    // -
    static Object read(Vec<Str>& tokens){
        if(tokens.size() == 0){
            return Object();
        }
        auto beg = tokens.begin();
        auto end = tokens.end();
        auto token = tokens[0];
        if(token == "("){
            Vec<Self> vec{};
            Vec<Str> parens{};
            parens.push_back("(");
            int nxt{0};
            while(parens.size() != 0){
                ++nxt;
                token = *(beg + nxt);
                if(token == ")"){
                    parens.pop_back();
                    continue;
                }
                try{ // if- token is a number
                    auto num = std::stod(token);
                    vec.push_back(std::make_shared<Object>(num));
                }catch(...){
                    vec.push_back(std::make_shared<Object>(token));
                }
            }
            ++nxt;
            if((beg + nxt) >= end){
                throw Error("unexpected EOF");
            }
            token = *(beg + nxt); // next token
            return Object(vec);
        }else if(token == ")"){
            throw Error("unexpect ')' while reading.");
        }else{
            return atom(token);
        }
    }

    // -
    static Object atom(Str token){
        try{
            auto num = std::stod(token);
            return Object(num);
        }catch(const std::invalid_argument& err){
            return Object(token);
        }
    }

    // -*-
    static Env runtime;
    
private:
    // -
    static Env prelude;
    static Env initialize_prelude(void);

    static Env initialize_runtime(void){
        return Env(Klisp::prelude);
    }

    static Object call(Str fn, Vec<Object> argv){
        if(!Klisp::runtime.contains(fn)){
            std::stringstream ss;
            ss << "unbound function name `" << fn << "'.";
            throw Error(Error::Kind::RuntimeError, ss.str());
        }
        auto _fun = Klisp::runtime.get(fn);
        auto fun = _fun.as_fun();
        return fun(argv);
    }
};

// -
Env Klisp::prelude = Env();
Env Klisp::runtime = Env();

// -
f64 fn_add(f64 x, f64 y){
    return x + y;
}

f64 fn_sub(f64 x, f64 y){
    return x - y;
}

f64 fn_div(f64 x, f64 y){
    if(y == 0.0){
        throw Error(Error::Kind::ValueError, "division by zero.");
    }
    return x / y;
}

f64 fn_mul(f64 x, f64 y){
    return x * y;
}

f64 fn_mod(f64 x, f64 y){
    if(y == 0.0){
        throw Error(Error::Kind::ValueError, "division by zero.");
    }
    return std::fmod(x, y);
}

// -
Object fn_less(Object lhs, Object rhs){
    auto x = lhs.as_float();
    auto y = rhs.as_float();
    return Object::toSymbol((x < y));
}

Object fn_less_eq(Object lhs, Object rhs){
    auto x = lhs.as_float();
    auto y = rhs.as_float();
    return Object::toSymbol((x <= y));
}

Object fn_greater(Object lhs, Object rhs){
    auto x = lhs.as_float();
    auto y = rhs.as_float();
    return Object::toSymbol((x > y));
}

Object fn_greater_eq(Object lhs, Object rhs){
    auto x = lhs.as_float();
    auto y = rhs.as_float();
    return Object::toSymbol((x >= y));
}

Object fn_equal(Object lhs, Object rhs){
    if(lhs.kind() != rhs.kind()){
        throw Error(
            Error::Kind::TypeError,
            "operands type mismatched for operator `=`"
        );
    }
    if(lhs.is_number()){
        auto x = lhs.as_float();
        auto y = rhs.as_float();
        return Object::toSymbol((x == y));
    }else if(lhs.is_symbol()){
        auto xsym = lhs.as_symbol();
        auto ysym = rhs.as_symbol();
        return Object::toSymbol((xsym==ysym));
    }else{
        throw Error(
            Error::Kind::SyntaxError,
            "operator `=` not supported for list"
        );
    }
}


Object fn_list(Object args){
    if(args.kind() == Kind::List){
        return args;
    }
    Vec<Self> result{};

    result.push_back(std::make_shared<Object>(args));
    return Object(result);
}

Object fn_not(Object arg){
    auto val = !arg.is_false();
    return Object::toSymbol(val);
}

Object fn_or(Vec<Object> args){
    bool flag{false};
    for(const auto& arg: args){
        if(!arg.is_false()){
            flag = true;
            break;
        }
    }

    return flag ? Object::True() : Object::False();
}

Object fn_and(Vec<Object> args){
    bool flag{true};
    for(const auto& arg: args){
        if(arg.is_false()){
            flag = false;
            break;
        }
    }

    return flag ? Object::True() : Object::False();
}

Object fn_println(Vec<Object> args){
    for(const auto arg: args){
        std::cout << arg.str() << std::endl;
    }
    return Object();
}


// for now, only read-in numbers
Object fn_input(Object prompt){
    Str numstr;
    if(prompt.kind() == Kind::Nil){
        std::cin >> numstr;
    }else if(prompt.kind() == Kind::Symbol){
        std::cout << prompt.str();
        std::cin >> numstr;
    }

    Object result{};
    try{
        result = Object(std::stod(numstr));
    }catch(const std::invalid_argument& err){
        throw Error(Str{err.what()});
    }catch(const std::out_of_range& err){
        throw Error(Str{err.what()});
    }catch(...){
        throw Error("unexpected error occured.");
    }

    return result;
}

Object fn_quit(){
    std::exit(0);
    return Object(); // never reached
}

// -
Env Klisp::initialize_prelude(void){
    Env env{};
    env.put("pi", Object(3.141592653589793));
    env.put("+", Object(Callable(fn_add)));
    env.put("-", Object(Callable(fn_sub)));
    env.put("*", Object(Callable(fn_mul)));
    env.put("/", Object(Callable(fn_div)));
    env.put("%", Object(Callable(fn_mod)));
    env.put("=", Object(Callable(fn_equal)));
    env.put("<", Object(Callable(fn_less)));
    env.put(">", Object(Callable(fn_greater)));
    env.put("<=", Object(Callable(fn_less_eq)));
    env.put(">=", Object(Callable(fn_greater_eq)));
    env.put("not", Object(Callable(fn_not)));
    env.put("or", Object(Callable(fn_or)));
    env.put("and", Object(Callable(fn_and)));
    env.put("println", Object(Callable(fn_println)));
    env.put("input", Object(Callable(fn_input)));
    env.put("quit", Object(Callable(fn_quit)));

    return env;
}

// -*-
Object Callable::call([[maybe_unused]] Vec<Object> args){
    if(this->m_params.size() != args.size()){
        std::stringstream ss;
        ss << "invalid number of arguments. Expcected "
           << this->m_params.size() << ", got " << args.size();
        throw Error(Error::Kind::ValueError, ss.str());
    }else if(this->m_kind != Callable::FunKind::UDEF){
        if(this->is_nullary()){
            auto fun = std::get<Callable::NullaryFun>(this->m_fun);
            return fun();
        }else if(this->is_unary()){
            auto fun = std::get<Callable::UnaryFun>(this->m_fun);
            return fun(args[0]);
        }else if(this->is_binary()){
            auto fun = std::get<Callable::BinaryFun>(this->m_fun);
            auto lhs = args[0];
            auto rhs = args[1];
            return fun(lhs, rhs);
        }else{
            auto fun = std::get<Callable::ArithFun>(this->m_fun);
            auto x = args[0].as_float();
            auto y = args[1].as_float();
            auto num = fun(x, y);
            return Object(num);
        }
    }else{// 
        auto env = Env(this->m_env);
        for(auto i=0; i < m_params.size(); i++){
            auto item = m_params[i];
            if(item.kind() != Kind::Symbol){
                throw Error(
                    Error::Kind::TypeError, "param names must be symbols"
                );
            }
            env.put(item.as_symbol(), args[i]);
        }
        return Klisp::eval(this->m_body, env);
    }
}



// -*-----------------------------------------------------------------*-
}//-*- end::namespace::klisp                                         -*-
// -*-----------------------------------------------------------------*-
