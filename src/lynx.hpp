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


#define LYNX_DECLARE_COPY(Name)         \
    Name(const Name& other);            \
    Name& operator=(const Name& other)


#define LYNX_DECLARE_MOVE(Name)         \
    Name(Name&& other);                 \
    Name& operator=(Name&& other)

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
class Symbol final: public Object{
private:
    Str m_str;

public:
    Symbol(const Str& str);
    LYNX_DECLARE_COPY(Symbol);
    LYNX_DECLARE_MOVE(Symbol);
    ~Symbol() = default;
    LYNX_OBJECT_COMMONS();
};

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
    Str m_filename;
    Str m_mode;
    std::fstream m_stream;
    

public:
    static std::map<Str, std::ios::openmode> openmodes;
    File();
    File(const Str& filename);
    /*
    Mode:
        - "r" : open file for reading. The file must exist on disk
        - "rb":
        - "rw": open file for reading and writing.
        - "rw+":
        - "rwb":
        - "rwb+":
        - "w": open file for writing. File must exist
        - "wb":
        - "wb+"
        - "wr": open file for reading and writing.
        - "w+"
        - "wr+"
        - "wrb+":
    */
    File(const Str& filename, const Str& mode);
    LYNX_DECLARE_MOVE(File);
    ~File(){
        if(this->m_stream.is_open()){ this->m_stream.close(); }
    }
    LYNX_OBJECT_COMMONS();

    const Str& filename(void) const{ return this->m_filename; }
    const Str& mode(void) const{ return this->m_mode; }

    bool check_openmode(const Str& mode){
        if(File::openmodes.find(mode) == File::openmodes.end()){
            return false;
        }
        return true;
    }

private:
    // -
    std::ios::openmode get_openmode(const Str& mode){
        return File::openmodes[mode];
    }
    // -
    Vec<Str> get_openmodes_list(void){
        Vec<Str> result{};
        for(auto entry: File::openmodes){
            result.push_back(entry.first);
        }
        return result;
    }
};

std::map<Str, std::ios::openmode> File::openmodes = {
    {"r", std::ios::in},
    {"rb", std::ios::in | std::ios::binary},
    {"rw", std::ios::in | std::ios::out},
    {"rw+", std::ios::in | std::ios::out | std::ios::ate},
    {"rwb", std::ios::in | std::ios::out | std::ios::binary},
    {"rwb+", std::ios::in | std::ios::out | std::ios::binary | std::ios::ate},
    {"w", std::ios::out},
    {"w+", std::ios::out| std::ios::ate},
    {"wb", std::ios::out | std::ios::binary},
    {"wb+", std::ios::out | std::ios::binary | std::ios::ate},
    {"wrb", std::ios::in | std::ios::out | std::ios::binary},
    {"wrb+", std::ios::in | std::ios::out | std::ios::binary | std::ios::ate},
};

// -*-
class Callable{
protected:
    virtual Self call(Vec<Self> argv) = 0;
};

class Builtin final: public Object, public Callable{
private:
    Str m_name;
    int m_arity;
    BuiltinFn m_fn;
    Str m_doc;

public:
    Builtin();
    Builtin(const Str& name, int arity, BuiltinFn fn, Str doc="");
    LYNX_DECLARE_MOVE(Builtin);
    ~Builtin() = default;
    LYNX_OBJECT_COMMONS();

    Self operator()(Vec<Self> argv){ return this->call(argv); }

    const Str& name(void) const { return this->m_name; }
    const int& arity(void) const { return this->m_arity; }
    const BuiltinFn& fun(void) const { return this->m_fn; }
    const Str& help(void) const { return this->m_doc; }
    Str& help(){ return this->m_doc; }

private:
    Self call(Vec<Self> argv) override{
        return this->m_fn(argv);
    }
};

// -
class Function final: public Object, public Callable{
private:
    Str m_name;
    Vec<Self> m_params;
    Vec<Self> m_body;
    Scope m_scope;
    Str m_doc;

public:
    Function();
    Function(const Str& name, Vec<Self> param, Vec<Self> body, Scope scope, Str doc="");
    LYNX_DECLARE_MOVE(Function);
    ~Function() = default;
    LYNX_OBJECT_COMMONS();

    const Str& name(void) const { return this->m_name; }
    int arity() const { return this->m_params.size(); }
    const Vec<Self>& params(void) const { return this->m_params; }
    const Vec<Self>& body(void) const { return this->m_body; }
    const Scope& scope(void) const { return this->m_scope; }
    const Str& help(void) const { return this->m_doc; }
    Str& help(void) { return this->m_doc; }

private:
    Self call(Vec<Self> argv) override;
};

// -
class Lambda final: public Object, public Callable{
private:
    Vec<Self> m_params;
    Vec<Self> m_body;
    Scope m_scope;
    
public:
    Lambda();
    Lambda(Vec<Self> params, Vec<Self> body, Scope scope);
    LYNX_DECLARE_MOVE(Lambda);
    ~Lambda() = default;
    LYNX_OBJECT_COMMONS();

    int arity() const { return this->m_params.size(); }
    const Vec<Self>& params(void) const { return this->m_params; }
    const Vec<Self>& body(void) const { return this->m_body; }
    const Scope& scope(void) const { return this->m_scope; }
    
private:
    Self call(Vec<Self> argv) override;
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