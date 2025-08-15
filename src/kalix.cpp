// kalix.cpp -*-
// Based on wisp.cpp
#include<filesystem>
#include<exception>
#include<stdexcept>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<memory>
#include<random>
#include<limits>
#include<vector>
#include<string>
#include<cctype>
#include<ctime>
#include<cmath>
#include<stack>
#include<map>
#include<any>
#include<set>

#define KLX_ERRORS()                            \
    KLX_DEF(Error, "Error")                     \
    KLX_DEF(TypeError, "TypeError")             \
    KLX_DEF(ValueError, "ValueError")           \
    KLX_DEF(IndexError, "IndexError")           \
    KLX_DEF(RuntimeError, "RuntimeError")       \
    KLX_DEF(SyntaxError, "SyntaxError")


#define KLX_TYPES()                 \
    KLX_DEF(Nil, "nil")             \
    KLX_DEF(Bool, "bool")           \
    KLX_DEF(Integer, "integer")     \
    KLX_DEF(Float, "float")         \
    KLX_DEF(String, "string")       \
    KLX_DEF(CFun, "builtin")        \
    KLX_DEF(Fun, "function")        \
    KLX_DEF(Lambda, "lambda")       \
    KLX_DEF(Symbol, "symbol")       \
    KLX_DEF(List, "list")


#define KLX_UNUSED(arg)  (void)arg


namespace fs = std::filesystem;

// -*----------------------------------------------------------------*-
// -*- begin::namespace::klx                                        -*-
// -*----------------------------------------------------------------*-
namespace klx{
    class Object; // Value
    class Env;
    // struct Quote;
    struct Lambda;
    struct Builtin;
    struct Fun;

    using Str = std::string;
    template<typename T>
    using Vec = std::vector<T>;
    template<typename T>
    using Shared = std::shared_ptr<T>;
    template<typename T>
    using UniquePtr = std::unique_ptr<T>;
    using Self = UniquePtr<Object>;
    using f64 = double;
    using i64 = std::int64_t;
    using u32 = std::uint32_t;
    using usize = std::size_t;

    template<typename K, typename V>
    using Dict = std::map<K, V>;
    using Args = Vec<Object>;
    using ArrayList = Vec<Object>;

    using CFun = Object (*)(Args, Env&);

    // -*-
    enum class TypeKind{
#define KLX_DEF(tok, _) tok,
        KLX_TYPES()
#undef KLX_DEF
    };

    // - Type names
    Dict<TypeKind, Str> types = {
#define KLX_DEF(tok, name) {TypeKind::tok, name},
        KLX_TYPES()
#undef KLX_DEF
    };

    // -*-
    enum class ErrorKind{
#define KLX_DEF(tok, _) tok,
        KLX_ERRORS()
#undef KLX_DEF
    };

    // -*- Error messages -*-
    Dict<ErrorKind, Str> errors = {
#define KLX_DEF(tok, msg) {ErrorKind::tok, msg},
        KLX_ERRORS()
#undef KLX_DEF
    };
} // namespace::klx

namespace klx::utils{
// -
klx::Str readfile(klx::Str filename){
    std::ifstream fin(filename);
    if(!fin){
        std::stringstream ss;
        ss << "unable to open file `" << filename << "'";
        throw std::runtime_error(ss.str());
    }
    fin.seekg(0, std::ios::end);
    std::string data{};
    data.reserve(fin.tellg());
    fin.seekg(0, std::ios::beg);
    data.assign(
        std::istreambuf_iterator<char>(fin),
        std::istreambuf_iterator<char>()
    );
    fin.close();
    return data;
}

klx::Str input(klx::u32 id, std::string prefix){
    auto level = [](u32 n){
        if(n==0){ return; }
        for(decltype(n) i=0; i < n; ++i){
            std::cout << "    ";
        }
    };
    std::cout << "\x1b[92m" << prefix << "\x1b[0m:\x1b[94m";
    std::cout << id << "\x1b[0m>> ";
    std::string src{};
    char c;
    std::cin >> c;
    u32 nlevel{};
    if(c == '('){
        ++nlevel;
        src += c;
        std::stack<char> parens{};
        parens.push(c);
        while(!parens.empty()){
            std::cin >> c;
            src += c;
            if(c == ')'){
                --nlevel;
                parens.pop();
            }else if(c == '('){
                parens.push('(');
                ++nlevel;
            }else if(c == '\n'){
                level(nlevel);
            }
        }
    }else{
        std::cin.unget();
        std::getline(std::cin >> std::ws, src);
    }
    std::cout << std::endl;
    return src;
}

void output(klx::u32 id, Str prefix, Str data){
    std::cout << "\x1b[93m" << prefix << "\x1b[0m:\x1b[94m";
    std::cout << id << "\x1b[0m>> " << data << std::endl;
}

// -
void replace(klx::Str& src, klx::Str old, klx::Str neo){
    usize i{0};
    for(i = src.find(old, i); i != Str::npos; i=src.find(old, i)){
        src.replace(i, old.size(), neo);
        i += neo.size();
    }
}

// -
bool is_valid_symbol_char(char c){
    return (
        (std::isalpha(c) || std::ispunct(c)) &&
        c != '(' && c != ')' && c != '\'' && c != '"'
    );
}

// -
}// namespace::klx::utils

// -*-
namespace klx{
// -
// -----------
// -*- Env -*-
// -----------
class Env{
public:
    explicit Env() noexcept = default;
    explicit Env(Env* parent) noexcept;
    Env(const Env& other) noexcept;
    Env& operator=(const Env& other) noexcept;
    ~Env() = default;
    bool contains(Str name) const;
    Object get(Str name) const;
    void put(Str name, Object obj);
    void update(Str name, Object obj);

    Env* parent(void);
    const Env* parent(void) const;

    friend bool operator==(const Env& lhs, const Env& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Env& env);

private:
    Dict<Str, Object> m_bindings;
    Env* m_parent;
};


// -------------
// -*- Error -*-
// -------------
class Error{
public:
    explicit Error(ErrorKind kind, const Str& msg) noexcept;
    explicit Error(Object self, const Str& msg) noexcept;
    explicit Error(ErrorKind kind, Object self, const Str& msg) noexcept;
    Error(const Error& other) noexcept = delete;
    Error& operator=(const Error&) noexcept = delete;
    Error(Error&& other) noexcept;
    Error& operator=(Error&& other) noexcept;
    ~Error() = default;

    Str describe(void) const;

private:
    ErrorKind m_kind;
    Self m_reason;
    Str m_msg;
};

// -*-
struct Symbol{
    Str value;
    Symbol(Str val): value{val}{}
    friend bool operator==(const Symbol lhs, const Symbol& rhs){
        return lhs.value == rhs.value;
    }
    friend bool operator!=(const Symbol lhs, const Symbol& rhs){
        return !(lhs == rhs);
    }
};

// --------------
// -*- Object -*-
// --------------
class Object{
public:
    explicit Object() noexcept;
    explicit Object(bool flag) noexcept;
    explicit Object(i64 num) noexcept;
    explicit Object(f64 num) noexcept;
    explicit Object(ArrayList xs) noexcept;
    explicit Object(const Symbol& symbol) noexcept;
    explicit Object(const Str& str) noexcept;
    explicit Object(const Lambda& lambda) noexcept;
    explicit Object(const Builtin& builtin) noexcept;
    explicit Object(const Fun& fun) noexcept;

    Object(const Object& other) noexcept;
    Object(Object&& other) noexcept;
    Object& operator=(const Object& other) noexcept;
    Object& operator=(Object&& other) noexcept;
    ~Object() = default;

    // -*-
    Vec<Symbol> used_symbols(void) const;
    bool is_nil(void) const {
        return this->m_kind == TypeKind::Nil;
    }
    bool is_builtin(void) const;
    bool is_function(void) const{ return this->m_kind == TypeKind::Fun; }
    bool is_number(void) const;
    bool is_integer(void) const;
    bool is_float(void) const;
    bool is_string(void) const;
    bool is_symbol(void) const;
    bool is_callable(void) const;
    bool is_lambda(void) const;
    bool is_list(void) const;
    bool is_bool(void) const;

    bool as_bool(void) const;
    i64 as_integer(void) const;
    f64 as_float(void) const;
    Str as_string(void) const;
    Symbol as_symbol(void) const;
    ArrayList as_list(void) const;
    Lambda as_lambda(void) const;
    Builtin as_builtin(void) const;
    Fun as_function(void) const;

    Object to_integer(void) const;
    Object to_float(void) const;
    Object to_bool(void) const;
    Object to_string(void) const;

    Object apply(Args args, Env& env);
    Object eval(Env& env);

    void push(Object obj);
    Object pop(void);

    // -*- COMPARAISON OPERATIONS -*-
    friend bool operator==(const Object& lhs, const Object& rhs);
    friend bool operator!=(const Object& lhs, const Object& rhs);
    friend bool operator>=(const Object& lhs, const Object& rhs);
    friend bool operator<=(const Object& lhs, const Object& rhs);
    friend bool operator>(const Object& lhs, const Object& rhs);
    friend bool operator<(const Object& lhs, const Object& rhs);

    // -*- ARITHMETIC OPERATIONS. -*-
    friend Object operator+(const Object& lhs, const Object& rhs);
    friend Object operator-(const Object& lhs, const Object& rhs);
    friend Object operator*(const Object& lhs, const Object& rhs);
    friend Object operator/(const Object& lhs, const Object& rhs);
    friend Object operator%(const Object& lhs, const Object& rhs);

    Symbol type(void) const;
    Str str(void) const;
    Str repr(void) const;

    friend std::ostream& operator<<(std::ostream& os, const Object& obj);

    bool& has_doc(void){ return this->m_has_doc; }
    const bool& has_doc(void) const{ return this->m_has_doc; }

private:
    TypeKind m_kind;
    std::any m_val;
    bool m_has_doc{false};
};

struct Builtin{
    Str name;
    CFun fun;
    Builtin() = default;
    Builtin(Str key, CFun fn): name{key}, fun{fn}{}
    ~Builtin() = default;
};

struct Lambda{
    ArrayList params;
    Object body;
    Env env;
    Lambda() = default;
    Lambda(const ArrayList& p, const Object& obj, const Env& e)
    : params{p}, body{obj}, env{e}{}
    ~Lambda() = default;
};

struct Fun : public Lambda{
    Str name;
    Fun(Str name, const ArrayList& p, const Object& obj, const Env& e)
    : Lambda(p, obj, e)
    , name{name}{}
};


// ..............................
// -*- Tokenization & Parsing -*-
// ..............................
class Kalix{
public:
    explicit Kalix() noexcept = default;
    ~Kalix() = default;

    static Object run(Str src, Env& env);
    static void repl(Env& env);

    static Dict<Str, fs::path> modules;
    static Env runtime;
    static Env initialize_runtime(void);

    // -*-
    static Str version;
    static Str license;
    static Str banner;
    static Str help;
    static std::set<Str> keywords;
    static bool is_keyword(const Str& name);
    static Dict<Str, Str> documentation;
    /*
    lambda, if, define, fun, loop, for, progn, scope, quote, let, var
    ---
    true, false, nil
    */
    // -*-
    static Object handle_lambda(Args args, Env& env);
    static Object handle_if(Args args, Env& env);
    static Object handle_define(Args args, Env& env);
    static Object handle_fun(Args args, Env& env);
    static Object handle_while(Args args, Env& env);
    static Object handle_for(Args args, Env& env);
    static Object handle_progn(Args args, Env& env);    // do
    // static Object handle_scope(Args args, Env& env);
    static Object handle_quote(Args args, Env& env);
    static Object handle_let(Args args, Env& env);      // <added>
    static Object handle_var(Args args, Env& env);      // <added> == setq
    static Object handle_import(Args args, Env& env);

private:
    // -
    static Dict<Str, Object> prelude;

    static Dict<Str, Object> initialize_prelude(void);

    static void skip_whitespace(const Str& src, int& ptr);
    static Object parse(const Str& src, int& ptr);
    static ArrayList parse(const Str& src);
    static void eval_args(Args& args, Env& env);

    // -*-
    static Object fn_addpath(Args args, Env& env);
    static Object fn_help(Args args, Env& env);
    static Object fn_exit(Args args, Env& env);
    static Object fn_quit(Args args, Env& env);
    static Object fn_print(Args args, Env& env);
    static Object fn_println(Args args, Env& env);
    static Object fn_eprint(Args args, Env& env);
    static Object fn_eprintln(Args args, Env& env);
    static Object fn_panic(Args args, Env& env);
    static Object fn_input(Args args, Env& env);
    static Object fn_random(Args args, Env& env);
    static Object fn_read(Args args, Env& env);
    static Object fn_write(Args args, Env& env);
    static Object fn_eval(Args args, Env& env);
    static Object fn_list(Args args, Env& env);
    static Object fn_bool(Args args, Env& env);
    static Object fn_integer(Args args, Env& env);
    static Object fn_float(Args args, Env& env);
    static Object fn_string(Args args, Env& env);
    static Object fn_listp(Args args, Env& env);
    static Object fn_boolp(Args args, Env& env);
    static Object fn_integerp(Args args, Env& env);
    static Object fn_floatp(Args args, Env& env);
    static Object fn_stringp(Args args, Env& env);
    static Object fn_add(Args args, Env& env);
    static Object fn_sub(Args args, Env& env);
    static Object fn_mul(Args args, Env& env);
    static Object fn_div(Args args, Env& env);
    static Object fn_mod(Args args, Env& env);
    static Object fn_equal(Args args, Env& env);
    static Object fn_not_equal(Args args, Env& env);
    static Object fn_greater(Args args, Env& env);
    static Object fn_less(Args args, Env& env);
    static Object fn_greater_equal(Args args, Env& env);
    static Object fn_less_equal(Args args, Env& env);
    static Object fn_not(Args args, Env& env);
    static Object fn_or(Args args, Env& env);
    static Object fn_and(Args args, Env& env);
    static Object fn_typeof(Args args, Env& env);
    static Object fn_concat(Args args, Env& env);
    static Object fn_index(Args args, Env& env);
    static Object fn_get(Args args, Env& env);
    static Object fn_insert(Args args, Env& env);
    static Object fn_remove(Args args, Env& env);
    static Object fn_length(Args args, Env& env);
    static Object fn_push(Args args, Env& env);
    static Object fn_pop(Args args, Env& env);
    static Object fn_head(Args args, Env& env);
    static Object fn_tail(Args args, Env& env);
    static Object fn_parse(Args args, Env& env);
    static Object fn_replace(Args args, Env& env);
    static Object fn_repr(Args args, Env& env);
    static Object fn_map(Args args, Env& env);
    static Object fn_filter(Args args, Env& env);
    static Object fn_reduce(Args args, Env& env);
    static Object fn_range(Args args, Env& env);
    static Object fn_linspace(Args args, Env& env);
};

// .....................................
// -*- Implementation of Error class -*-
// .....................................
/*
private:
    Object* m_reason;
    Str m_msg;
};
*/
Error::Error(ErrorKind kind, const Str& msg) noexcept
: m_kind{kind}
, m_reason{nullptr}
, m_msg{msg}
{}

Error::Error(Object self, const Str& msg) noexcept
: m_kind{ErrorKind::Error}
, m_reason{std::make_unique<Object>(self)}
, m_msg{msg}
{}

Error::Error(ErrorKind kind, Object self, const Str& msg) noexcept
: m_kind{kind}
, m_reason{std::make_unique<Object>(self)}
, m_msg{msg}{}

Error::Error(Error&& other) noexcept
: m_kind{std::move(other.m_kind)}
, m_reason{std::move(other.m_reason)}
, m_msg{std::move(other.m_msg)}
{}

Error& Error::operator=(Error&& other) noexcept{
    if(this != &other){
        this->m_kind = std::move(other.m_kind);
        this->m_reason = std::move(other.m_reason);
        this->m_msg = std::move(other.m_msg);
    }
    return *this;
}

Str Error::describe(void) const{
    auto kind = errors[this->m_kind];
    std::stringstream ss;
    ss << "\x1b[91m" << kind << "\x1b[0m:\n";
    if(this->m_reason != nullptr){
        ss << this->m_reason->repr() << "\n";
    }
    ss << this->m_msg << std::endl;
    return ss.str();
}

// .............................
// -*- Implementation of Env -*-
// .............................
/*
private:
    Dict<Str, Object> m_bindings;
    Env* m_parent;
};
*/
Env::Env(Env* parent) noexcept
: m_bindings{}
, m_parent{nullptr}
{}

Env::Env(const Env& other) noexcept
: m_bindings{other.m_bindings}
, m_parent{other.m_parent}
{}

Env& Env::operator=(const Env& other) noexcept{
    if(this != &other){
        this->m_bindings = other.m_bindings;
        this->m_parent = other.m_parent;
    }
    return *this;
}

bool Env::contains(Str name) const{
    auto entry = this->m_bindings.find(name);
    if(entry != this->m_bindings.end()){
        return true;
    }
    if(this->m_parent == nullptr){
        return false;
    }
    return this->m_parent->contains(name);
}

Object Env::get(Str name) const{
    if(!this->contains(name)){
        std::stringstream ss;
        ss << "undefined variable or symbol `" << name << "'";
        throw Error(ErrorKind::RuntimeError, ss.str());
    }
    auto entry = this->m_bindings.find(name);
    if(entry != this->m_bindings.end()){ // in the current scope
        return entry->second;
    }
    // search in the parent scope
    return this->m_parent->get(name);
}

void Env::put(Str name, Object obj){
    // if(this->contains(name)){
    //     std::stringstream ss;
    //     ss << "Variable `" << name << "' already defined.\n";
    //     ss << "Redefinition not allowed";
    //     throw Error(ErrorKind::RuntimeError, ss.str());
    // }
    this->m_bindings[name] = obj;
}

void Env::update(Str name, Object obj){
    if(!this->contains(name)){
        std::stringstream ss;
        ss << "Variable `" << name << "' not defined.\n";
        ss << "Unable to update.";
        throw Error(ErrorKind::RuntimeError, ss.str());
    }
    this->m_bindings[name] = obj;
}

Env* Env::parent(void){
    return this->m_parent;
}

const Env* Env::parent(void) const{
    return this->m_parent;
}

std::ostream& operator<<(std::ostream& os, const Env& env){
    os << "{\n";
    for(const auto& [key, val]: env.m_bindings){
        os << "    " << key << ": " << val.repr() << "\n";
    }
    os << "}";
    return os;
}

// ......................................
// -*- Implementation of Object class -*-
// ......................................
/*
class Object::{
private:
    TypeKind m_kind;
    std::any m_val;
};
*/
Object::Object() noexcept
: m_kind{TypeKind::Nil}
, m_val{}
{}

Object::Object(bool flag) noexcept
: m_kind{TypeKind::Bool}
, m_val{flag}
{}

Object::Object(i64 num) noexcept
: m_kind{TypeKind::Integer}
, m_val{num}
{}

Object::Object(f64 num) noexcept
: m_kind{TypeKind::Float}
, m_val{num}
{}

Object::Object(ArrayList xs) noexcept
: m_kind{TypeKind::List}
, m_val{xs}
{}

Object::Object(const Symbol& symbol) noexcept
: m_kind{TypeKind::Symbol}
, m_val{symbol}
{}

Object::Object(const Str& str) noexcept
: m_kind{TypeKind::String}
, m_val{str}
{}

Object::Object(const Lambda& lambda) noexcept
: m_kind{TypeKind::Lambda}
, m_val{lambda}
{}

Object::Object(const Builtin& builtin) noexcept
: m_kind{TypeKind::CFun}
, m_val{builtin}
{}

Object::Object(const Object& other) noexcept
: m_kind{other.m_kind}
, m_val{other.m_val}
{}

Object::Object(Object&& other) noexcept
: m_kind{std::move(other.m_kind)}
, m_val{std::move(other.m_val)}
{}

Object& Object::operator=(const Object& other) noexcept{
    if(this != &other){
        this->m_kind = other.m_kind;
        this->m_val = other.m_val;
    }
    return *this;
}

Object& Object::operator=(Object&& other) noexcept{
    if(this != &other){
        this->m_kind = std::move(other.m_kind);
        this->m_val = std::move(other.m_val);
    }
    return *this;
}


// -*-
Vec<Symbol> Object::used_symbols(void) const{
    Vec<Symbol> result{};
    if(this->is_symbol()){
        result.push_back(this->as_symbol());
    }else if(this->is_lambda()){
        auto lambda = this->as_lambda();
        auto body = lambda.body;
        result = body.used_symbols();
    }else if(this->is_list()){
        Vec<Symbol> tmp{};
        auto xs = this->as_list();
        for(usize i=0; i < xs.size(); i++){
            tmp = xs[i].used_symbols();
            result.insert(result.end(), tmp.begin(), tmp.end());
        }
    }
    return result;
}

// -
bool Object::is_builtin(void) const{
    return this->m_kind == TypeKind::CFun;
}

bool Object::is_number(void) const{
    return this->is_integer() || this->is_float();
}

bool Object::is_integer(void) const{
    return this->m_kind == TypeKind::Integer;
}

bool Object::is_float(void) const{
    return this->m_kind == TypeKind::Float;
}

bool Object::is_string(void) const{
    return this->m_kind == TypeKind::String;
}
bool Object::is_symbol(void) const{
    return this->m_kind == TypeKind::Symbol;
}

bool Object::is_callable(void) const{
    return (
        this->is_builtin() ||
        this->is_lambda() ||
        this->is_function()
    );
}

bool Object::is_lambda(void) const{
    return this->m_kind == TypeKind::Lambda;
}

bool Object::is_list(void) const{
    return this->m_kind == TypeKind::List;
}

bool Object::is_bool(void) const{
    return this->m_kind == TypeKind::Bool;
}

bool Object::as_bool(void) const{
    bool result{};
    if(this->is_bool()){
        result = std::any_cast<bool>(this->m_val);
    }else if(this->is_integer()){
        result = (std::any_cast<i64>(this->m_val) == 0);
    }else if(this->is_float()){
        result = (std::any_cast<f64>(this->m_val) == 0.0);
    }else{
        result = true;
    }
    return result;
}

i64 Object::as_integer(void) const{
    i64 num{};
    if(this->is_bool()){
        num = this->as_bool() ? 1 : 0;
    }else if(this->is_integer()){
        num = std::any_cast<i64>(this->m_val);
    }else if(this->is_float()){
        num = static_cast<i64>(std::any_cast<f64>(this->m_val));
    }else{
        std::stringstream stream;
        auto ty = this->type().value;
        stream << "casting `" << ty << "' object to integer is not supported.";
        auto kind = ErrorKind::TypeError;
        throw Error(kind, *this, stream.str());
    }
    return num;
}

// -
f64 Object::as_float(void) const{
    f64 num{};
    if(this->is_bool()){
        num = this->as_bool() ? 1.0 : 0.0;
    }else if(this->is_integer()){
        num = static_cast<f64>(std::any_cast<i64>(this->m_val));
    }else if(this->is_float()){
        num = std::any_cast<f64>(this->m_val);
    }else{
        std::stringstream stream;
        auto ty = this->type().value;
        stream << "casting `" << ty << "' object to float is not supported.";
        auto kind = ErrorKind::TypeError;
        throw Error(kind, *this, stream.str());
    }
    return num;
}

Str Object::as_string(void) const{
    if(!this->is_string()){
        std::stringstream stream;
        auto ty = this->type().value;
        stream << "casting `" << ty << "' object to string is not supported.";
        auto kind = ErrorKind::TypeError;
        throw Error(kind, *this, stream.str());
    }
    return std::any_cast<Str>(this->m_val);
}

Symbol Object::as_symbol(void) const{
    if(!this->is_symbol()){
        std::stringstream stream;
        auto ty = this->type().value;
        stream << "casting `" << ty << "' object to symbol is not supported.";
        auto kind = ErrorKind::TypeError;
        throw Error(kind, *this, stream.str());
    }
    return std::any_cast<Symbol>(this->m_val);
}

ArrayList Object::as_list(void) const{
    if(!this->is_list()){
        std::stringstream stream;
        auto ty = this->type().value;
        stream << "casting `" << ty << "' object to list is not supported.";
        auto kind = ErrorKind::TypeError;
        throw Error(kind, *this, stream.str());
    }

    return std::any_cast<ArrayList>(this->m_val);
}

Lambda Object::as_lambda(void) const{
    if(!this->is_lambda()){
        std::stringstream stream;
        auto ty = this->type().value;
        stream << "casting `" << ty << "' object to lambda is not supported.";
        auto kind = ErrorKind::TypeError;
        throw Error(kind, *this, stream.str());
    }
    return std::any_cast<Lambda>(this->m_val);
}

Fun Object::as_function(void) const{
    if(!this->is_function()){
        std::stringstream stream;
        auto ty = this->type().value;
        stream << "casting `" << ty << "' object to function is not supported.";
        auto kind = ErrorKind::TypeError;
        throw Error(kind, *this, stream.str());
    }
    return std::any_cast<Fun>(this->m_val);
}

Builtin Object::as_builtin(void) const{
    if(!this->is_builtin()){
        std::stringstream stream;
        auto ty = this->type().value;
        stream << "casting `" << ty << "' object to builtin is not supported.";
        auto kind = ErrorKind::TypeError;
        throw Error(kind, *this, stream.str());
    }
    return std::any_cast<Builtin>(this->m_val);
}

Object Object::to_integer(void) const{
    return Object(this->as_integer());
}

Object Object::to_float(void) const{
    return Object(this->as_float());
}

Object Object::to_bool(void) const{
    return Object(this->as_bool());
}

Object Object::to_string(void) const{
    return Object(this->as_string());
}

Object Object::apply(Args args, Env& env){
    if(!this->is_callable()){
        auto kind = ErrorKind::TypeError;
        auto ty = this->type().value;
        std::stringstream ss;
        ss << "object of type `" << ty << "' is not a callable.";
        throw Error(kind, *this, ss.str());
    }
    Object result{};
    if(this->is_builtin()){
        auto fun = this->as_builtin().fun;
        result = fun(args, env);
    }else if(this->is_lambda()){
        auto lambda = this->as_lambda();
        // Check arity
        if(lambda.params.size() != args.size()){
            auto kind = ErrorKind::RuntimeError;
            std::stringstream ss;
            if(lambda.params.size() > args.size()){
                ss << "insufficient number of arguments. Expected ";
                ss << lambda.params.size() << " but got " << args.size() << ".";
            }else if(lambda.params.size() < args.size()){
                ss << "too many arguments. Expected ";
                ss << lambda.params.size() << " but got " << args.size() << ".";
            }
            throw Error(kind, *this, ss.str());
        }
        auto ctx = Env(&env);
        for(auto i=0; i < lambda.params.size(); i++){
            auto p = lambda.params[i];
            if(!p.is_symbol()){
                auto kind = ErrorKind::TypeError;
                std::stringstream ss;
                ss << "parameters must be symbol literals";
                throw Error(kind, *this, ss.str());
            }
            ctx.put(p.as_symbol().value, args[i]);
        }
        result = lambda.body.eval(ctx);
    }else{
        auto func = this->as_function();
        // Check arity
        if(func.params.size() != args.size()){
            auto kind = ErrorKind::RuntimeError;
            std::stringstream ss;
            if(func.params.size() > args.size()){
                ss << "insufficient number of arguments. Expected ";
                ss << func.params.size() << " but got " << args.size() << ".";
            }else if(func.params.size() < args.size()){
                ss << "too many arguments. Expected ";
                ss << func.params.size() << " but got " << args.size() << ".";
            }
            throw Error(kind, *this, ss.str());
        }
        auto ctx = Env(&env);
        for(auto i=0; i < func.params.size(); i++){
            auto p = func.params[i];
            if(!p.is_symbol()){
                auto kind = ErrorKind::TypeError;
                std::stringstream ss;
                ss << "parameters must be symbol literals";
                throw Error(kind, *this, ss.str());
            }
            ctx.put(p.as_symbol().value, args[i]);
        }
        result = func.body.eval(ctx);
    }
    return result;
}

void Object::push(Object obj){
    if(!this->is_list()){
        std::stringstream stream;
        auto ty = this->type().value;
        stream << "'push' is not applicable to `" << ty << "' objects but to list.";
        auto kind = ErrorKind::SyntaxError;
        throw Error(kind, *this, stream.str());
    }
    auto xs = std::any_cast<ArrayList>(this->m_val);
    xs.push_back(obj);
    this->m_val = xs;
}

Object Object::pop(void){
    if(!this->is_list()){
        std::stringstream stream;
        auto ty = this->type().value;
        stream << "'push' is not applicable to `" << ty << "' objects but to list.";
        auto kind = ErrorKind::SyntaxError;
        throw Error(kind, *this, stream.str());
    }
    auto xs = std::any_cast<ArrayList>(this->m_val);
    if(xs.size() == 0){
        std::stringstream stream;
        stream << "'pop' is not applicable to empty list.";
        auto kind = ErrorKind::ValueError;
        throw Error(kind, *this, stream.str());
    }
    auto result = xs[xs.size()-1];
    xs.pop_back();
    this->m_val = xs;
    return result;
}

// -*- COMPARAISON OPERATIONS -*-
bool operator==(const Object& lhs, const Object& rhs){
    bool result{};
    if(lhs.is_float() && rhs.is_integer()){
        result = (lhs.as_float() == rhs.as_float());
    }else if(lhs.is_integer() && rhs.is_float()){
        result = (lhs.as_float() == rhs.as_float());
    }else if(lhs.type() != rhs.type()){
        result = false;
    }else{
        if(lhs.is_integer()){
            result = (lhs.as_integer() == rhs.as_integer());
        }else if(lhs.is_float()){
            result = (lhs.as_float() == rhs.as_float());
        }else if(lhs.is_builtin()){
            result = (lhs.as_builtin().fun == rhs.as_builtin().fun);
        }else if(lhs.is_string()){
            result = (lhs.as_string() == rhs.as_string());
        }else if(lhs.is_symbol()){
            result = (lhs.as_symbol() == rhs.as_symbol());
        }else if(lhs.is_list()){
            result = (lhs.as_list() == rhs.as_list());
        }else if(lhs.is_lambda()){
            auto x = lhs.as_lambda();
            auto y = rhs.as_lambda();
            result = (
                (x.body == y.body) &&
                (x.params == y.params) &&
                (x.env == y.env)
            );
        }else if(lhs.is_function()){
            auto x = lhs.as_function();
            auto y = rhs.as_function();
            result = (
                (x.body == y.body) &&
                (x.params == y.params) &&
                (x.env == y.env) &&
                (x.name == y.name)
            );
        }else{ // boolean
            result = (lhs.as_integer() == rhs.as_integer());
        }
    }
    return result;
}

bool operator!=(const Object& lhs, const Object& rhs){
    return !(lhs == rhs);
}

bool operator>=(const Object& lhs, const Object& rhs){
    return !(lhs < rhs);
}

bool operator<=(const Object& lhs, const Object& rhs){
    return (lhs < rhs ) || (lhs == rhs);
}

bool operator>(const Object& lhs, const Object& rhs){
    return !(lhs <= rhs);
}
bool operator<(const Object& lhs, const Object& rhs){
    auto checked = (
        (lhs.is_number() && rhs.is_number()) ||
        (lhs.is_string() && rhs.is_string()) ||
        (lhs.is_symbol() && rhs.is_symbol())
    );
    if(!checked){
        std::stringstream stream;
        auto xty = lhs.type().value;
        auto yty = rhs.type().value;
        stream << "'<' is not support between types `" << xty;
        stream << "' and `" << yty << "'";
        auto kind = ErrorKind::TypeError;
        throw Error(kind, stream.str());
    }
    bool result{};
    if(lhs.is_number()){
        if(lhs.is_float() && (lhs.type() == rhs.type())){
            result = (lhs.as_float() < rhs.as_float());
        }else if(lhs.is_integer() && (lhs.type() == rhs.type())){
            result = (lhs.as_integer() < rhs.as_integer());
        }else{
            result = (lhs.as_float() < rhs.as_float());
        }
    }else if(lhs.is_string()){
        result = (lhs.as_string() < rhs.as_string());
    }else{
        result = (lhs.as_symbol().value < rhs.as_symbol().value);
    }
    return result;
}

// -*- ARITHMETIC OPERATIONS. -*-
// + : for NUMBER | STRING | LIST
Object operator+(const Object& lhs, const Object& rhs){
    if(rhs.is_nil() || lhs.is_nil()){ return Object(); }

    auto checked = (
        (lhs.is_number() && rhs.is_number()) ||
        (lhs.is_string() && rhs.is_string()) ||
        (lhs.is_list() && rhs.is_list())
    );
    if(!checked){
        auto kind = ErrorKind::ValueError;
        auto xty = lhs.type();
        auto yty = rhs.type();
        std::stringstream stream;
        if(lhs.is_number()){
            stream << "`+' is not applicable between `";
            stream << xty.value << "' and `" << yty.value << "' object";
        }else{
            stream << "cannot `concatenate' " << xty.value;
            stream << "' object and `" << yty.value << "' object";
        }
        throw Error(kind, stream.str());
    }
    Object result{};
    if(lhs.is_number()){
        if(lhs.is_float() && (lhs.type() == rhs.type())){
            auto x = lhs.as_float();
            auto y = rhs.as_float();
            result = Object((x+y));
        }else if(lhs.is_integer() && (lhs.type() == rhs.type())){
            auto x = lhs.as_integer();
            auto y = rhs.as_integer();
            result = Object((x+y));
        }else{
            auto x = lhs.as_float();
            auto y = rhs.as_float();
            result = Object((x+y));
        }
    }else if(lhs.is_string()){
        auto xstr = lhs.as_string();
        auto ystr = rhs.as_string();
        result = Object((xstr + ystr));
    }else{// list
        auto xvec = lhs.as_list();
        auto yvec = rhs.as_list();
        for(auto y: yvec){
            xvec.push_back(y);
        }
        result = Object(xvec);
    }
    return result;
}

Object operator-(const Object& lhs, const Object& rhs){
    if(rhs.is_nil() || lhs.is_nil()){ return Object(); }

    auto checked = (lhs.is_number() && rhs.is_number());
    if(!checked){
        auto kind = ErrorKind::ValueError;
        auto xty = lhs.type();
        auto yty = rhs.type();
        std::stringstream stream;
        stream << "`-' is not applicable between `";
        stream << xty.value << "' and `" << yty.value << "' object\n";
        stream << "but support for numbers.";
        throw Error(kind, stream.str());
    }
    Object result{};
    if(lhs.is_float() && (lhs.type() == rhs.type())){
        auto x = lhs.as_float();
        auto y = rhs.as_float();
        result = Object((x-y));
    }else if(lhs.is_integer() && (lhs.type() == rhs.type())){
        auto x = lhs.as_integer();
        auto y = rhs.as_integer();
        result = Object((x-y));
    }else{
        auto x = lhs.as_float();
        auto y = rhs.as_float();
        result = Object((x-y));
    }
    return result;
}

Object operator*(const Object& lhs, const Object& rhs){
    if(rhs.is_nil() || lhs.is_nil()){ return Object(); }

    auto checked = (lhs.is_number() && rhs.is_number());
    if(!checked){
        auto kind = ErrorKind::ValueError;
        auto xty = lhs.type();
        auto yty = rhs.type();
        std::stringstream stream;
        stream << "`*' is not applicable between `";
        stream << xty.value << "' and `" << yty.value << "' object\n";
        stream << "but support for numbers.";
        throw Error(kind, stream.str());
    }
    Object result{};
    if(lhs.is_float() && (lhs.type() == rhs.type())){
        auto x = lhs.as_float();
        auto y = rhs.as_float();
        result = Object((x*y));
    }else if(lhs.is_integer() && (lhs.type() == rhs.type())){
        auto x = lhs.as_integer();
        auto y = rhs.as_integer();
        result = Object((x*y));
    }else{
        auto x = lhs.as_float();
        auto y = rhs.as_float();
        result = Object((x*y));
    }
    return result;
}
Object operator/(const Object& lhs, const Object& rhs){
    if(rhs.is_nil() || lhs.is_nil()){ return Object(); }

    auto checked = (lhs.is_number() && rhs.is_number());
    if(!checked){
        auto kind = ErrorKind::ValueError;
        auto xty = lhs.type();
        auto yty = rhs.type();
        std::stringstream stream;
        stream << "`/' is not applicable between `";
        stream << xty.value << "' and `" << yty.value << "' object\n";
        stream << "but support for numbers.";
        throw Error(kind, stream.str());
    }
    Object result{};
    if(lhs.is_float() && (lhs.type() == rhs.type())){
        auto x = lhs.as_float();
        auto y = rhs.as_float();
        if(y == 0.0){
            auto kind = ErrorKind::ValueError;
            throw Error(kind, "division by zero");
        }
        result = Object((x/y));
    }else if(lhs.is_integer() && (lhs.type() == rhs.type())){
        auto x = lhs.as_integer();
        auto y = rhs.as_integer();
        if(y == 0){
            auto kind = ErrorKind::ValueError;
            throw Error(kind, "division by zero");
        }
        result = Object((x/y));
    }else{
        auto x = lhs.as_float();
        auto y = rhs.as_float();
        if(y == 0.0){
            auto kind = ErrorKind::ValueError;
            throw Error(kind, "division by zero");
        }
        result = Object((x/y));
    }
    return result;
}
Object operator%(const Object& lhs, const Object& rhs){
    if(rhs.is_nil() || lhs.is_nil()){ return Object(); }

    auto checked = (lhs.is_number() && rhs.is_number());
    if(!checked){
        auto kind = ErrorKind::ValueError;
        auto xty = lhs.type();
        auto yty = rhs.type();
        std::stringstream stream;
        stream << "`%' is not applicable between `";
        stream << xty.value << "' and `" << yty.value << "' object\n";
        stream << "but support for numbers.";
        throw Error(kind, stream.str());
    }
    Object result{};
    if(lhs.is_float() && (lhs.type() == rhs.type())){
        auto x = lhs.as_float();
        auto y = rhs.as_float();
        if(y == 0.0){
            auto kind = ErrorKind::ValueError;
            throw Error(kind, "division by zero");
        }
        result = Object(std::fmod(x, y));
    }else if(lhs.is_integer() && (lhs.type() == rhs.type())){
        auto x = lhs.as_integer();
        auto y = rhs.as_integer();
        if(y == 0){
            auto kind = ErrorKind::ValueError;
            throw Error(kind, "division by zero");
        }
        result = Object((x%y));
    }else{
        auto x = lhs.as_float();
        auto y = rhs.as_float();
        if(y == 0.0){
            auto kind = ErrorKind::ValueError;
            throw Error(kind, "division by zero");
        }
        result = Object(std::fmod(x, y));
    }
    return result;
}

Symbol Object::type(void) const{
    auto entry = types.find(this->m_kind);
    if(entry == types.end()){
        auto kind = ErrorKind::RuntimeError;
        throw Error(kind, "unknown type");
    }
    auto val = entry->second;
    return Symbol(val);
}

Str Object::str(void) const{
    std::stringstream stream;
    if(this->is_symbol()){ stream << this->as_symbol().value; }
    else if(this->is_integer()){
        stream << this->as_integer();
    }else if(this->is_float()){
        stream << this->as_float();
    }else if(this->is_bool()){
        stream << (this->as_bool() ? "true" : "false");
    }else if(this->is_string()){
        stream << this->as_string();
    }else if(this->is_symbol()){
        stream << this->as_symbol().value;
    }else if(this->is_lambda()){
        auto lambda = this->as_lambda();
        stream << "(lambda (";
        auto params = lambda.params;
        for(auto i=0; i < params.size(); i++){
            stream << params[i].repr();
            if(i < params.size() - 1){ stream << " "; }
        }
        stream << ")\n";
        stream << "   " << lambda.body.repr() << ")";
    }else if(this->is_function()){
        auto fun = this->as_function();
        stream << "(fun " << fun.name << "(";
        auto params = fun.params;
        for(auto i=0; i < params.size(); i++){
            stream << params[i].repr();
            if(i < params.size() - 1){ stream << " "; }
        }
        stream << ")\n";
        if(this->has_doc()){
            stream << "    " << Kalix::documentation[fun.name] << "\n";
        }
        stream << "    " << fun.body.repr() << ")";
    }else if(this->is_builtin()){
        auto builtin = this->as_builtin();
        stream << "<builtin `" << builtin.name << "' @ " << builtin.fun << ">";
    }else if(this->is_list()){
        stream << "(";
        auto xs = this->as_list();
        for(auto i=0; i < xs.size(); i++){
            stream << xs[i].repr();
            if(i < xs.size() - 1){ stream << " "; }
        }
        stream << ")";
    }else if(this->is_nil()){
        stream << "nil";
    }else{
        throw Error(ErrorKind::RuntimeError,  "encountered unknown type");
    }
    return stream.str();
}

Str Object::repr(void) const{
    std::stringstream stream;
    if(this->is_symbol()){ stream << this->as_symbol().value; }
    else if(this->is_integer()){
        stream << this->as_integer();
    }else if(this->is_float()){
        stream << this->as_float();
    }else if(this->is_bool()){
        stream << (this->as_bool() ? "true" : "false");
    }else if(this->is_string()){
        stream << std::quoted(this->as_string());
    }else if(this->is_symbol()){
        stream << this->as_symbol().value;
    }else if(this->is_lambda()){
        auto lambda = this->as_lambda();
        stream << "(lambda (";
        auto params = lambda.params;
        for(auto i=0; i < params.size(); i++){
            stream << params[i].repr();
            if(i < params.size() - 1){ stream << " "; }
        }
        stream << ")\n";
        stream << "   " << lambda.body.repr() << ")";
    }else if(this->is_function()){
        auto fun = this->as_function();
        stream << "(fun " << fun.name << "(";
        auto params = fun.params;
        for(auto i=0; i < params.size(); i++){
            stream << params[i].repr();
            if(i < params.size() - 1){ stream << " "; }
        }
        stream << ")\n";
        if(this->has_doc()){
            stream << "    " << Kalix::documentation[fun.name] << "\n";
        }
        stream << "    " << fun.body.repr() << ")";
    }else if(this->is_builtin()){
        auto builtin = this->as_builtin();
        stream << "<builtin `" << builtin.name << "' @ " << builtin.fun << ">";
    }else if(this->is_list()){
        stream << "(";
        auto xs = this->as_list();
        for(auto i=0; i < xs.size(); i++){
            stream << xs[i].repr();
            if(i < xs.size() - 1){ stream << " "; }
        }
        stream << ")";
    }else if(this->is_nil()){
        stream << "nil";
    }else{
        throw Error(ErrorKind::RuntimeError,  "encountered unknown type");
    }
    return stream.str();
}

std::ostream& operator<<(std::ostream& os, const Object& obj){
    os << obj.str();
    return os;
}



// -*-
Object Object::eval(Env& env){
    Object result{};
    switch(this->m_kind){
    case TypeKind::Symbol:
        result = env.get(this->as_symbol().value);
        break;
    case TypeKind::List:{ // function-call or s-expression definition
            auto xs = this->as_list();
            if(xs.size() == 0){
                std::stringstream ss;
                auto kind = ErrorKind::SyntaxError;
                ss << "missing function-name of keyword in function call expression";
                throw Error(kind, *this, ss.str());
            }
            auto sym = xs[0];
            if(!sym.is_symbol()){
                auto kind = ErrorKind::TypeError;
                std::stringstream ss;
                ss << "object `" << sym.repr() << "' is not the symbol.";
                throw Error(kind, ss.str());
            }
            if(Kalix::is_keyword(sym.as_symbol().value)){
                auto key = sym.as_symbol().value;
                if(key == "fun"){
                    return Kalix::handle_fun(xs, env);
                }else if(key == "lambda"){
                    return Kalix::handle_lambda(xs, env);
                }else if(key == "define"){
                    return Kalix::handle_define(xs, env);
                }else if(key == "while"){
                    return Kalix::handle_while(xs, env);
                }else if(key == "for"){
                    return Kalix::handle_for(xs, env);
                }else if(key == "progn"){
                    return Kalix::handle_progn(xs, env);
                }else if(key == "if"){
                    return Kalix::handle_if(xs, env);
                }else if(key == "import"){
                    return Kalix::handle_import(xs, env);
                }else if(key == "var"){
                    return Kalix::handle_var(xs, env);
                }else if(key == "let"){
                    return Kalix::handle_let(xs, env);
                }else{
                    auto kind = ErrorKind::Error;
                    std::stringstream ss;
                    ss << "unknown keyword `" << key << "'";
                    throw Error(kind, ss.str());
                }
            }
            auto argv = ArrayList(xs.begin()+1, xs.end());
            auto func = xs[0].eval(env);
            if(!func.is_callable()){
                auto ty = func.type().value;
                auto kind = ErrorKind::SyntaxError;
                std::stringstream ss;
                ss << "symbol `" << xs[0].as_symbol().value << "' is not the name of a callable";
            }
            for(auto i=0; i < argv.size(); i++){
                argv[i] = argv[i].eval(env);
            }

            result = func.apply(argv, env);
        }
    default:
        // Function, Lambda, Builtin, Nil, Bool, Integer, Float, or, String
        result = *this;
        break;
    }
    return Object();
}


Dict<Str, fs::path> Kalix::modules = {};
Env Kalix::runtime = Env();
std::set<Str> Kalix::keywords = {
    "lambda", "if", "define", "fun", "while",
    "for", "progn", "import", "let", "var",
};
Dict<Str, Object> Kalix::prelude = {};

Str Kalix::version = "";
Str Kalix::license = "";
Str Kalix::banner = "";
Str Kalix::help = "";
Dict<Str, Str> Kalix::documentation = {};

// -*-
Object Kalix::run(Str src, Env& env){
    auto parsed = Kalix::parse(src);
    auto len = parsed.size();
    for(decltype(len) i=0; i < len-1; i++){
        parsed[i].eval(env);
    }
    return parsed[len-1].eval(env);
}

/*
klx::Str input(klx::u32 id, std::string prefix);

void output(klx::u32 id, Str prefix, Str data);
*/

void Kalix::repl(Env& env){
    Str diary{};
    Vec<Str> history{};
    Str src{};
    Object result{};
    static i64 id{};
    auto help = [](){
        std::cerr << ":q | :quit        Exit the interpreter\n";
        std::cerr << ":s | :show.       Display the content of the current environment\n";
        std::cerr << ":hist | :history  Display the history of commands entered in this interpreter session.\n";
        std::cerr << ":diary            Export all command entered to a file\n";
        std::cerr << ":? | :h | :help   Display this messaage\n";
    };
    while(true){
        ++id;
        src = utils::input(id, "kalix");
        history.push_back(src);
        if(src.empty()){ continue; }
        else if(src == ":q" || src == ":quit"){
            break;
        }else if(src == ":show" || src == ":s"){
            std::cerr << env << std::endl;
        }else if(src == ":history" || src == ":hist"){
            for(usize i=0; i < history.size(); i++){
                std::cerr << "[" << (i+1) << "] " << history[i] << std::endl;
            }
        }else if(src == ":?" || src == ":h" || src == ":help"){
            help();
        }else if(src == ":diary"){
            std::cout << "File to export to: ";
            auto filename = Str{};
            std::fstream fout(filename, std::ios_base::out);
            fout << diary;
            fout.close();
        }else{
            try{
                diary += src + "\n";
                result = Kalix::run(src, env);
                utils::output(id, "kalix", result.repr());
            }catch(const Error& err){
                std::cerr << err.describe() << std::endl;
            }catch(const std::runtime_error& err){
                std::cerr << err.what() << std::endl;
            }catch(...){
                std::cerr << Error(ErrorKind::Error, "unknown error occured").describe() << std::endl;
            }
        }
    }
}


Env Kalix::initialize_runtime(void){
    prelude = Kalix::initialize_prelude();
    Env env{};
    for(auto [key, val]: prelude){
        env.put(key, val);
    }
    return env;
}

// -*-
bool Kalix::is_keyword(const Str& name){
    if(Kalix::keywords.find(name) != Kalix::keywords.end()){
        return true;
    }
    return false;
}
    
// -*-
// (lambda params body)
Object Kalix::handle_lambda(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(lambda params body)\n\n";
    syntax << "where\n\n";
    syntax << "    params      a list of symbols\n";
    syntax << "    body        any valid s-expression\n\n";
    syntax << "Example:\n\n";
    syntax << "     (lambda (x y) (+ x y))\n";
    Kalix::documentation["lambda"] = syntax.str();
    if(args.size() != 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "malformed lambda-expression\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    auto params = args[0];
    if(params.type() != Symbol(types[TypeKind::List])){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "malformed lambda-expression. Invalid parameters list.\n\n";
        // ss << syntax.str();
        throw Error(kind, ss.str());
    }
    for(const auto p: params.as_list()){
        if(!p.is_symbol()){
            auto kind = ErrorKind::SyntaxError;
            throw Error(
                kind, 
                "malformed lambda-expression.\n"
                "Parameters in parameters list must be symbols."
            );
        }
    }
    auto body = args[1];
    auto symbols = body.used_symbols();
    Dict<Str, Object> captures{};
    for(const auto& sym: symbols){
        for(const auto& p: params.as_list()){
            if(p.as_symbol() != sym){
                auto key = p.as_symbol().value;
                if(!env.contains(key)){
                    throw Error(ErrorKind::Error, Str("unbound varibale `") + key + "' ");
                }
                captures[key] = env.get(key);
                break;
            }
        }
    }
    // -*-
    Env ctx(&env);
    for(const auto& [key, val]: captures){
        ctx.put(key, val);
    }

    // -*-
    return Object(Lambda(params.as_list(), body, ctx));
}

// -*-
// (if test ok alt)
Object Kalix::handle_if(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(if test conseq alt)\n\n";
    syntax << "where\n\n";
    syntax << "    test        a boolean expression\n";
    syntax << "    conseq      expression evaluated and returned if test evaluates to true";
    syntax << "    alt.        expression evaluated and returned if test evaluates to false\n\n";
    syntax << "Example:\n\n";
    syntax << "     (if (< 1 2) (println \"1 is less than 2\") (println \"1 is greater than 2\"))\n";
    Kalix::documentation["if"] = syntax.str();
    if(args.size() != 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "malformed lambda-expression\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    if(args.size() != 3){
        std::stringstream ss;
        ss << "malformed if-expression. Expected 3 arguments, but got " << args.size();
        throw Error(ErrorKind::SyntaxError, ss.str());
    }
    return (args[0].eval(env).as_bool() ? args[1].eval(env) : args[2].eval(env));
}

// -*-
// (define name val [docstr])
Object Kalix::handle_define(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(define name value) \nor\n";
    syntax << "(define name value  docstr)\n\n";
    syntax << "where\n";
    syntax << "    name     symbol represention a variable\n";
    syntax << "    value    value bound to `name'\n";
    syntax << "    docstr   optional doctstring\n";
    syntax << "Examples:\n";
    syntax << "    (define name \"Hello world!\")\n";
    syntax << "    (define name \"John McCarthy\", \"Inventor of lisp programming language\")\n";

    Kalix::documentation["define"] = syntax.str();

    if(args.size() < 2 || args.size() > 3){
        std::stringstream ss;
        ss << "malformed define-expression. Expected 2 or 3 arguments, but got " << args.size() << "\n";
        throw Error(ErrorKind::SyntaxError, ss.str());
    }

    auto key = args[0];
    if(!key.is_symbol()){
        throw Error(ErrorKind::SyntaxError, "invalid variable in `define` expression");
    }
    auto name = key.as_symbol().value;
    auto value = args[1].eval(env);
    Object docstr{};
    bool hasDoc{false};
    if(args.size() == 3){
        docstr = args[2];
        if(!docstr.is_string()){
            throw Error(
                ErrorKind::SyntaxError,
                "malformed documentation string in `define' expression."
            );
        }
        hasDoc = true;
    }

    env.put(name, value);
    if(hasDoc){
        Kalix::documentation[name] = docstr.as_string();
    }
    return value;
}

// (fun name params [docstr] body)
Object Kalix::handle_fun(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(fun name params body)\nor\n";
    syntax << "(fun name params doctrs body)\n\n";
    syntax << "Examples:\n";
    syntax << "    (fun greet (name)\n";
    syntax << "        \"Print a greeting message for NAME\"\n";
    syntax << "        (println \"Hello {name}\"))\n";

    Kalix::documentation["fun"] = syntax.str();
    if(args.size() < 3 || args.size() > 4){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "malformed function definition expression\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    auto sname = args[0];
    if(!sname.is_symbol()){
        auto kind = ErrorKind::SyntaxError;
        throw Error(
            kind,
            "malformed function definition expression. Invalid function name."
        );
    }
    auto name = sname.as_symbol();
    auto params = args[1];
    if(params.type() != Symbol(types[TypeKind::List])){
        auto kind = ErrorKind::SyntaxError;
        throw Error(kind, "malformed function definition expression. Invalid parameters list.");
    }
    auto xs = params.as_list();
    for(const auto p: xs){
        if(!p.is_symbol()){
            auto kind = ErrorKind::SyntaxError;
            throw Error(
                kind, 
                "malformed function definition expression.\n"
                "Parameters in parameters list must be symbols."
            );
        }
    }
    Object doc{}, body{};
    bool hasDoc{false};
    if(args.size() == 4){
        doc = args[2];
        if(!doc.is_string()){
            throw Error(
                ErrorKind::SyntaxError,
                Str("Invalid documenation string for function `") + name.value + "'"
            );
        }
        hasDoc = true;
        body = args[3];
    }else{
        body = args[2];
    }
    auto symbols = body.used_symbols();
    Dict<Str, Object> captures{};
    for(const auto& sym: symbols){
        for(const auto& p: params.as_list()){
            if(p.as_symbol() != sym){
                auto key = p.as_symbol().value;
                if(!env.contains(key)){
                    throw Error(ErrorKind::Error, Str("unbound varibale `") + key + "' ");
                }
                captures[key] = env.get(key);
                break;
            }
        }
    }
    // -*-
    Env ctx(&env);
    for(const auto& [key, val]: captures){
        ctx.put(key, val);
    }

    auto fun = Object(Fun(name.value, xs, body, ctx));
    fun.has_doc() = hasDoc;
    Kalix::documentation[name.value] = doc.as_string();
    // -*-
    env.put(name.value, fun);
    return fun;
}

Object Kalix::handle_while(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(while test body)\n";
    syntax << "where\n";
    syntax << "    test      a booean s-expression\n";
    syntax << "    body      one or more s-expressions\n\n";
    syntax << "Examples:\n";
    syntax << "    (var x 1)\n";
    syntax << "    (while (< x 10)";
    syntax << "         (println \"x = {x}\")";
    syntax << "         (+ x 1))\n";
    Kalix::documentation["while"] = syntax.str();
    if(args.size() < 1){
        std::stringstream ss;
        auto kind = ErrorKind::SyntaxError;
        ss << "malformed while-expression.\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    Object result{};
    auto test = args[0];
    auto body = Object(ArrayList(args.begin()+1, args.end()));
    auto symbols = body.used_symbols();
    Env ctx{&env};
    for(const auto& sym: symbols){
        auto key = sym.value;
        if(!env.contains(key)){
            std::stringstream ss;
            ss << "unbound variable `" << key << "'";
            throw Error(ErrorKind::RuntimeError, ss.str());
        }
        ctx.put(key, env.get(key));
    }
    
    while(test.eval(ctx).as_bool()){
        for(usize i=1; i < args.size()-1; i++){
            args[i].eval(ctx);
        }
        result = args[args.size()-1].eval(ctx);
    }
    return result;
}

// (for (x xs) body)
Object Kalix::handle_for(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(for (x xs) body)\n";
    syntax << "where\n";
    syntax << "    x         iteration variable\n";
    syntax << "    xs        iterabble (i.e a list for now)\n";
    syntax << "    body      one or more s-expressions\n\n";
    syntax << "Examples:\n";
    syntax << "    (for (x range(10))";
    syntax << "         (println \"x = {x}\"))";
    Kalix::documentation["for"] = syntax.str();
    if(args.size() <= 1){
        std::stringstream ss;
        auto kind = ErrorKind::SyntaxError;
        ss << "malformed for-expression.\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    auto _args1 = args[0];
    if(!_args1.is_list()){
        std::stringstream ss;
        auto kind = ErrorKind::SyntaxError;
        ss << "malformed for-expression.\n\n";
        throw Error(kind, ss.str());
    }
    auto args1 = _args1.as_list();
    if(args1.size() != 2){
        std::stringstream ss;
        auto kind = ErrorKind::SyntaxError;
        ss << "malformed for-expression. Expect the first argument to be a list\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    auto _x = args1[0];
    if(!_x.is_symbol()){
        std::stringstream ss;
        auto kind = ErrorKind::SyntaxError;
        ss << "malformed for-expression.\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    auto _xs = args1[1];
    if(!_xs.is_list()){
        std::stringstream ss;
        auto kind = ErrorKind::SyntaxError;
        ss << "malformed for-expression.\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    auto x = _x.as_symbol();
    auto xs = _xs.as_list();
    auto exprs = ArrayList(args.begin()+1, args.end());
    auto body = Object(exprs);

    Env ctx{&env};
    ctx.put(x.value, Object());
    Dict<Str, Object> dict{};
    auto symbols = body.used_symbols();
    for(const auto& sym: symbols){
        auto key = sym.value;
        if(!env.contains(key)){
            std::stringstream ss;
            ss << "unbound variable `" << key << "'";
            throw Error(ErrorKind::RuntimeError, ss.str());
        }
        dict[key] = env.get(key).eval(env);
    }
    for(const auto& [key, val]: dict){
        ctx.put(key, val);
    }
    auto name = x.value;
    Object result{};
    for(auto& val: xs){
        ctx.update(name, val.eval(env));
        result = body.eval(ctx);
    }

    return result;
}

Object Kalix::handle_progn(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(progn ...)\n\n";
    syntax << "Evaluates a list of expressions and return the value of the last evaluated.";
    // syntax << "\n";
    Kalix::documentation["progn"] = syntax.str();
    Object result{};
    for(auto& arg: args){
        result = arg.eval(env);
    }
    return result;
} // do

Object Kalix::handle_quote(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(quote arg)\nor\n";
    syntax << "'arg\n\n";
    syntax << "Example:\n";
    syntax << "    (quote x)\n";
    if(args.size() != 1){
        std::stringstream ss;
        ss << "invalid number of arguments. Expect 1 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(ErrorKind::SyntaxError, ss.str());
    }
    return args[0];
}

// (let varlist body...)
Object Kalix::handle_let(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(let varlist body...)\n\n";
    syntax << "Example\n";
    syntax << "    (let ((x 1)\n";
    syntax << "        (y 2))\n";
    syntax << "        (println \"x = {x}, y = {y}\")\n";
    syntax << "        (println \"(+ x y) ===> {(+ x y)}\"))\n";
    Kalix::documentation["let"] = syntax.str();
    if(args.size() <= 2){
        std::stringstream ss;
        auto kind = ErrorKind::SyntaxError;
        ss << "invalid number of arguments. Expected at least 2 but got " << args.size() << "\n\n";
        ss << syntax.str() << "\n";
        throw Error(kind, ss.str());
    }
    auto vars = args[0];
    if(!vars.is_list()){
        std::stringstream ss;
        auto kind = ErrorKind::SyntaxError;
        ss << "malformed `let` expression\n";
        throw Error(kind, ss.str());
    }
    Dict<Str, Object> dict{};
    for(const auto vardef: vars.as_list()){
        auto pair = vardef.as_list();
        if(pair.size() != 2){
            throw Error(ErrorKind::SyntaxError, "malformed pair list in let expression.");
        }
        auto key = pair[0].as_symbol().value;
        auto val = pair[0];
        dict[key] = val;
    }

    Env ctx(&env);
    for(auto& [k, v]: dict){
        ctx.put(k, v);
    }

    auto vec = ArrayList(args.begin()+1, args.end());
    Object result{};
    for(auto v: vec){
        result = v.eval(ctx);
    }

    return result;
} // <added>

Object Kalix::handle_var(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(var name1 val1 name2 val2 ...)\n";
    syntax << "where\n";
    syntax << "     values val1, val2, etc. are bound to variables name1, name2 respectively.\n\n";
    syntax << "Example\n";
    syntax << "    (var name \"John Doe\")\n";
    syntax << "    (var country \"Germany\" capital \"Berlin\")";
    Kalix::documentation["var"] = syntax.str();
    if(args.size() % 2 != 0){
        std::stringstream ss;
        auto kind = ErrorKind::SyntaxError;
        ss << "invalid number of argument in `var' expression.\n";
        ss << "Expect an even number of arguments.\n\n";
        ss << syntax.str();
    }
    Str key{};
    Object val{};
    for(usize i=0; i < args.size()-1; i += 2){
        key = args[i].as_symbol().value;
        val = args[i+1].eval(env);
        env.put(key, val); 
    }
    return val;
} // <added> == setq

// (import module)
// (import "filename")
Object Kalix::handle_import(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(import module-name)\n";
    syntax << "(import \"path-to-module\")\n\n";
    syntax << "where\n";
    syntax << "    module-name        the name of a module already interned into the interpreter\n";
    syntax << "   \"path-to-module\"  the path to the module to be imported.\n\n";
    syntax << "Example:\n";
    syntax << "    (import math)         ;; load the builtin math module\n";
    syntax << "    (import \"./mymodule.klx\")\n";
    syntax << "    (add-path mymodule  \"../../path-to-mymodule.klx\") ;; intern `mymodule'\n";
    syntax << "    (import mymodule)     ;; now load mymodule\n";

    Kalix::documentation["import"] = syntax.str();
    if(args.size() != 1){
        std::stringstream ss;
        auto kind = ErrorKind::SyntaxError;
        ss << "invalid `import` expression\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    auto self = args[0];
    if(!(self.is_string() || self.is_symbol())){
        std::stringstream ss;
        auto kind = ErrorKind::SyntaxError;
        ss << "invalid `import` expression\n\n";
        throw Error(kind, ss.str());
    }
    if(self.is_string()){
        auto myfile = self.as_string();
        auto mypath = fs::absolute(fs::path(myfile));
        if(!fs::exists(mypath)){
            std::stringstream ss;
            auto kind = ErrorKind::RuntimeError;
            ss << "file `" << myfile << "' not found";
            throw Error(kind, ss.str());
        }
        auto filename = mypath.string();
        Env ctx(env);
        Kalix::run(utils::readfile(filename), ctx);
        env = ctx;
    }else{
        auto mymod = self.as_symbol().value;
        auto filename = Kalix::modules[mymod].string();
        Env ctx(env);
        Kalix::run(utils::readfile(filename), ctx);
        env = ctx;
    }
    return Object();
}

// -*-
Dict<Str, Object> Kalix::initialize_prelude(void){
    const auto MY_INTMAX = std::numeric_limits<i64>::max();
    const auto MY_INTMIN = std::numeric_limits<i64>::min();
    const auto MY_FLOAT_MIN = std::numeric_limits<f64>::max();
    const auto MY_FLOAT_MAX = std::numeric_limits<f64>::min();
    auto pi = 3.14;
    Dict<Str, Object> result = {
        {"nil", Object()},
        {"false", Object(false)},
        {"true", Object(true)},
        {"Float::PI", Object(3.141592653589793)},
        {"Float::E", Object(2.718281828459045)},
        {"Integer::MIN", Object(MY_INTMAX)},
        {"Integer::MAX", Object(MY_INTMAX)},
        {"Float::MIN", Object(MY_FLOAT_MAX)},
        {"Float::MAX", Object(MY_FLOAT_MIN)},
        {"add-path", Object(Builtin("add-path", fn_addpath))},              // [001]
        {"help", Object(Builtin("help", fn_help))},                         // [002]
        {"exit", Object(Builtin("exit", fn_exit))},                         // [003]
        {"quit", Object(Builtin("quit", fn_quit))},                         // [004]
        {"print", Object(Builtin("print", fn_print))},                      // [005]
        {"println", Object(Builtin("println", fn_println))},                // [006]
        {"eprint", Object(Builtin("eprint", fn_eprint))},                   // [007]
        {"eprintln", Object(Builtin("eprintln", fn_eprintln))},             // [008]
        {"panic", Object(Builtin("panic", fn_panic))},                      // [009]
        {"random", Object(Builtin("random", fn_random))},                   // [010]
        {"input", Object(Builtin("input", fn_input))},                      // [011]
        {"read", Object(Builtin("read", fn_read))},                         // [012]
        {"write", Object(Builtin("write", fn_write))},                      // [013]
        {"eval", Object(Builtin("eval", fn_eval))},                         // [014]
        {"list", Object(Builtin("list", fn_list))},                         // [015]
        {"integer", Object(Builtin("integer", fn_integer))},                // [016]
        {"float", Object(Builtin("float", fn_float))},                      // [017]
        {"string", Object(Builtin("string", fn_string))},                   // [018]
        {"bool", Object(Builtin("bool", fn_bool))},                         // [019]
        {"list?", Object(Builtin("list?", fn_listp))},                      // [020]
        {"integer?", Object(Builtin("integer?", fn_integerp))},             // [021]
        {"float?", Object(Builtin("float?", fn_floatp))},                   // [022]
        {"string?", Object(Builtin("string?", fn_stringp))},                // [023]
        {"bool?", Object(Builtin("bool?", fn_boolp))},                      // [024]
        {"+", Object(Builtin("+", fn_add))},                                // [025]
        {"-", Object(Builtin("-", fn_sub))},                                // [026]
        {"*", Object(Builtin("*", fn_mul))},                                // [027]
        {"/", Object(Builtin("/", fn_div))},                                // [028]
        {"%", Object(Builtin("%", fn_mod))},                                // [029]
        {"=", Object(Builtin("=", fn_equal))},                              // [030]
        {"!=", Object(Builtin("!=", fn_not_equal))},                        // [031]
        {">", Object(Builtin(">", fn_greater))},                            // [032]
        {"<", Object(Builtin("<", fn_less))},                               // [033]
        {"<=", Object(Builtin("<=", fn_less_equal))},                       // [034]
        {">=", Object(Builtin(">=", fn_greater_equal))},                    // [035]
        {"not", Object(Builtin("not", fn_not))},                            // [036]
        {"or", Object(Builtin("or", fn_or))},                               // [037]
        {"and", Object(Builtin("and", fn_and))},                            // [038]
        {"typeof", Object(Builtin("typeof", fn_typeof))},                   // [039]
        {"concat", Object(Builtin("concat", fn_concat))},                   // [040]
        {"get", Object(Builtin("and", fn_get))},                            // [041]
        {"index", Object(Builtin("index", fn_index))},                      // [042]
        {"insert", Object(Builtin("insert", fn_insert))},                   // [043]
        {"remove", Object(Builtin("remove", fn_remove))},                   // [044]
        {"length", Object(Builtin("length", fn_length))},                   // [045]
        {"push", Object(Builtin("push", fn_push))},                         // [046]
        {"pop", Object(Builtin("pop", fn_pop))},                            // [047]
        {"head", Object(Builtin("head", fn_head))},                         // [048]
        {"tail", Object(Builtin("tail", fn_tail))},                         // [049]
        {"parse", Object(Builtin("parse", fn_parse))},                      // [050]
        {"replace", Object(Builtin("replace", fn_replace))},                // [051]
        {"debug", Object(Builtin("debug", fn_repr))},                       // [052]
        {"map", Object(Builtin("map", fn_map))},                            // [053]
        {"filter", Object(Builtin("filter", fn_filter))},                   // [054]
        {"reduce", Object(Builtin("reduce", fn_reduce))},                   // [055]
        {"range", Object(Builtin("range", fn_range))},                      // [056]
        {"linspace", Object(Builtin("linspace", fn_linspace))},             // [057]
    };

    return result;
}

//! @todo: weak formulation. must check validity of ptr as index into `src'
void Kalix::skip_whitespace(const Str& src, int& ptr){
    while(std::isspace(src[ptr++])){}
}

Object Kalix::parse(const Str& src, int& ptr){
    Kalix::skip_whitespace(src, ptr);
    auto script = Str{src};
    // skip comment
    while(script[ptr] == ';'){
        auto _ptr = ptr;
        while(script[_ptr] != '\n' && _ptr < src.length()){
            _ptr++;
        }
        script.erase(ptr, _ptr - ptr);
        Kalix::skip_whitespace(script, ptr);
        if(script.substr(ptr, script.length()-ptr-1)==""){
            return Object();
        }
    }

    if(script==""){
        return Object();
    }else if(script[ptr] == '\''){// (quote object)
        ptr++;
        return Object(
            (ArrayList){Object(Symbol("quote")), Kalix::parse(script, ptr)}
        );
    }else if(script[ptr] == '('){ // list
        Kalix::skip_whitespace(script, ++ptr);
        auto self = Object(ArrayList());
        while(script[ptr] != ')'){
            self.push(Kalix::parse(script, ptr));
        }
        Kalix::skip_whitespace(script, ++ptr);
        return self;
    }else if(std::isdigit(script[ptr]) || (script[ptr]=='-' && std::isdigit(script[ptr+1]))){
        auto _ptr = ptr;
        auto negate = (script[ptr] == '-');
        if(negate){ ptr++; }
        Str numstr{};
        Str mantissa{}, fractional{}, exponent{};
        while(std::isdigit(script[ptr])){ ptr++;}
        mantissa = script.substr(_ptr, ptr-_ptr);
        bool hasDot = (script[ptr] == '.');
        if(hasDot){ ptr++; }
        _ptr = ptr;
        if(std::isdigit(script[ptr])){
            while(std::isdigit(script[ptr])){ ptr++; }
            fractional = script.substr(_ptr, ptr-_ptr);
        }
        bool hasExpo = (script[ptr] == 'e' || script[ptr] == 'E');
        if(hasExpo){ ptr++; }
        _ptr = ptr;
        auto _test = (
            (script[ptr]=='-' && std::isdigit(script[ptr+1])) ||
            (script[ptr]=='+' && std::isdigit(script[ptr+1]))
        );
        if(std::isdigit(script[ptr]) || _test){
            auto hasLeadingSign = (script[ptr] == '-' || (script[ptr]=='+'));
            if(hasLeadingSign){ ptr++; }
            while(std::isdigit(script[ptr])){ ptr++; }
            exponent = script.substr(_ptr, ptr-_ptr);
        }
        numstr = mantissa;
        if(hasDot){ numstr += "."; }
        if(fractional.length() != 0){ numstr += fractional; }
        if(hasExpo){ numstr += "e"; }
        if(hasExpo && exponent.length()==0){
            throw Error(ErrorKind::Error, "malformed floating-point number");
        }else{
            numstr += exponent;
        }
        Kalix::skip_whitespace(script, ptr);
        auto isFloating = (
            numstr.find(".") != Str::npos ||
            numstr.find("e") != Str::npos
        );
        if(isFloating){
            try{
                auto num = std::stod(numstr);
                return Object(num);
            }catch(const std::invalid_argument& err){
                throw Error(ErrorKind::ValueError, err.what());
            }catch(const std::out_of_range& err){
                throw Error(ErrorKind::ValueError, err.what());
            }catch(...){
                throw Error(ErrorKind::Error, "unexpected unknown error found.");
            }
        }else{
            try{
                auto num = static_cast<i64>(std::stoll(numstr));
                return Object(num);
            }catch(const std::invalid_argument& err){
                throw Error(ErrorKind::ValueError, err.what());
            }catch(const std::out_of_range& err){
                throw Error(ErrorKind::ValueError, err.what());
            }catch(...){
                throw Error(ErrorKind::Error, "unexpected unknown error found.");
            }
        }
    }else if(std::isdigit(script[ptr]) || (script[ptr]=='+' && std::isdigit(script[ptr+1]))){
        auto _ptr = ptr;
        auto hasLeadingPlus = (script[ptr] == '+');
        if(hasLeadingPlus){ ptr++; }
        Str numstr{};
        Str mantissa{}, fractional{}, exponent{};
        while(std::isdigit(script[ptr])){ ptr++;}
        mantissa = script.substr(_ptr, ptr);
        bool hasDot = (script[ptr] == '.');
        if(hasDot){ ptr++; }
        _ptr = ptr;
        if(std::isdigit(script[ptr])){
            while(std::isdigit(script[ptr])){ ptr++; }
            fractional = script.substr(_ptr, ptr-_ptr);
        }
        bool hasExpo = (script[ptr] == 'e' || script[ptr] == 'E');
        if(hasExpo){ ptr++; }
        _ptr = ptr;
        auto _test = (
            (script[ptr]=='-' && std::isdigit(script[ptr+1])) ||
            (script[ptr]=='+' && std::isdigit(script[ptr+1]))
        );
        if(std::isdigit(script[ptr]) || _test){
            auto hasLeadingSign = (script[ptr] == '-' || (script[ptr]=='+'));
            if(hasLeadingSign){ ptr++; }
            while(std::isdigit(script[ptr])){ ptr++; }
            exponent = script.substr(_ptr, ptr-_ptr);
        }
        numstr = mantissa;
        if(hasDot){ numstr += "."; }
        if(fractional.length() != 0){ numstr += fractional; }
        if(hasExpo){ numstr += "e"; }
        if(hasExpo && exponent.length()==0){
            throw Error(ErrorKind::Error, "malformed floating-point number");
        }else{
            numstr += exponent;
        }
        Kalix::skip_whitespace(script, ptr);
        auto isFloating = (
            numstr.find(".") != Str::npos ||
            numstr.find("e") != Str::npos
        );
        if(isFloating){
            try{
                auto num = std::stod(numstr);
                return Object(num);
            }catch(const std::invalid_argument& err){
                throw Error(ErrorKind::ValueError, err.what());
            }catch(const std::out_of_range& err){
                throw Error(ErrorKind::ValueError, err.what());
            }catch(...){
                throw Error(ErrorKind::Error, "unexpected unknown error found.");
            }
        }else{
            try{
                auto num = static_cast<i64>(std::stoll(numstr));
                return Object(num);
            }catch(const std::invalid_argument& err){
                throw Error(ErrorKind::ValueError, err.what());
            }catch(const std::out_of_range& err){
                throw Error(ErrorKind::ValueError, err.what());
            }catch(...){
                throw Error(ErrorKind::Error, "unexpected unknown error found.");
            }
        }
    }else if(script[ptr] == '"'){ // string
        auto n = 1;
        Str str{};
        while(script[ptr+n] != '"'){
            if(ptr+n >= script.length()){
                throw Error(
                    ErrorKind::ValueError,
                    "malformed string literal. Missing a closing '\"'"
                );
            }
            str += script[ptr+n];
            if(script[ptr+n] == '\\'){
                // skip one of '\\'
                n++;
            }
            n++;
        }

        // auto str = script.substr(ptr+1, (n-1));
        ptr += n+1; // take into account the last '"'
        Kalix::skip_whitespace(script, ptr);
        return Object(str);
    }else if(utils::is_valid_symbol_char(script[ptr])){ // symbols
        int n = 0;
        while(utils::is_valid_symbol_char(script[ptr+n])){ n++; }
        auto symval = script.substr(ptr, n);
        ptr += n;
        Kalix::skip_whitespace(script, ptr);
        return Object(Symbol(symval));
    }else{
        throw Error(ErrorKind::Error, "malformed program.");
    }
    return Object();
}
ArrayList Kalix::parse(const Str& src){
    int i{0}, _i = -1;
    ArrayList result{};
    while(_i != i && i <= src.length()-1){
        _i = i;
        result.push_back(Kalix::parse(src, i));
    }
    if(i < src.length()){
        throw Error(ErrorKind::Error, "malformed program.");
    }
    return result;
}


void Kalix::eval_args(Args& args, Env& env){
    auto len = args.size();
    for(decltype(len) i=0; i < len; i++){
        args[i] = args[i].eval(env);
    }
}

Object Kalix::fn_addpath(Args args, Env& env){
    Kalix::eval_args(args, env);
    std::stringstream syntax;
    syntax << "(add-path mymodname \"path-to-my-module\")\n\n";
    syntax << "where\n";
    syntax << "    mymodname               the name given to the module\n";
    syntax << "    \"path-to-my-module\"   path to the module\n";
    Kalix::documentation["add-path"] = syntax.str();
    if(args.size() != 2){
        std::stringstream ss;
        auto kind = ErrorKind::SyntaxError;
        ss << "invalid number of argumennts. Expect 2 arguments but got " << args.size() << "\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    auto name = args[0].as_symbol();
    auto filepath = args[1].as_string();
    Kalix::modules[name.value] = fs::absolute(fs::path(filepath));
    return Object(name);
}

// -*-
// (help symbol)
Object Kalix::fn_help(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(help term)\n";
    syntax << "where\n";
    syntax << "    term    display the help for `term'\n\n";
    syntax << "Example\n";
    syntax << "    (help cos)\n";
    Kalix::documentation["help"] = syntax.str();
    if(args.size() != 1){
        std::stringstream ss;
        auto kind = ErrorKind::SyntaxError;
        ss << "`help`: invalid number of arguments. Expect 1 but got " << args.size() << "\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto term = args[0].as_symbol().value;
    auto entry = Kalix::documentation.find(term);
    if(entry != Kalix::documentation.end()){
        std::cerr << "`" << term << "' has no documentation." << std::endl;
    }else{ // print the documentation
        std::cerr << entry->second << std::endl;
    }
    return Object();
}

// -*-
Object Kalix::fn_exit(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(exit)\n";
    syntax << "(exit exit-code)\n";
    syntax << "where\n";
    syntax << "    exit the current session with the given exit-code\n";
    syntax << "    exit-code     any integer value\n";
    Kalix::documentation["exit"] = syntax.str();
    if(args.size() > 1){
        std::stringstream ss;
        auto kind = ErrorKind::SyntaxError;
        ss << "`exit': invalid number of arguments. Expect at most 1 but got " << args.size() << "\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto ec = args.size() < 1 ? 0 : args[0].as_integer();
    std::exit(ec);
    return Object(); // never reached.
}


Object Kalix::fn_quit(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(quit)\n";
    syntax << "where\n";
    syntax << "    exit the current running session.\n";
    Kalix::documentation["quit"] = syntax.str();
    if(args.size()){
        std::stringstream ss;
        auto kind = ErrorKind::SyntaxError;
        ss << "`exit': invalid number of arguments. Expect 0 but got " << args.size() << "\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    std::exit(EXIT_SUCCESS);
    return Object(); // never reached.
}

Object Kalix::fn_print(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(print [args])\n";
    Kalix::documentation["print"] = syntax.str();
    eval_args(args, env);
    if(args.size() == 0){
        std::cout << "";
        return Object();
    }
    for(const auto& arg: args){
        std::cout << arg.str() << " ";
    }
    return Object();
}

Object Kalix::fn_println(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(println [args])\n";
    Kalix::documentation["println"] = syntax.str();
    eval_args(args, env);
    if(args.size() == 0){
        std::cout << "" << std::endl;
        return Object();
    }
    for(const auto& arg: args){
        std::cout << arg.str() << std::endl;
    }
    return Object();
}

Object Kalix::fn_eprint(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(eprint [args])\n";
    Kalix::documentation["eprint"] = syntax.str();
    eval_args(args, env);
    if(args.size() == 0){
        std::cerr << "";
        return Object();
    }
    for(const auto& arg: args){
        std::cerr << arg.str() << " ";
    }
    return Object();
}

Object Kalix::fn_eprintln(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(eprintln [args])\n";
    Kalix::documentation["eprintln"] = syntax.str();
    eval_args(args, env);
    if(args.size() == 0){
        std::cerr << "" << std::endl;
        return Object();
    }
    for(const auto& arg: args){
        std::cerr << arg.str() << " " << std::endl;
    }
    return Object();
}

Object Kalix::fn_panic(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(panic [args])\n";
    Kalix::documentation["panic"] = syntax.str();
    eval_args(args, env);
    if(args.size() == 0){
        std::cerr << "" << std::endl;
        std::exit(EXIT_FAILURE);
        return Object();
    }
    for(const auto& arg: args){
        std::cerr << arg.str() << std::endl;
    }
    std::exit(EXIT_FAILURE);
    return Object();
}

Object Kalix::fn_input(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(input [prompt])\n";
    Kalix::documentation["input"] = syntax.str();
    if(args.size() > 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`input': invalid number of argument. Expect at most 1 but got " << args.size() << "\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    if(args.size() == 1){
        std::cout << args[0].as_string();
    }
    Str text{};
    std::getline(std::cin >> std::ws, text);
    return Object(text);
}

Object Kalix::fn_random(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(random )\n";
    syntax << "(random vmax)\n";
    syntax << "(random vmin vmax)\n";
    syntax << "(random vmin vmax count)\n";
    Kalix::documentation["random"] = syntax.str();
    if(args.size() > 3){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "invalid number of argument while applying `random'.\n";
        ss << "Expect at most 3 but got " << args.size() << "\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    Object result{};
    std::random_device dev;
    std::mt19937 rng(dev());
    if(args.size() == 0){ // get a random number in the interval [0.0 1)
        std::uniform_real_distribution dist(0.0, 1.0);
        auto val = dist(rng);
        result = Object(val);
    }else if(args.size() == 1){
        auto vmax = args[0].as_float();
        if(vmax < 0){
            std::uniform_real_distribution dist(vmax, 0.0);
            result = Object(dist(rng));
        }else{
            std::uniform_real_distribution dist(0.0, vmax);
            result = Object(dist(rng));
        }
    }else if(args.size() == 2){
        auto vmin = args[0].as_float();
        auto vmax = args[1].as_float();
        if(vmin > vmax){
            throw Error(ErrorKind::ValueError, "in`(random vmin vmax)`. Expect vmin < vmax");
        }
        std::uniform_real_distribution dist(vmin, vmax);
        result = Object(dist(rng));
    }else{
        auto vmin = args[0].as_float();
        auto vmax = args[1].as_float();
        if(vmin > vmax){
            throw Error(ErrorKind::ValueError, "in`(random vmin vmax n)`. Expect vmin < vmax");
        }
        if(!args[2].is_integer()){
            throw Error(ErrorKind::TypeError, "in`(random vmin vmax n)`. Expect 'n' as integer.");
        }
        auto N = args[2].as_integer();
        if(N <= 0){
            throw Error(ErrorKind::ValueError, "in`(random vmin vmax n)`. Expect n > 0");
        }
        result = Object(ArrayList{});
        std::uniform_real_distribution dist(vmin, vmax);
        for(decltype(N) i=0; i < N; i++){
            result.push(Object(dist(rng)));
        }
    }
    return result;
}

Object Kalix::fn_read(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(read filename)\n";
    Kalix::documentation["read"] = syntax.str();
    if(args.size() != 1){
        std::stringstream ss;
        ss << "invalid argument number while applying `read'.\n";
        ss << "expect 1 argument but got " << args.size() << "\n";
        ss << syntax.str();
        throw Error(ErrorKind::SyntaxError, ss.str());
    }
    auto fname = args[0].as_string();
    auto path = fs::absolute(fs::path(fname));
    if(!fs::exists(path)){
        std::stringstream ss;
        ss << "file `" << fname << "' does not exit";
        throw Error(ErrorKind::ValueError, ss.str());
    }
    auto filename = path.string();
    return Object(utils::readfile(filename));
}

// (write filname data [mode])
// mode =  :append
Object Kalix::fn_write(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(write filename data)\n";
    syntax << "(write filename data :append)\n\n";
    syntax << "where\n";
    syntax << "    filename     the name of the file to write to\n";
    syntax << "    data.        the data to be written to the file\n";
    syntax << "    :append      optional indicating to append the data\n";
    syntax << "                 at the end if the file already exits.";
    Kalix::documentation["write"] = syntax.str();

    if(args.size() > 3){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`write': invalid number of arguments. Expect at most 3 but got\n";
        ss << args.size() << "\n\n" << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto fname = args[0].as_string();
    auto path = fs::absolute(fs::path(fname));
    if(!fs::exists(path)){
        std::stringstream ss;
        ss << "file `" << fname << "' does not exits.";
        throw Error(ErrorKind::ValueError, ss.str());
    }
    auto data = args[1].str(); // serialize the data
    auto mode = std::ios_base::out;
    if(args.size() == 3){
        auto opt = args[2].as_symbol();
        if(opt != Symbol(":append")){
            std::stringstream ss;
            ss << "`write': unknown option " << opt.value << ". Valid option is :append";
            throw Error(ErrorKind::ValueError, ss.str());
        }
        mode |= (std::ios_base::app | std::ios_base::ate);
    }

    auto filename = path.string();
    std::fstream fout(filename, mode);
    fout << data;
    return (fout.good() ? Object(true) : Object(false));
}


// (eval expr)
Object Kalix::fn_eval(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(eval expr)\n";
    Kalix::documentation["eval"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`eval': invalid number of argument. Expect 1 but got ";
        ss << args.size() << "\n\n" << syntax.str();
        throw Error(kind, ss.str());
    }
    // eval_args(args, env);
    auto expr = args[0];
    return expr.eval(env);
}

// -*-
Object Kalix::fn_list(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(list [x1 x2 x3 ...])\n\n";
    syntax << "Example:\n";
    syntax << "    (list 1 2 3 4)";
    Kalix::documentation["list"] = syntax.str();
    eval_args(args, env);
    return Object(args);
}

Object Kalix::fn_bool(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(bool arg)\n";
    Kalix::documentation["bool"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`bool': invalid number of argument. Expect 1 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    return Object(args[0].as_bool());
}

Object Kalix::fn_integer(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(integer arg)\n";
    Kalix::documentation["integer"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`integer': invalid number of argument. Expect 1 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto expr = args[0];
    i64 num{};
    if(expr.is_string()){
        try{
            auto numstr = expr.as_string();
            num = static_cast<i64>(std::stoll(numstr));
        }catch(const std::invalid_argument& err){
            throw Error(ErrorKind::ValueError, Str{err.what()});
        }catch(const std::out_of_range& err){
            throw Error(ErrorKind::ValueError, Str{err.what()});
        }catch(...){
            throw Error(ErrorKind::Error, "invalid argument to `integer'");
        }
    }else{
        num = args[0].as_integer();
    }
    return Object(num);
}

// -
Object Kalix::fn_float(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(float arg)\n";
    Kalix::documentation["float"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`float': invalid number of argument. Expect 1 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto expr = args[0];
    f64 num{};
    if(expr.is_string()){
        try{
            auto numstr = expr.as_string();
            num = static_cast<f64>(std::stod(numstr));
        }catch(const std::invalid_argument& err){
            throw Error(ErrorKind::ValueError, Str{err.what()});
        }catch(const std::out_of_range& err){
            throw Error(ErrorKind::ValueError, Str{err.what()});
        }catch(...){
            throw Error(ErrorKind::Error, "invalid argument to `float'");
        }
    }else{
        num = args[0].as_float();
    }
    return Object(num);
}

Object Kalix::fn_string(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(string arg)\n";
    Kalix::documentation["string"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`string': invalid number of argument. Expect 1 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    return Object(args[0].str());
}

Object Kalix::fn_listp(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(list? arg)\n";
    Kalix::documentation["list?"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`list?': invalid number of argument. Expect 1 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    return Object(args[0].is_list());
}

Object Kalix::fn_boolp(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(bool? arg)\n";
    Kalix::documentation["bool?"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`bool?': invalid number of argument. Expect 1 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    return Object(args[0].is_bool());
}

Object Kalix::fn_integerp(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(integer? arg)\n";
    Kalix::documentation["integer?"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`integer?': invalid number of argument. Expect 1 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    return Object(args[0].is_integer());
}

Object Kalix::fn_floatp(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(float? arg)\n";
    Kalix::documentation["bool"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`float?': invalid number of argument. Expect 1 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    return Object(args[0].is_float());
}

Object Kalix::fn_stringp(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(string? arg)\n";
    Kalix::documentation["string?"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`string?': invalid number of argument. Expect 1 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    return Object(args[0].is_string());
}

Object Kalix::fn_add(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(+ x1 x2 x3 ...)\n\n";
    syntax << "Example:\n";
    syntax << "    (+ 1 2 3)\n";
    syntax << "    (+ 1 3.14 5)\n";
    syntax << "    (+ 1 2)\n";
    syntax << "    (+ 3.1)\n";
    Kalix::documentation["+"] = syntax.str();
    if(args.size() < 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`+': invalid number of arguments. Expect at least 1 but got 0\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    
    bool fixed{};
    auto expr = args[0];
    if(!expr.is_number()){
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "invalid argument type. Expect a number.";
        throw Error(kind, ss.str());
    }
    if(args.size() == 1){ return expr; }
    auto result = expr;
    for(auto i=1; i < args.size(); i++){
        result = result + args[i];
    }

    return result;
}

Object Kalix::fn_sub(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(- x1 x2 x3 ...)\n\n";
    syntax << "Example:\n";
    syntax << "    (- 1 2 3)\n";
    syntax << "    (- 1 3.14 5)\n";
    syntax << "    (- 1 2)\n";
    syntax << "    (- 3.1)\n";
    Kalix::documentation["-"] = syntax.str();
    if(args.size() < 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`-': invalid number of arguments. Expect at least 1 but got 0\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    bool fixed{};
    auto expr = args[0];
    if(!expr.is_number()){
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "invalid argument type. Expect a number.";
        throw Error(kind, ss.str());
    }
    if(expr.is_integer()){
        fixed = true;
    }
    f64 acc = expr.as_float();
    if(args.size()==1){ // Just negation of the arguments
        return (fixed ? Object(static_cast<i64>(-1 * acc)) : Object((-1*acc)));
    }
    auto result = expr;
    for(auto i=1; i < args.size(); i++){
        result = result - args[i];
    }

    return result;
}

Object Kalix::fn_mul(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(* x1 x2 x3 ...)\n\n";
    syntax << "Example:\n";
    syntax << "    (* 1 2 3)\n";
    syntax << "    (* 1 3.14 5)\n";
    syntax << "    (* 1 2)\n";
    Kalix::documentation["*"] = syntax.str();
    if(args.size() < 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`*': invalid number of arguments. Expect at least 2 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    
    bool fixed{};
    auto acc = args[0];
    if(!acc.is_number()){
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "invalid argument type. Expect a number.";
        throw Error(kind, ss.str());
    }
    for(auto i=1; i < args.size(); i++){
        acc = acc * args[i];
    }

    return Object(acc);
}

Object Kalix::fn_div(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(/ x1 x2 x3 ...)\n\n";
    syntax << "Example:\n";
    syntax << "    (/ 1 2 3)\n";
    syntax << "    (/ 1 3.14 5)\n";
    syntax << "    (/ 1 2)\n";
    syntax << "    (/ 3.1)\n";
    Kalix::documentation["/"] = syntax.str();
    if(args.size() < 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`/': invalid number of arguments. Expect at least 1 but got 0\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }

    eval_args(args, env);
    auto expr = args[0];
    if(!expr.is_number()){
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "invalid argument type. Expect a number.";
        throw Error(kind, ss.str());
    }
    
    if(args.size() == 1){ // Just compute the inverse
        return Object((Object(1.0) / expr));
    }
    auto result = expr;
    for(auto i=1; i < args.size(); i++){
        if(expr.is_float()){
            if(args[i].as_float() == 0.0){
                throw Error(ErrorKind::ValueError, "division by zero");
            }
        }else{
            if(args[i].as_integer() == 0){
                throw Error(ErrorKind::ValueError, "division by zero");
            }
        }
        result = result / args[i];
    }

    return result;
}

Object Kalix::fn_mod(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(% q p)\n\n";
    syntax << "Example:\n";
    syntax << "    (% 9 4)\n";
    syntax << "    (% 7.1 2)\n";
    Kalix::documentation["%"] = syntax.str();
    if(args.size() != 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`%': invalid number of arguments. Expect 2 but got " << args.size() << " \n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto x = args[0];
    auto y = args[1];
    if(!(x.is_number() && y.is_number())){
        auto _x = x.type().value;
        auto _y = y.type().value;
        std::stringstream ss;
        ss << "'%' is not support between " << _x << " and " << _y;
        throw Error(ErrorKind::TypeError, ss.str());
    }
    
    return Object((x % y));
}

Object Kalix::fn_equal(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(= expr1 expr2)\n\n";
    syntax << "Example:\n";
    syntax << "    (= 1 1)\n";
    syntax << "    (= 3  3.0)\n";
    Kalix::documentation["="] = syntax.str();
    if(args.size() != 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`=': invalid number of arguments. Expect 2 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto lhs = args[0];
    auto rhs = args[1];
    return Object((lhs == rhs));
}

Object Kalix::fn_not_equal(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(!= expr1 expr2)\n\n";
    syntax << "Example:\n";
    syntax << "    (!= 3.1 3.12)\n";
    Kalix::documentation["!="] = syntax.str();
    if(args.size() != 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`!=': invalid number of arguments. Expect 2 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto lhs = args[0];
    auto rhs = args[1];
    return Object(!(lhs == rhs));
}

Object Kalix::fn_greater(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(> expr1 expr2)\n\n";
    syntax << "Example:\n";
    syntax << "    (> 3.1 3.12)\n";
    Kalix::documentation[">"] = syntax.str();
    if(args.size() != 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`>': invalid number of arguments. Expect 2 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto lhs = args[0];
    auto rhs = args[1];
    return Object((lhs > rhs));
}

Object Kalix::fn_less(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(< expr1 expr2)\n\n";
    syntax << "Example:\n";
    syntax << "    (< 3.1 3.12)\n";
    Kalix::documentation["<"] = syntax.str();
    if(args.size() != 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`<': invalid number of arguments. Expect 2 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto lhs = args[0];
    auto rhs = args[1];
    return Object((lhs < rhs));
}

Object Kalix::fn_greater_equal(Args args, Env& env){
std::stringstream syntax;
    syntax << "(>= expr1 expr2)\n\n";
    syntax << "Example:\n";
    syntax << "    (>= 3.1 3.12)\n";
    Kalix::documentation[">="] = syntax.str();
    if(args.size() != 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`>=': invalid number of arguments. Expect 2 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto lhs = args[0];
    auto rhs = args[1];
    return Object((lhs >= rhs));
}
Object Kalix::fn_less_equal(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(<= expr1 expr2)\n\n";
    syntax << "Example:\n";
    syntax << "    (<= 3.1 3.12)\n";
    Kalix::documentation["<="] = syntax.str();
    if(args.size() != 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`<=': invalid number of arguments. Expect 2 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto lhs = args[0];
    auto rhs = args[1];
    return Object((lhs <= rhs));
}

Object Kalix::fn_not(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(not expr)\n\n";
    syntax << "Example:\n";
    syntax << "    (not false)\n";
    Kalix::documentation["not"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`not': invalid number of arguments. Expect 1 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    return Object(!args[0].as_bool());
}

Object Kalix::fn_or(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(or x1 x2 x3 ...)\n\n";
    syntax << "Example:\n";
    syntax << "    (or true true)\n";
    syntax << "    (or false false)\n";
    syntax << "    (or false false false false true)\n";
    Kalix::documentation["or"] = syntax.str();
    if(args.size() < 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`or': invalid number of arguments. Expect at least 2 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto flag = false;
    for(const auto& val: args){
        if(val.as_bool()){
            flag = true;
            break;
        }
    }
    return Object(flag);
}

Object Kalix::fn_and(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(and x1 x2 x3 ...)\n\n";
    syntax << "Example:\n";
    syntax << "    (and true true)\n";
    syntax << "    (and false false)\n";
    syntax << "    (and false false false false true)\n";
    Kalix::documentation["and"] = syntax.str();
    if(args.size() < 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`and': invalid number of arguments. Expect at least 2 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto flag = true;
    for(const auto& val: args){
        if(!val.as_bool()){
            flag = false;
            break;
        }
    }
    return Object(flag);
}

Object Kalix::fn_typeof(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(typeof expr)\n\n";
    syntax << "Example:\n";
    syntax << "    (typeof 3.1)\n";
    Kalix::documentation[""] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`typeof': invalid number of arguments. Expect 1 but got " << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    return Object(args[0].type());
}


Object Kalix::fn_concat(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(concat xstr ystr)\n";
    syntax << "(concat xlist ylist)\n";
    Kalix::documentation[""] = syntax.str();
    if(args.size() != 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`concat': invalid number of arguments. Expect 2 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    auto lhs = args[0];
    auto rhs = args[1];
    if(lhs.type() != rhs.type()){
        auto kind = ErrorKind::ValueError;
        std::stringstream ss;
        ss << "`concat': cannot concatenate values of type `";
        ss << lhs.type().value << "' and `" << rhs.type().value << "'";
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    Object result{};
    if(lhs.is_string()){
        auto xstr = lhs.as_string();
        auto ystr = rhs.as_string();
        result = Object((xstr + ystr));
    }else if(lhs.is_list()){
        result = Object(lhs.as_list());
        for(const auto& x: rhs.as_list()){
            result.push(x);
        }
    }else{
        auto kind = ErrorKind::ValueError;
        std::stringstream ss;
        ss << "`concat': cannot concatenate values of type `";
        ss << lhs.type().value << "' and `" << rhs.type().value << "'";
        throw Error(kind, ss.str());
    }
    return result;
}

// Look for 'val' or 'substr' in a list or string.
// Return nil if not found otherwise the index of the first occurence.
Object Kalix::fn_index(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(index xlist val)\n";
    syntax << "(index xstr substr)\n";
    Kalix::documentation["index"] = syntax.str();
    if(args.size() != 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`index': invalid number of arguments. Expect 2 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto lhs = args[0];
    auto rhs = args[1];
    if(!(lhs.is_list() || rhs.is_string())){
        std::stringstream ss;
        ss << "`index': first argument must be a string or a list.\n";
        throw Error(ErrorKind::TypeError, ss.str());
    }
    if(lhs.is_string()){
        auto substr = rhs.as_string();
        auto source = lhs.as_string();
        auto pos = source.find(substr);
        if(pos == Str::npos){
            return Object();  // nil
        }
        return Object(static_cast<i64>(pos));
    }

    // lhs is a list
    auto xs = lhs.as_list();
    Object result{};
    for(auto i=0; i < xs.size(); i++){
        if(xs[i] == rhs){
            result = Object(static_cast<i64>(i));
            break;
        }
    }

    return result;
}

// Object Kalix::fn_get(Args args, Env& env){}
Object Kalix::fn_insert(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(insert xstr  idx substr)\n";
    syntax << "(insert xlist idx val)\n";
    Kalix::documentation["insert"] = syntax.str();
    if(args.size() != 3){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`insert': invalid number of arguments. Expect 3 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto lhs = args[0];
    Object result{};
    if(!args[1].is_integer()){
        std::stringstream ss;
        ss << "`insert': second argument must be and index.";
        throw Error(ErrorKind::TypeError, ss.str());
    }
    auto idx = static_cast<int>(args[1].as_integer());

    if(lhs.is_string()){    
        auto substr = args[2].as_string();
        auto text = lhs.as_string();
        if(idx >= text.length()){
            throw Error(ErrorKind::IndexError, "`insert`: index value out of range.");
        }
        text.insert(text.begin() + idx, substr.begin(), substr.end());
        result = Object(text);
    }else if(lhs.is_list()){
        auto val = args[2];
        auto xs = lhs.as_list();
        if(idx >= xs.size()){
            throw Error(ErrorKind::IndexError, "`insert`: index value out of range.");
        }
        xs.insert(xs.begin()+idx, val);
        result = Object(xs);
    }
    return result;
}

Object Kalix::fn_remove(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(remove xstr idx)\n";
    syntax << "(remove xlist idx)\n";
    Kalix::documentation["remove"] = syntax.str();
    if(args.size() != 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`remove': invalid number of arguments. Expect 2 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto lhs = args[0];
    if(!(lhs.is_string() || lhs.is_list())){
        throw Error(ErrorKind::TypeError, "`remove': first argument mut be an string or a list");
    }
    if(args[1].is_integer()){
        throw Error(ErrorKind::TypeError, "`remove': second argument mut be an integer");
    }
    auto idx = (int)(args[1].as_integer());
    Object result{};
    if(lhs.is_string()){
        auto xstr = lhs.as_string();
        if(idx >= xstr.length()){
            throw Error(ErrorKind::IndexError, "`remove`: index value out of range.");
        }
        xstr.erase(idx, 1); // remove 1 character @ idx
        result = Object(xstr);
    }else if(lhs.is_list()){
        auto xs = lhs.as_list();
        if(idx >= xs.size()){
            throw Error(ErrorKind::IndexError, "`remove`: index value out of range.");
        }
        xs.erase(xs.begin() + idx);
        result = Object(xs);
    }else{
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "`remove': first argument must be a string or a list.";
        throw Error(kind, ss.str());
    }
    return result;
}

Object Kalix::fn_length(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(length xstr)\n";
    syntax << "(length xlist)\n";
    Kalix::documentation["length"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`length': invalid number of arguments. Expect 1 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto self = args[0];
    if(!(self.is_string() || self.is_list())){
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "`length': argument must be a list or a string";
        throw Error(kind, ss.str());
    }
    auto result = Object();
    if(self.is_string()){
        auto len = static_cast<i64>(self.as_string().length());
        result = Object(len);
    }else{
        auto len = static_cast<i64>(self.as_list().size());
        result = Object(len);
    }
    return result;
}

Object Kalix::fn_push(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(push xstr substr)\n";
    syntax << "(push xlist val)\n";
    Kalix::documentation["push"] = syntax.str();
    if(args.size() != 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`push': invalid number of arguments. Expect 2 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto self = args[0];
    auto rhs = args[0];
    if(!(self.is_string() || self.is_list())){
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "`pop': first argument must be a list or a string";
        throw Error(kind, ss.str());
    }
    auto result = Object();
    if(self.is_string()){
        if(!rhs.is_string()){
            std::stringstream ss;
            ss << "`push': when the first argument is a string, the second ";
            ss << "argument is expected to be a string";
            throw Error(ErrorKind::TypeError, ss.str());
        }
        auto xstr = self.as_string();
        auto substr = rhs.as_string();
        result = Object((xstr + substr));
    }else{
        self.push(rhs);
        result = Object(self);
    }
    return result;
}

Object Kalix::fn_pop(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(pop xstr)\n";
    syntax << "(pop xlist)\n";
    Kalix::documentation["pop"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`pop': invalid number of arguments. Expect 1 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    if(!(args[0].is_list() || args[0].is_string())){
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "`pop': argument must be a list or a string";
        throw Error(kind, ss.str());
    }
    auto result = Object();
    if(args[0].is_list()){
        [[maybe_unused]] auto _ = args[0].pop();
        auto xs = args[0].as_list();
        result = Object(xs);
    }else{
        auto xstr = args[0].as_string();
        xstr.pop_back();
        result = Object(xstr);
    }
    return result;
}

Object Kalix::fn_head(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(head xlist)\n";
    Kalix::documentation["head"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`head': invalid number of arguments. Expect 1 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto self = args[0].as_list();
    if(self.empty()){
        auto kind = ErrorKind::ValueError;
        std::stringstream ss;
        ss << "`head': cannot get head data from empty list.";
        throw Error(kind, ss.str());
    }
    return self[0];
}

Object Kalix::fn_tail(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(tail xlist)\n";
    Kalix::documentation["tail"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`tail': invalid number of arguments. Expect 1 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto self = args[0].as_list();
    if(self.empty()){ // self is an empty list
        return Object(); // nil
    }
    auto result = Object(ArrayList{});
    for(auto i=1; i < self.size(); i++){
        result.push(self[i]);
    }
    return result;
}

Object Kalix::fn_parse(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(parse xstr)\n";
    Kalix::documentation["parse"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`parse': invalid number of arguments. Expect 1 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto text = args[0].as_string();
    auto vec = parse(text);
    return Object(vec);
}

Object Kalix::fn_replace(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(replace xstr old neo)\n";
    syntax << "(replace xlist old neo)\n";
    Kalix::documentation["replace"] = syntax.str();
    if(args.size() != 3){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`replace': invalid number of arguments. Expect 3 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto self = args[0];
    if(!(self.is_list() || self.is_string())){
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "`replace': first argument must be a list or a string";
        throw Error(kind, ss.str());
    }
    auto result = Object();
    if(self.is_string()){
        auto old = args[1].as_string();
        auto neo = args[2].as_string();
        auto str = self.as_string();
        utils::replace(str, old, neo);
        result = Object(str);
    }else{
        auto vec = self.as_list();
        auto old = args[1];
        auto neo = args[2];
        for(int i=0; i < vec.size(); i++){
            if(vec[i] == old){
                vec[i] = neo;
                break;
            }
        }
        result = Object(vec);
    }
    return result;
}

Object Kalix::fn_repr(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(repr arg)\n";
    Kalix::documentation["repr"] = syntax.str();
    if(args.size() != 1){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`repr': invalid number of arguments. Expect 1 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    return Object(args[0].repr());
}

Object Kalix::fn_map(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(map fn xs)\n";
    Kalix::documentation["map"] = syntax.str();
    if(args.size() != 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`map': invalid number of arguments. Expect 2 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto fun = args[0];
    if(!fun.is_callable()){
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "`map': first argument must be a callable.";
        throw Error(kind, ss.str());
    }
    auto argv = args[1];
    if(!argv.is_list()){
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "`map': second argument must be a list.";
        throw Error(kind, ss.str());
    }
    auto result = Object(ArrayList{});
    ArrayList tmp{};
    for(const auto& val: argv.as_list()){
        tmp.push_back(val);
        result.push(fun.apply(tmp, env));
        tmp.clear();
    }
    return result;
}

Object Kalix::fn_filter(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(filter fn xs)\n";
    Kalix::documentation["filter"] = syntax.str();
    if(args.size() != 2){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`filter': invalid number of arguments. Expect 2 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto fun = args[0];
    if(!fun.is_callable()){
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "`filter': first argument must be a callable.";
        throw Error(kind, ss.str());
    }
    auto argv = args[1];
    if(!argv.is_list()){
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "`filter': second argument must be a list.";
        throw Error(kind, ss.str());
    }
    auto result = Object(ArrayList{});
    ArrayList tmp{};
    for(const auto& val: argv.as_list()){
        tmp.push_back(val);
        if(fun.apply(tmp, env).as_bool()){
            result.push(val);
        }
        tmp.clear();
    }
    return result;
}


Object Kalix::fn_reduce(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(reduce fn xs init)\n";
    Kalix::documentation["reduce"] = syntax.str();
    if(args.size() != 3){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`reduce': invalid number of arguments. Expect 3 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    auto fun = args[0];
    if(!fun.is_callable()){
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "`reduce': first argument must be a callable.";
        throw Error(kind, ss.str());
    }
    auto argv = args[1];
    if(!argv.is_list()){
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "`reduce': second argument must be a list.";
        throw Error(kind, ss.str());
    }
    auto acc = args[2];
    ArrayList tmp{};
    for(const auto& val: argv.as_list()){
        tmp.push_back(acc);
        tmp.push_back(val);
        acc = fun.apply(tmp, env);
        tmp.clear();
    }
    return acc;
}

Object Kalix::fn_range(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(range start stop)\n";
    syntax << "(range start stop step)\n";
    Kalix::documentation["range"] = syntax.str();
    if(args.size() > 3){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`range': invalid number of arguments. Expect at most 3 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    i64 start{}, stop{}, step{};
    for(const auto& val: args){
        if(!val.is_integer()){
            auto kind = ErrorKind::TypeError;
            std::stringstream ss;
            ss << "`range': arguments must be integers";
            throw Error(kind, ss.str());
        }
    }
    
    start = args[0].as_integer();
    stop = args[1].as_integer();
    step = (args.size() == 2 ? 1 : args[2].as_integer());
    if(start > stop){
        auto kind = ErrorKind::ValueError;
        std::stringstream ss;
        ss << "`range': invalid arguments ordering. Expect start < stop but\n";
        ss << "got start = " << start << " and stop = " << stop;
        throw Error(kind, ss.str());
    }
    auto result = Object(ArrayList{});
    for(decltype(start) val = start; val < stop; val += step){
        result.push(Object(val));
    }

    return result;
}

Object Kalix::fn_linspace(Args args, Env& env){
    std::stringstream syntax;
    syntax << "(linspace start stop)\n";
    syntax << "(linspace start stop count)\n";
    Kalix::documentation["linspace"] = syntax.str();
    if(args.size() > 3){
        auto kind = ErrorKind::SyntaxError;
        std::stringstream ss;
        ss << "`linspace': invalid number of arguments. Expect at most 3 but got ";
        ss << args.size() << "\n\n";
        ss << syntax.str();
        throw Error(kind, ss.str());
    }
    eval_args(args, env);
    i64 start{}, stop{}, step{}, count{};
    for(const auto& val: args){
        if(!val.is_number()){
            auto kind = ErrorKind::TypeError;
            std::stringstream ss;
            ss << "`linspace': arguments must be numbers";
            throw Error(kind, ss.str());
        }
    }
    start = args[0].as_float();
    stop = args[1].as_float();
    if(args.size() == 3 && !args[2].is_integer()){
        auto kind = ErrorKind::TypeError;
        std::stringstream ss;
        ss << "`linspace': third arguments must be an integer";
        throw Error(kind, ss.str());
    }
    count = (args.size() == 2 ? 1 : args[2].as_integer());
    if(start > stop){
        auto kind = ErrorKind::ValueError;
        std::stringstream ss;
        ss << "`linspaace': invalid arguments ordering. Expect start < stop but\n";
        ss << "got start = " << start << " and stop = " << stop;
        throw Error(kind, ss.str());
    }
    auto result = Object(ArrayList{});
    if(count < 2){
        throw Error(
            ErrorKind::ValueError,
            "`linspace': third argument must be an integer >= 2"
        );
    }
    auto dx = (stop - start) / (count - 1);
    for(decltype(start) val = start; val < stop; val += dx){
        result.push(Object(val));
    }

    return result;
}


// -*----------------------------------------------------------------*-
}//-*- end::namespace::klx                                          -*-
// -*----------------------------------------------------------------*-

// ------------------------------
// -*- M A I N   D R I V E R  -*-
// ------------------------------
int main(int argc, char** argv){
    std::string progname{argv[0]};
    auto help = [&](){
        std::stringstream ss;
        ss << "Usage:\n\n";
        ss << "    " << progname << " [-i]\n";
        ss << "    " << progname << " [-v | --version]\n";
        ss << "    " << progname << " [--license]";
        ss << "    " << progname << " [--no-banner]\n";
        ss << "    " << progname << " [-f filename]\n\n";
        ss << "    " << progname << " [-h | --help]\n";
        ss << "OPTIONS:\n";
        ss << "  -i                Switch to interactive mode\n";
        ss << "  -v | --version    Show the version\n";
        ss << "  --license         Show the license\n";
        ss << "  --no-banner       Do not display the banner\n";
        ss << "  -f filename       Run the script in file named <filename>\n";
        ss << "  -h | --help       Show this help" << std::endl;
        return ss.str();
    };

    using namespace klx;
    Kalix::version = "0.0.1";
    Kalix::license = "MIT license";
    Kalix::help = help();
    Kalix::banner = progname + "\n"
        "VERSION: " + Kalix::version + "\n"
        "LICENSE: " + Kalix::license + "\n";
    Kalix::initialize_runtime();
    ArrayList args{};
    for(int i=0; i < argc; i++){
        args.push_back(Object(Str{argv[i]}));
    }
    Kalix::runtime.put("ARGV", Object(args));
    if(argc == 1){
        std::cerr << Kalix::banner << std::endl;
        Kalix::repl(Kalix::runtime);
    }else if(argc == 2){
        auto arg = Str{argv[1]};
        if(arg == "-i"){
            std::cerr << Kalix::banner << std::endl;
            Kalix::repl(Kalix::runtime);
        }else if(arg == "-v" || arg == "--version"){
            std::cerr << Kalix::version << std::endl;
            std::exit(0);
        }else if(arg == "-h" || arg == "--help"){
            std::cerr << Kalix::help << std::endl;
            std::exit(0);
        }else if(arg == "--license"){
            std::cerr << Kalix::license << std::endl;
            std::exit(0);
        }else if(arg == "--no-banner"){
            Kalix::repl(Kalix::runtime);
        }
    }else if(argc == 3 && Str{argv[1]} == "-f"){
        Kalix::run(utils::readfile(argv[2]), Kalix::runtime);
    }else{
        std::cerr << "\x1b[31mError\x1b[0m: invalid arguments." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
}