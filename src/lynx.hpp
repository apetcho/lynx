#ifndef __LYNX_HPP__
#define __LYNX_HPP__

#include<unordered_map>
#include<exception>
#include<stdexcept>
#include<iostream>
#include<fstream>
#include<sstream>
#include<memory>
#include<string>
#include<vector>
#include<cstdint>
#include<cassert>
#include<variant>
#include<optional>
#include<map>


#define LYNX_UNUSED(arg)    (void)arg


#define LYNX_DECLARE_COPY(Name)     \
    Name(Name&& other);             \
    Name& operator=(Name&& other)


#define LYNX_DECLARE_MOVE(Name)         \
    Name(const Name& other);            \
    Name& operator=(const Name& other)

#define LYNX_OBJECT_COMMONS()           \
    Str str(void) const override;       \
    Str repr(void) const override;      \
    operator bool()

#define LYNX_SCALAR_COMMONS()           \
    operator i64();                     \
    operator f64();                     \
    operator Str()


/*
modulename::ident
for(ident: iterable){}

for(i: range(0, 10)){
    ...
}

let data = [....]
for(x: data){
    ...
}

let dict = {(key1, val1), (key2, val2), ...}
*/

// -*----------------------------------------------------------------*-
// -*- begin::namespace::lynx                                       -*-
// -*----------------------------------------------------------------*-
namespace lynx{
// -
template<typename T>
using Shared = std::shared_ptr<T>;
template<typename T>
using Vec = std::vector<T>;
using u32 = std::uint32_t;
using i32 = std::int32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;
using u8 = std::uint8_t;
using i8 = std::int8_t;
using f64 = double;
using Str = std::string;
template<typename K, typename V>
using HashMap = std::unordered_map<K, V>;

// -----------------------
// - forward declaration -
// -----------------------
class Env;
// class Module;
class Object;
class Error;
class Type;

using Self = Shared<Object>;
using Scope = Shared<Env>;
typedef Self (*BuiltinFn)(Vec<Self>);
typedef Self (*Callback)(Self&, Vec<Self>);

// --------------
// --- Object ---
// --------------
class Object{
protected:
    Type* m_type;

public:
    Object();
    Object(Type* type);
    LYNX_DECLARE_COPY(Object);
    LYNX_DECLARE_MOVE(Object);
    virtual ~Object(){ this->m_type = nullptr; }
    Type* type(void) const{ return this->m_type; }
    virtual Str str(void) const { return "nil"; }
    virtual Str repr(void) const { return "'nil'"; }
};

// ---------------
// --- Boolean ---
// ---------------
class Boolean final: public Object{
private:
    bool m_value;

public:
    Boolean();
    Boolean(bool val);
    LYNX_DECLARE_COPY(Boolean);
    LYNX_DECLARE_MOVE(Boolean);
    ~Boolean() = default;
    LYNX_OBJECT_COMMONS();
    LYNX_SCALAR_COMMONS();
};

// ---------------
// --- Integer ---
// ---------------
class Integer final : public Object{
private:
    i64 m_num;
public:
    Integer();
    Integer(i64);
    LYNX_DECLARE_COPY(Integer);
    LYNX_DECLARE_MOVE(Integer);
    ~Integer() = default;
    LYNX_OBJECT_COMMONS();
    LYNX_SCALAR_COMMONS();
};

// -------------
// --- Float ---
// -------------
class Float final: public Object{
private:
    f64 m_num;

public:
    Float();
    Float(f64 num);
    LYNX_DECLARE_COPY(Float);
    LYNX_DECLARE_MOVE(Float);
    ~Float() = default;
    LYNX_OBJECT_COMMONS();
    LYNX_SCALAR_COMMONS();
};

// --------------
// --- Symbol ---
// --------------
// class Symbol final: public Object{
// private:
//     Str m_str;

// public:
//     Symbol(const Str& str);
//     LYNX_DECLARE_COPY(Symbol);
//     LYNX_DECLARE_MOVE(Symbol);
//     ~Symbol() = default;
//     LYNX_OBJECT_COMMONS();
// };

// --------------
// --- String ---
// --------------
class String final: public Object{
private:
    Str m_str;

public:
    String();
    String(const Str& str);
    LYNX_DECLARE_COPY(String);
    LYNX_DECLARE_MOVE(String);
    ~String() = default;
    LYNX_OBJECT_COMMONS();
};

// -------------
// --- Array ---
// -------------
class Array final: public Object{
private:
    Vec<Self> m_data;

public:
    Array();
    Array(const Vec<Self> vec);
    LYNX_DECLARE_COPY(Array);
    LYNX_DECLARE_MOVE(Array);
    ~Array();
    LYNX_OBJECT_COMMONS();
    Vec<Self> data(void) const;
};

// ------------
// --- Dict ---
// ------------
class Dict final: public Object{
public:
    struct Comparator{
        bool operator()(const Self& lhs, const Self& rhs) const;
    };
    using Map = std::map<Self, Self, Comparator>;

private:
    Map m_data;

public:
    Dict();
    Dict(std::map<Str, Self>& data);
    LYNX_DECLARE_COPY(Dict);
    LYNX_DECLARE_MOVE(Dict);
    ~Dict();
    LYNX_OBJECT_COMMONS();
    Map data(void) const;
};

// -*-
class File: public Object{
private:
    std::fstream m_stream;

public:
    File();
    File(const Str& filename);
    /*
    Mode:
        - "r", "rw", "rb", "rw+"
        - "w", "wb"
    */
    File(const Str& filename, const Str& mode);
    LYNX_DECLARE_MOVE(File);
    ~File();
    LYNX_OBJECT_COMMONS();
    std::fstream stream();
};

// -*-
class Callable final: public Object{
private:
    enum class Kind{Builtin, Function, Lambda};
    
    // - builtin
    struct Builtin{
        Str name;
        i32 arity;
        BuiltinFn fun;
        Str docstr;
        Builtin(const Str& name, i32 arity, BuiltinFn fn, Str doc="")
        : name{name}, arity{arity}, fun{fn}, docstr{doc}{}
    };

    // - user-defined function
    struct Function{
        Str name;
        Vec<Str> params;
        Vec<Self> body;
        Scope scope;
        Str docstr;
        Function(Str& name, Vec<Str> params, Vec<Self> body, Scope& scope, Str doc="")
        : name{name}, params{params}, body{body}, scope{scope}, docstr{doc}{}
    };

    // - lambda
    struct Lambda{
        Vec<Str> params;
        Vec<Self> body;
        Scope scope;
        Lambda(Vec<Str> params, Vec<Self> body, Scope& scope)
        : params{params}, body{body}, scope{scope}{}
    };

    using Fn = std::variant<Builtin, Function, Lambda>;
    Kind m_kind;
    Fn m_fn;

public:
    Callable(const Str& name, i32 arity, BuiltinFn fn, Str doc="");
    Callable(Str& name, Vec<Str> params, Vec<Self> body, Scope& scope, Str doc="");
    Callable(Vec<Str> params, Vec<Self> body, Scope& scope);
    LYNX_DECLARE_MOVE(Callable);
    ~Callable() = default;
    LYNX_OBJECT_COMMONS();

    Self operator()(Vec<Self> argv, Scope& scope);
    const Str& help(void) const;
    Str& help(void);
    bool is_builtin(void) const;
    bool is_function(void) const;
    bool is_lambda(void) const;
};

// ------------
// --- Type ---
// ------------
class Type{
protected:
    Str m_name;
    HashMap<Str, Callback> m_methods;

public:
    Type(const Str& name, const HashMap<Str, Callback> methods)
    : m_name{name}, m_methods{methods}{}
    ~Type() = default;
    Self operator()(const Str& method, Self& self, Vec<Self> argv);
    const Str name(void) const{ return this->m_name; }
    const HashMap<Str, Callback>& methods(void) const{ return this->m_methods; }
};

// -----------
// --- Env ---
// -----------
class Env final{
private:
    std::map<Str, Self> m_bindings;
    Scope m_parent;

public:
    Env();
    Env(const Env& env);
    Env(Scope& parent);
    ~Env();
    Self get(const Str& key);
    void put(const Str& key, const Self& val);
    Self update(const Str& key, const Self& val);
};

// -*-
class Error: public std::exception{
protected:
    Str m_message;

public:
    Error(): m_message{}{}
    Error(const Str& msg): m_message{msg}{}
    ~Error() = default;
    const char* what(void) const noexcept override{
        return this->m_message.c_str();
    }
    virtual const Str describe(void) const{
        std::stringstream stream;
        stream << "Error: " << this->m_message << std::endl;
        return stream.str();
    }
};

class ValueError final: public Error{
public:
    ValueError(): Error(){};
    ValueError(const Str& msg): Error(msg){}
    ~ValueError() = default;

    const Str describe(void) const override{
        std::stringstream stream;
        stream << "ValueError: " << this->m_message << std::endl;
        return stream.str();
    }
};

class TypeError final: public Error{
public:
    TypeError(): Error(){}
    TypeError(const Str& msg): Error(msg){}
    ~TypeError() = default;

    const Str describe(void) const{
        std::stringstream stream;
        stream << "TypeError:" << this->m_message << std::endl;
        return stream.str();
    }
};

class SyntaxError final: public Error{
public:
    SyntaxError(): Error(){};
    SyntaxError(const Str& msg): Error(msg){}
    ~SyntaxError() = default;

    const Str describe(void) const{
        std::stringstream stream;
        stream << "SyntaxError:" << this->m_message << std::endl;
        return stream.str();
    }
};

class RuntimeError final: public Error{
public:
    RuntimeError(): Error(){}
    RuntimeError(const Str& msg): Error(msg){}
    ~RuntimeError() = default;

    const Str describe(void) const{
        std::stringstream stream;
        stream << "RuntimeError:" << this->m_message << std::endl;
        return stream.str();
    }
};

// -*--------------------*-
// -*- Lynx Interpreter -*-
// -*--------------------*-
class Lynx{
private:
    using Stream = std::variant<std::stringstream, std::fstream>;
    Stream m_stream;

public:
    Lynx(): m_stream{}{}
    ~Lynx() = default;
    void run(const Str& source);
    void repl(const std::fstream&& stream, Vec<Str> argv);
    
    static Str version(void);
    static Str license(void);
    static Str authors(void);
    static std::map<Str, BuiltinFn> initialize_builtins(void);
    static std::map<Str, std::unique_ptr<Type>> initialize_types(void);

    // -
    static std::map<Str, std::unique_ptr<Type>> types;
    static std::map<Str, BuiltinFn> bulitins;
    static std::map<Str, Self> runtime;
    static Type* type(const Str& name){
        return Lynx::types[name].get();
    }
};

std::map<Str, BuiltinFn> Lynx::bulitins = {};
std::map<Str, Self> Lynx::runtime = {};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::lynx                                         -*-
// -*----------------------------------------------------------------*-

#endif