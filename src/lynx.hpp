#ifndef __LYNX_HPP__
#define __LYNX_HPP__

#include<filesystem>
#include<unordered_map>
#include<unordered_set>
#include<exception>
#include<stdexcept>
#include<iostream>
#include<fstream>
#include<sstream>
#include<variant>
#include<cassert>
#include<cstdint>
#include<string>
#include<vector>
#include<memory>
#include<complex>
#include<array>
#include<map>
#include<set>

namespace fs = std::filesystem;

#define LYNX_UNUSED(arg)    (void)arg


#define LYNX_DECLARE_COPY(Name)                 \
    Name(const Name& other) noexcept;           \
    Name& operator=(const Name& other) noexcept

#define LYNX_DELETE_COPY(Name)                  \
    Name(const Name& other) = delete;           \
    Name& operator=(const Name& other) = delete

#define LYNX_DECLARE_MOVE(Name)             \
    Name(Name&& other) noexcept;            \
    Name& operator=(Name&& other) noexcept

#define LYNX_DELETE_MOVE(Name)              \
    Name(Name&& other) = delete;            \
    Name& operator=(Name&& other) = delete

// #define LYNX_OVERLOADABLE_SPECIAL_METHODS()     \
//     virtual Self __str__(Args args);            \
//     virtual Self __repr__(Args args);           \
//     virtual Self __hash__(Args args);           \
//     virtual Self __call__(Args args)


/*
Operators:

<Arithmetic-Operator>
__add__ (+), __add_assign__ (+=),
__sub__ (-), __sub_assign__ (-=),
__mul__ (*), __mul_assign__ (*=),
__div__ (/), __div_assign__ (/=),
__pow__ (**), __pow_assign__ (**=),

<Bitwise-Operator>
__bit_not__ (~),
__bit_or__ (|), __bit_or_assign__ (|=),
__bit_and__ (&), __bit_and_assign__ (&=),
__bit_xor__ (^), __bit_xor_assign__ (^=),
__bit_shl__ (<<), __bit_shl_assign__ (<<=), 
__bit_shr__ (>>), __bit_shr_assign__ (>>=),

<Relational-Operator>
__lt__ (<), __le__ (<=), 
__gt__ (>), __ge__ (>=), 
__eq__ (==), __ne__ (!=)

<Other-Special-Operator>
__str__, __repr__, __hash__, __call__
__type__, __format__,

<Indexing>
__getitem__, __setitem__
*/


#define LYNX_OVERLOADABLE_OPS_AND_FUNS_KINDS()                          \
    LYNX_DEF(__str__, "__str__", ":s|String()")                         \
    LYNX_DEF(__repr__, "__repr__", ":r|String()")                       \
    LYNX_DEF(__hash__, "__hash__", "hash()")                            \
    LYNX_DEF(__call__, "__call__", "obj(args)")                         \
    LYNX_DEF(__next__, "__next__", "next(iterable)")                    \
    LYNX_DEF(__done__, "__done__", "done(iterable)")                    \
    LYNX_DEF(__setitem__, "__setitem__", "obj[i] = v")                  \
    LYNX_DEF(__getitem__, "__getitem__", "obj[i]")                      \
    LYNX_DEF(__setattr__, "__setattr__", "setattr(obj, attr, val)")     \
    LYNX_DEF(__getattr__, "__getattr__", "getattr(obj, attr)")          \
    LYNX_DEF(__delattr__, "__delattr__", "delattr(obj, attr)")          \
    LYNX_DEF(__bool__, "__bool__", "Bool(scalar)")                      \
    LYNX_DEF(__integer__, "__integer__", "Integer(scalar)")             \
    LYNX_DEF(__float__, "__float__", "Float(scalar)")                   \
    LYNX_DEF(__float__, "__complex__", "Complex(...)")                  \
    LYNX_DEF(__list__, "__list__", "List(iterable)")                    \
    LYNX_DEF(__tuple__, "__tuple__", "Tuple(iterable)")                 \
    LYNX_DEF(__hashmap__, "__hashmap__", "Dict(iterable)")              \
    LYNX_DEF(__hashset__, "__hashset__", "Set(iterable)")               \
    LYNX_DEF(__next__, "__next__", "next()")                            \
    LYNX_DEF(__done__, "__done__", "done()")                            \
    LYNX_DEF(__add__, "__add__", "operator+()")                         \
    LYNX_DEF(__sub__, "__sub__", "operator-()")                         \
    LYNX_DEF(__mul__, "__mul__", "operator*()")                         \
    LYNX_DEF(__div__, "__div__", "operator/()")                         \
    LYNX_DEF(__mod__, "__mod__", "operator%()")                         \
    LYNX_DEF(__pow__, "__pow__", "operator**()")                        \
    LYNX_DEF(__add_assign__, "__add_assign__", "operator+=()")          \
    LYNX_DEF(__sub_assign__, "__add_assign__", "operator-=()")          \
    LYNX_DEF(__mul_assign__, "__add_assign__", "operator*=()")          \
    LYNX_DEF(__div_assign__, "__div_assign__", "operator/=()")          \
    LYNX_DEF(__mod_assign__, "__mod_assign__", "operator%=()")          \
    LYNX_DEF(__pow_assign__, "__pow_assign__", "operator**=()")         \
    LYNX_DEF(__lt__, "__lt__", "operator<()")                           \
    LYNX_DEF(__le__, "__le__", "operator<=()")                          \
    LYNX_DEF(__gt__, "__gt__", "operator>()")                           \
    LYNX_DEF(__ge__, "__ge__", "operator>=()")                          \
    LYNX_DEF(__eq__, "__eq__", "operator==()")                          \
    LYNX_DEF(__ne__, "__ne__", "operator!=()")                          \
    LYNX_DEF(__bit_not__, "__bit_not__", "operator~()")                 \
    LYNX_DEF(__bit_or__, "__bit_or__", "operator|()")                   \
    LYNX_DEF(__bit_xor__, "__bit_xor__", "operator^()")                 \
    LYNX_DEF(__bit_and__, "__bit_and__", "operator&()")                 \
    LYNX_DEF(__bit_shl__, "__bit_shl__", "operator<<()")                \
    LYNX_DEF(__bit_shr__, "__bit_shr__", "operator>>()")                \
    LYNX_DEF(__bit_or_assign__, "__bit_or_assign__", "operator|=()")    \
    LYNX_DEF(__bit_xor_assign__, "__bit_xor_assign__", "operator^=()")  \
    LYNX_DEF(__bit_and_assign__, "__bit_and_assign__", "operator&=()")  \
    LYNX_DEF(__bit_shl_assign__, "__bit_shl_assign__", "operator<<=()") \
    LYNX_DEF(__bit_shr_assign__, "__bit_shr_assign__", "operator>>()")


// -*------------------------------------------------*-
// -*- FormatSpecs: a, s, r, (i, b, o, x), (f, e|E) -*-
// -*------------------------------------------------*-
// -
// -*----------------------------------------------------------------*-
// -*- begin::namespace::lynx                                       -*-
// -*----------------------------------------------------------------*-
namespace lynx{
// -
template<typename T>
using Shared = std::shared_ptr<T>;
template<typename T>
using Weak = std::weak_ptr<T>;
template<typename K, typename V>
using HashMap = std::map<K, V>;
template<typename T>
using HashSet = std::set<T>;
template<typename T>
using Vec = std::vector<T>;
using Str = std::string;
using usize = std::size_t;
using u32 = std::uint32_t;
using i32 = std::int32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;
using u8 = std::uint8_t;
using i8 = std::int8_t;
using f64 = double;
using Complex = std::complex<double>;
using Str = std::string;


// -----------------------
// - forward declaration -
// -----------------------
class Lynx;
// class UnitTest;
// class TestCase;

class Object;
class Set;
class Dict;
class Error;
class Result;
class Structure;
class StringIterator;
class TupleIterator;
class ListIterator;
class DictIterator;
class SetIterator;
class RangeIterator;
class Iterable;
class Token;

using Self = Shared<Object>;
using Args = Vec<Self>;
using List = Vec<Self>;
using Iterator = Shared<Iterable>;
using HSet = HashSet<Self>;
using HMap = HashMap<Self, Self>;
using CFun = Self (*)(Args);
// -*-
using Callaback = Self (Object::*)(Args);

struct AbstractAST;
using Ast = Shared<AbstractAST>;

#define LYNX_TYPE_KINDS()           \
    LYNX_DEF(None, "Nil")           \
    LYNX_DEF(Bool, "Bool")          \
    LYNX_DEF(Integer, "Integer")    \
    LYNX_DEF(Float, "Float")        \
    LYNX_DEF(Complex, "Complex")    \
    LYNX_DEF(Sym, "Symbol")         \
    LYNX_DEF(String, "String")      \
    LYNX_DEF(Tuple, "Tuple")        \
    LYNX_DEF(Vector, "List")        \
    LYNX_DEF(Set, "HashSet")        \
    LYNX_DEF(Dict, "HashMap")       \
    LYNX_DEF(Struct, "Structure")   \
    LYNX_DEF(Iter, "Iterator")      \
    LYNX_DEF(CFun, "Builtin")       \
    LYNX_DEF(Fun, "Function")       \
    LYNX_DEF(Lambda, "Lambda")      \
    LYNX_DEF(Macro, "Macro")        \
    LYNX_DEF(Result, "Result")


// -*-
class Iterable{
public:
    virtual ~Iterable() = default;
    virtual Self next(void) = 0;
    virtual Self done(void) = 0;
};

// -*-
struct Nil{};

// -*-
class Symbol{
public:
    explicit Symbol() noexcept: m_data{""}{}
    explicit Symbol(const Str& sym) noexcept;
    Symbol(const Symbol& sym) noexcept;
    Symbol(Symbol&& other) noexcept;
    Symbol& operator=(const Symbol& other) noexcept;
    Symbol& operator=(Symbol&& other) noexcept;

    bool is_defined(void) const{ return this->m_data.length()!=0; }

    operator Str();
    friend bool operator==(const Symbol& lhs, const Symbol& rhs);
    friend bool operator<(const Symbol& lhs, const Symbol& rhs);
    friend bool operator==(const Symbol& lhs, const Str& rhsStr);
    friend bool operator==(const Str& lhsStr, const Symbol& rhs);
    friend bool operator<(const Str& lhsStr, const Symbol& rhs);
    friend bool operator<(const Symbol& lhs, const Str& rhsStr);

    friend class Object;

private:
    Str m_data;
};


#define LYNX_TYPE_VARIANTS()                                \
    Nil, bool, i64, f64, Complex, Symbol, Str,              \
    List, Set, Dict, Structure, Iterator, CFun, Ast, Result


class Error final{
public:
    enum class Kind{
        Error, TypeError, ValueError, SyntaxError,
        IndexError, KeyError, SymbolError, AttributeError,
        RuntimeError,
    };
    explicit Error() noexcept;
    explicit Error(Kind kind, Str msg) noexcept;
    explicit Error(Kind kind, const Self& reason) noexcept;
    explicit Error(const Symbol& sym, Str msg) noexcept;
    explicit Error(const Symbol& sym, const Self& reason) noexcept;
    LYNX_DECLARE_COPY(Error);
    LYNX_DECLARE_MOVE(Error);
    ~Error();
    Symbol kind(void) const;
    const Self& reason(void) const;
    const Str& message(void) const;
    Str describe(void) const;

    friend class Result;

private:
    Kind m_kind;
    Self m_reason;
    Str m_msg;
    Symbol m_symbol;
};

class Result final{
public:
    explicit Result() noexcept;
    explicit Result(const Self& self) noexcept;
    explicit Result(const Error& error) noexcept;
    explicit Result(const Symbol& sym, const Error& error) noexcept;
    LYNX_DECLARE_COPY(Result);
    LYNX_DECLARE_MOVE(Result);
    ~Result();
    Self ok(void) const;
    Error err(void) const;
    bool is_ok(void) const{ return this->m_ok; }

private:
    using Value = std::variant<Self, Error>;
    Value m_value;
    bool m_ok;
};

class Structure final{
public:
    explicit Structure(const Str& name) noexcept;
    LYNX_DECLARE_COPY(Structure);
    LYNX_DECLARE_MOVE(Structure);
    ~Structure();
    Symbol name(void) const;
    const Symbol& instance_name() const{ return this->m_instance_name; }
    Symbol& instance_name(){ return this->m_instance_name; }
    
    HashMap<Symbol, Self> attributes(void) const;
    HashMap<Symbol, Self> properties(void) const;
    HashMap<Symbol, Self> object_attribues(void) const;
    HashMap<Symbol, Self> methods(void) const;
    void define_initilizer(Self initfn);            // initfn = Object(FunDefAstExpr)
    void initialize(Args args);                     // @init
    void define_attribute(Self attr);               // 
    void define_property(Self property);            // @x ...
    void define_object_attribute(Self var);         // obj:var = val
    void define_method(Self method);                // obj.fn(...)
    void delete_attribute(const Str& key);          //
    void delete_property(const Str& key);           //
    void delete_object_attribute(const Str& key);   //

    friend class Object;

private:
    // New Structure|Type Name
    Symbol m_name;
    Symbol m_instance_name;
    // store object attributes defined in structure block, but not in
    // the init function
    // to retrieve this attributes, the following syntax is used:
    // let myattributeValue = obj::attribute
    HashMap<Symbol, Self> m_attributes;

    /*
    Store object properties defined int the structure-init block using
    the following syntax:

        struct Point{
            ...
            @init(x, y){
                ...
                @x = x
                @y = y
                ....
            }
        }
        
        Accessing the properties is done as follows:
        let point = Point(2, 3.14)
        let x = point.x
        let y = point.y
    */
    HashMap<Symbol, Self> m_properties;

    /*
    Store structure's methods:

    Examples:
        struct Name{
            fun my_method1(...){ ... }
            fun my_method2(...){ ... }
            ; overload '+' operator
            @operator+(other){ ....}
            ...
            ; index-or-key-access operator
            ; Applicable for: String, Tuple, List, HashSet, HashMap
            @operator[](index){ ... }
            @operator[](index, value){ ... }
            ; implement string-conversion for Name
            fun __str__(){ ... }
            ; implement parsable-string representation
            fun __repr__(){ ... }
            ; overload builtin function
            fun __hash__(){ ... }
            ; other conversion function
            fun __bool__(){ ... }
            fun __integer__(){ ... }
            fun __float__(){ ... }
            fun __complex__(){ ... }
            fun __tuple__(){ ... }
            fun __list__(){ ... }
            fun __hashmap__(){ ... }
            fun __hashset__(){ ... }
            ; make a callable object
            fun __call__(args){ ... }
            ; iterator interface
            fun __next__(){ ... }
            fun __done__(){ ... }
            ; attribute-access operators
            fun __getattr__(...){ ... }
            fun __setattr__(...){ ... }
            fun __delattr__(...){ ... }
        }

        struct Point{
            @init(x, y){
                @x = x
                @y = y
            }
            @operator+(other){
                let x = @x + other.x
                let y = @y + other.y
                return Point(x, y)
            }

            fun __str__(){
                return format("({@x}, {@y})")
            }
        }
        
        let point1 = Point(1, 2)
        let point2 = Point(3, 2)
        let point3 = point1 + point2
        println(point1)                 ; (1, 2)
        println(point2)                 ; (3, 2)
        println(point3)                 ; (4, 4)
    */
    HashMap<Symbol, Self> m_methods;
    /*
    Store struct-object attributes. This can be accessed as follow:

        let myAttr = obj:attr
    */
    HashMap<Symbol, Self> m_object_attributes;
};

class Set final{
public:
    // -
    struct Hasher{
        usize operator()(const Self& lhs) const;
    };

    struct Equal{
        bool operator()(const Self& lhs, const Self& rhs) const;
    };

    using UserSet = std::unordered_set<Self, Hasher, Equal>;

    // -
    explicit Set() noexcept;
    explicit Set(Self iterable) noexcept;
    LYNX_DECLARE_COPY(Set);
    LYNX_DECLARE_MOVE(Set);
    ~Set();
    bool contains(const Self& key) const;
    usize len(void) const;
    void insert(const Self& item);
    bool is_disjoint(const Set& hset) const;
    bool is_superset(const Set& hset) const;
    bool is_subset(const Set& hset) const;
    void remove(const Self& item);
    void clear(void);
    Set::UserSet data(void) const;

    friend Set operator+(const Set& lhs, const Set& rhs);
    friend Set operator-(const Set& lhs, const Set& rhs);
    friend Set operator|(const Set& lhs, const Set& rhs);
    friend Set operator^(const Set& lhs, const Set& rhs);
    friend Set operator&(const Set& lhs, const Set& rhs);

    friend class Object;

private:
    UserSet m_data;
};

class Dict final{
public:
    struct Hasher{
        usize operator()(const Self& lhs) const;
    };

    struct Equal{
        bool operator()(const Self& lhs, const Self& rhs) const;
    };
    // -
    using UserDict = std::unordered_map<Self, Self, Hasher, Equal>;
    // -

    explicit Dict() noexcept;
    explicit Dict(Self iterable) noexcept;
    LYNX_DECLARE_COPY(Dict);
    LYNX_DECLARE_MOVE(Dict);
    ~Dict();
    bool contains(const Self& key) const;
    usize len(void) const;
    void remove(const Self& key);
    Self popitem(const Self& key);
    Self& operator[](const Self& key);
    const Self& operator[](const Self& key) const;
    friend Self operator+(const Dict& lhs, const Dict& rhs);
    Self values(void) const;
    Self keys(void) const;
    Self items(void) const;
    Self update(const Self& key, const Self& value);

    Dict::UserDict data(void) const{ return this->m_data; }

    friend class Object;

private:
    UserDict m_data;
};


class Object final{
public:
    enum class Kind{
    #define LYNX_DEF(tok, _) tok,
        LYNX_TYPE_KINDS()
    #undef LYNX_DEF
    };
public:
    explicit Object() noexcept;                         // None
    explicit Object(bool val) noexcept;                 // Bool
    explicit Object(i64 num) noexcept;                  // Integer
    explicit Object(f64 num) noexcept;                  // Float
    explicit Object(Complex z) noexcept;                // Complex
    explicit Object(Symbol sym) noexcept;               // Sym
    explicit Object(Str str) noexcept;                  // String
    explicit Object(Kind kind, List data) noexcept;     // Tutple, List
    explicit Object(Set data) noexcept;                // Set
    explicit Object(Dict data) noexcept;                // Dict
    explicit Object(Structure klass) noexcept;          // Struct
    explicit Object(Iterator iter) noexcept;            // Iter
    explicit Object(const Str& name, CFun cfun) noexcept;// CFun
    explicit Object(Ast ast) noexcept;                  // Fun, Lambda, Macro
    explicit Object(Result result) noexcept;            // OkErr
    LYNX_DECLARE_COPY(Object);
    LYNX_DECLARE_MOVE(Object);
    ~Object();

    // ------------------------------
    // -*- Type Casting Operators -*-
    // ------------------------------
    operator bool();
    operator i64();
    operator f64();
    operator Complex();
    operator Symbol();
    operator Str();
    operator List();
    operator Set();
    operator Dict();
    operator CFun();
    operator Result();
    operator Ast();

    // -------------------------
    // -*- Logical Operators -*-
    // -------------------------
    // Bool
    Object& operator!();
    // Bool
    friend Object operator&&(const Object& lhs, const Object& rhs);
    // Bool
    friend Object operator||(const Object& lhs, const Object& rhs);

    // -------------------------
    // -*- Bitwise Operators -*-
    // -------------------------
    // Num
    Object& operator~();
    // Num, Set (union)
    friend Object operator|(const Object& lhs, const Object& rhs);
    // Num, Set (intersection)
    friend Object operator&(const Object& lhs, const Object& rhs);
    // Num, Set (symetric_diff)
    friend Object operator^(const Object& lhs, const Object& rhs);
    // Num
    friend Object operator<<(const Object& lhs, const Object& rhs);
    // Num
    friend Object operator>>(const Object& lhs, const Object& rhs);

    // --------------------------
    // -*- Relation Operators -*-
    // --------------------------
    friend Object operator==(const Object& lhs, const Object& rhs);
    friend Object operator!=(const Object& lhs, const Object& rhs);
    friend Object operator<=(const Object& lhs, const Object& rhs);
    friend Object operator>=(const Object& lhs, const Object& rhs);
    friend Object operator<(const Object& lhs, const Object& rhs);
    friend Object operator>(const Object& lhs, const Object& rhs);

    // ----------------------------
    // -*- Arithmetic Operators -*-
    // ----------------------------
    // Num, Set (union), Tuple, List, Dict
    friend Object operator+(const Object& lhs, const Object& rhs);
    // Num, Set (difference)
    friend Object operator-(const Object& lhs, const Object& rhs);
    // Num
    friend Object operator/(const Object& lhs, const Object& rhs);
    // Num
    friend Object operator%(const Object& lhs, const Object& rhs);
    // Num
    friend Object operator*(const Object& lhs, const Object& rhs);
    // Num, Set (union), Tuple, List, Dict
    Object& operator+=(const Object& lhs);
    // Num, Set (difference)
    Object& operator-=(const Object& lhs);
    // Num
    Object& operator/=(const Object& lhs);
    // Num
    Object& operator%=(const Object& lhs);
    // Num
    Object& operator*=(const Object& lhs);

    // -----------------------
    // -*- Index Operators -*-
    // -----------------------
    // String, List
    Self& operator[](int idx);
    // String, Tuple, List
    const Self& operator[](int idx) const;
    Self& operator[](const Self& key);
    const Self& operator[](const Self& key) const;

    // ------------------
    // -*- Predicates -*-
    // ------------------
    bool is_nil(void) const;
    bool is_bool(void) const;
    bool is_integer(void) const;
    bool is_float(void) const;
    bool is_complex(void) const;
    bool is_number(void) const;
    bool is_symbol(void) const;
    bool is_string(void) const;
    bool is_tuple(void) const;
    bool is_list(void) const;
    bool is_hashset(void) const;
    bool is_hashmap(void) const;
    bool is_builtin(void) const;
    bool is_function(void) const;
    bool is_macro(void) const;
    bool is_lambda(void) const;
    bool is_callable(void) const;
    bool is_hashable(void) const;
    bool is_iterable(void) const;
    bool is_iterator(void) const;
    bool is_result(void) const;
    bool is_ok(void) const;
    bool is_structure(void) const;
    // -
    bool is_builitn_type(void) const;

    const bool& is_structure_instance(void) const{
        return this->m_is_structure_instance;
    }

    bool& is_structure_instance(void){
        return this->m_is_structure_instance;
    }

    void as_version(void){ this->m_is_version = true; }
    bool is_version(void) const{ return this->m_is_version; }

    void as_newtype(void){ this->m_newtype = true; }
    bool is_newtype(void) const{ return this->m_newtype; }

    void as_constant(void){ this->m_constant = true; }
    bool is_constant(void) const{ return this->m_constant; }

    void as_fixed_type(void){ this->m_fixed_type = true; }
    bool is_fixed_type(void) const { return this->m_fixed_type; }

    // ------------------------------
    // -*- fn, fun, lambda, macro -*-
    // ------------------------------
    Self operator()(Args args);

    // ------------------------
    // -*- call type method -*-
    // ------------------------
    Self operator()(const Str& fname, Args args);

    // -*-
    Str format(void) const;
    Self ok(void) const;
    Error err(void) const;

    // -*-
    Symbol type(void) const;
    Str repr(void) const;
    usize hash(void) const;
    Iterator iter(void) const;

    Str& docstr(void){ return this->m_doc; }
    const Str& doctrs(void) const{ return this->m_doc; }

    // -------------------------------
    // -*- Complex numbers methods -*-
    // -------------------------------
    Self real(Args);
    Self imag(Args);
    Self abs(Args);
    Self arg(Args);
    Self norm(Args);
    Self conj(Args);
    Self polar(Args); 

    // -*---------------------------------
    // -*- Sequence Specific Operators -*-
    // -----------------------------------
    Self clear(Args args);
    // String, Tuple, List, Set, Dict
    Self len(Args args);
    Self find(Args args);
    Self find_all(Args args);
    Self find_last(Args args);
    // String, Tuple, List
    Self slice(Args args);
    Self sort(Args args);

    // ---------------------------------
    // -*- String Specific Operators -*-
    // ---------------------------------
    Self upper(Args args);
    Self lower(Args args);
    Self capitalize(Args args);
    Self title(Args args);
    Self split(Args args);
    Self join(Args args);
    Self endswith(Args args);
    Self startswith(Args args);
    Self chr(Args args);
    Self ord(Args args);

    // -*- List Specific methods -*-
    Self index(Args args);
    Self remove(Args args); // also for Set.
    Self insert(Args args);
    Self head(Args args);
    Self tail(Args args);
    Self last(Args args);
    // Self extend(Args args); // lhs + rhs
    Self append(Args args); // also for Set, aliased as 'add'
    Self pop(Args args);
    Self push(Args args);

    // -*- Dict -*-
    Self keys(Args args);
    Self values(Args args);
    Self items(Args args);
    Self popitem(Args args);
    Self update(Args args);

    // -*- Structure Specifics -*-
    Self setattr(Args args);
    Self getattr(Args args);
    Self hasattr(Args args);
    Self delattr(Args args);

    friend class Set;
    friend class Dict;

    friend std::ostream& operator<<(std::ostream& os, const Object& obj);

private:
    using Value = std::variant<LYNX_TYPE_VARIANTS()>;
    Object::Kind m_kind;
    Value m_value;
    Symbol m_name; // builtin function name
    bool m_is_version;
    bool m_newtype;
    bool m_constant;
    bool m_fixed_type;
    bool m_is_structure_instance;

    Str m_doc = ""; // builtin function documentation string

    friend class StringIterator;
    friend class TupleIterator;
    friend class ListIterator;
    friend class DictIterator;
    friend class SetIterator;
    friend class RangeIterator;

protected:
    // Arithmethic-ops
    Self __add__(Args args);
    Self __sub__(Args args);
    Self __mul__(Args args);
    Self __div__(Args args);
    Self __mod__(Args args);
    Self __pow__(Args args);
    Self __add_assign__(Args args);
    Self __sub_assign__(Args args);
    Self __mul_assign__(Args args);
    Self __div_assign__(Args args);
    Self __mod_assign__(Args args);
    Self __pow_assign__(Args args);
    // // Logical-ops
    // Self __logical_or__(Args args);
    // Self __logical_and__(Args args);
    // Self __logical_not__(Args args);
    // Relational-ops
    Self __lt__(Args args);
    Self __le__(Args args);
    Self __gt__(Args args);
    Self __ge__(Args args);
    Self __eq__(Args args);
    Self __ne__(Args args);
    // Bitwise-ops
    Self __bit_not__(Args args);
    Self __bit_or__(Args args);
    Self __bit_xor__(Args args);
    Self __bit_and__(Args args);
    Self __bit_shl__(Args args);
    Self __bit_shr__(Args args);
    Self __bit_or_assign__(Args args);
    Self __bit_xor_assign__(Args args);
    Self __bit_and_assign__(Args args);
    Self __bit_shl_assign__(Args args);
    Self __bit_shr_assign__(Args args);
    // Indexing
    Self __getitem__(Args args);
    Self __setitem__(Args args);
    // Slicing-ops
    Self __slice__(Args args);
    // Iterable
    Self __next__(Args args);
    Self __done__(Args args);
    // Property-handler
    Self __setattr__(Args args);
    Self __getattr__(Args args);
    Self __delattr__(Args args);
    // String
    Self __str__(Args args);
    // Parse-able string
    Self __repr__(Args args);
    // Hashable
    Self __hash__(Args args);
    // Callable
    Self __call__(Args args);
    // typecat-operators
    Self __bool__(Args args);
    Self __integer__(Args args);
    Self __float__(Args args);
    Self __cmplx__(Args args);
    Self __tuple__(Args args);
    Self __list__(Args args);
    Self __hashset__(Args args);
    Self __hashmap__(Args args);
};

// -*-
class StringIterator final: public Iterable{
public:
    explicit StringIterator(const Self& data) noexcept;
    LYNX_DECLARE_MOVE(StringIterator);
    ~StringIterator();
    Self next(void) override;
    Self done(void) override;

private:
    Self m_data;
};

class TupleIterator final: public Iterable{
public:
    explicit TupleIterator(const Self& data) noexcept;
    LYNX_DECLARE_MOVE(TupleIterator);
    ~TupleIterator();
    Self next(void) override;
    Self done(void) override;

private:
    Self m_data;
};

// -*-
class ListIterator final: public Iterable{
public:
    explicit ListIterator(const Self& data) noexcept;
    LYNX_DECLARE_MOVE(ListIterator);
    ~ListIterator();
    Self next(void) override;
    Self done(void) override;

private:
    Self m_data;
};

// -*-
class DictIterator final: public Iterable{
public:
    explicit DictIterator(const Self& data) noexcept;
    LYNX_DECLARE_MOVE(DictIterator);
    ~DictIterator();
    Self next(void) override;
    Self done(void) override;

private:
    Self m_data;
};

// -*-
class SetIterator final: public Iterable{
public:
    explicit SetIterator(const Self& data) noexcept;
    LYNX_DECLARE_MOVE(SetIterator);
    ~SetIterator();
    Self next(void) override;
    Self done(void) override;

private:
    Self m_data;
};

// -*-
class RangeIterator final: public Iterable{
public:
    explicit RangeIterator(const Self& start, const Self& stop) noexcept;
    explicit RangeIterator(const Self& start, const Self& stop, const Self& step) noexcept;
    LYNX_DECLARE_MOVE(RangeIterator);
    ~RangeIterator();
    Self next(void) override;
    Self done(void) override;

private:
    Self m_start;
    Self m_stop;
    Self m_step;
};

// -*-
#define LYNX_RESERVED_WORD_KINDS()          \
    LYNX_DEF(Import, "import")              \
    LYNX_DEF(From, "from")                  \
    LYNX_DEF(In, "in")                      \
    LYNX_DEF(Fun, "fun")                    \
    LYNX_DEF(Macro, "macro")                \
    LYNX_DEF(Lambda, "lambda")              \
    LYNX_DEF(Struct, "struct")              \
    LYNX_DEF(When, "when")                  \
    LYNX_DEF(Match, "match")                \
    LYNX_DEF(With, "with")                  \
    LYNX_DEF(If, "if")                      \
    LYNX_DEF(Elif, "elif")                  \
    LYNX_DEF(Else, "else")                  \
    LYNX_DEF(For, "for")                    \
    LYNX_DEF(While, "while")                \
    LYNX_DEF(Break, "break")                \
    LYNX_DEF(Continue, "continue")          \
    LYNX_DEF(Return, "return")              \
    LYNX_DEF(Let, "let")                    \
    LYNX_DEF(Var, "var")                    \
    LYNX_DEF(Ok, "Ok")                      \
    LYNX_DEF(Err, "Err")                    \
    LYNX_DEF(AtVersion, "@version")         \
    LYNX_DEF(AtInit, "@init")               \
    LYNX_DEF(AtMain, "@main")               \
    LYNX_DEF(AtArgv, "@argv")               \
    LYNX_DEF(AtTesting, "@testing")         \
    LYNX_DEF(AtTest, "@test")               \
    LYNX_DEF(AtOperator, "@operator")       \
    LYNX_DEF(AtSetup, "@setup")             \
    LYNX_DEF(AtTeardown, "@teardown")       \
    LYNX_DEF(AtDefine, "@define")           \
    LYNX_DEF(AtDoc, "@doc")                 \
    LYNX_DEF(Key, ":keys")                  \
    LYNX_DEF(Optional, ":optional")         \
    LYNX_DEF(Varargs, ":varargs")           \
    LYNX_DEF(None, "nil")                   \
    LYNX_DEF(True, "true")                  \
    LYNX_DEF(False, "false")


#define LYNX_OPERATOR_KINDS()       \
    LYNX_DEF(And, "and")            \
    LYNX_DEF(Or, "or")              \
    LYNX_DEF(Not, "not")            \
    LYNX_DEF(Plus, "+")             \
    LYNX_DEF(Minus, "-")            \
    LYNX_DEF(Div, "/")              \
    LYNX_DEF(Mod, "%")              \
    LYNX_DEF(Pow, "**")             \
    LYNX_DEF(AddAssign, "+=")       \
    LYNX_DEF(SubAssign, "-=")       \
    LYNX_DEF(DivAssign, "/=")       \
    LYNX_DEF(ModAssign, "%=")       \
    LYNX_DEF(PowAssign, "**=")      \
    LYNX_DEF(BitNot, "~")           \
    LYNX_DEF(BitOr, "|")            \
    LYNX_DEF(BitAnd, "&")           \
    LYNX_DEF(BitXor, "^")           \
    LYNX_DEF(BitShl, "<<")          \
    LYNX_DEF(BitShr, ">>")          \
    LYNX_DEF(BitOrAssign, "|=")     \
    LYNX_DEF(BitAndAssign, "&=")    \
    LYNX_DEF(BitXorAssign, "^=")    \
    LYNX_DEF(BitShlAssign, "<<=")   \
    LYNX_DEF(BitShrAssign, ">>=")


#define LYNX_TOKEN_KINDS()          \
    LYNX_DEF(Invalid, "INVALID")    \
    LYNX_DEF(LComment, "LComment")  \
    LYNX_DEF(MLComment, "MLComment")\
    LYNX_DEF(Eof, "EOF")            \
    LYNX_DEF(Ident, "IDENTIFIER")   \
    LYNX_DEF(LParen, "(")           \
    LYNX_DEF(RParen, ")")           \
    LYNX_DEF(LBrace, "{")           \
    LYNX_DEF(RBrace, "}")           \
    LYNX_DEF(LBracket, "[")         \
    LYNX_DEF(RBracket, "]")         \
    LYNX_DEF(Arrow, "->")           \
    LYNX_DEF(At, "@")               \
    LYNX_DEF(Pound, "#")            \
    LYNX_DEF(Dollar, "$")           \
    LYNX_DEF(DollarBang, "$!")      \
    LYNX_DEF(Bang, "!")             \
    LYNX_DEF(Semicolon, ";")        \
    LYNX_DEF(Colon, ":")            \
    LYNX_DEF(DoubleColon, "::")     \
    LYNX_DEF(Comma, ",")            \
    LYNX_DEF(Dot, ".")              \
    LYNX_DEF(Integer, "Integer")    \
    LYNX_DEF(Float, "Float")        \
    LYNX_DEF(Sym, "Symbol")         \
    LYNX_DEF(String, "String")      \
    LYNX_DEF(Tuple, "Tuple")        \
    LYNX_DEF(List, "List")          \
    LYNX_DEF(Set, "Set")            \
    LYNX_DEF(Dict, "Dict")          \
    LYNX_RESERVED_WORD_KINDS()      \
    LYNX_OPERATOR_KINDS()


// -*-
enum class TokenKind{
#define LYNX_DEF(tok, _) tok,
    LYNX_TOKEN_KINDS()
#undef LYNX_DEF
};

// -*-
class Token final{
public:
    explicit Token();
    explicit Token(TokenKind kind, usize row, usize col, Str::iterator beg) noexcept;
    explicit Token(TokenKind kind, usize row, usize col, Str::iterator beg, Str::iterator end) noexcept;
    Token(const Token& token) noexcept;
    Token(Token&& token) noexcept;
    
    Token& operator=(const Token& token);
    Token& operator=(Token&& token);
    ~Token();
    TokenKind kind(void) const;
    usize row(void) const;
    usize column(void) const;
    Str lexme(void);

private:
    TokenKind m_kind;
    usize m_row;
    usize m_col;
    Str::iterator m_beg;
    Str::iterator m_end;
};

class Tokenizer final{
public:
    explicit Tokenizer() noexcept;
    explicit Tokenizer(Str::iterator beg, Str::iterator end) noexcept;
    Tokenizer& operator=(const Tokenizer& tokenizer) noexcept;
    Tokenizer& operator=(Tokenizer&& tokenizer) noexcept;
    ~Tokenizer();
    Vec<Token> tokenize(void) const;

private:
    Str::iterator m_srcBeg;
    Str::iterator m_srcEnd;
    int m_pos;

    static int unescape(int c);
private:
    int peek(int idx=0);
    bool match(usize len);
    void skip_line_comment(void);
    void skip_multiline_comment(void);
    void advance(int count=0);
};


#define LYNX_AST_KINDS()                                        \
    LYNX_DEF(Invalid, "INVALID_AST")                            \
    LYNX_DEF(Identifier, "IDENTIFIER")                          \
    LYNX_DEF(BoolLiteral, "BOOL_LITERAL")                       \
    LYNX_DEF(IntegerLiteral, "INTEGER_LITERAL")                 \
    LYNX_DEF(FloatLiteral, "FLOAT_LITERAL")                     \
    LYNX_DEF(ComplexExpr, "COMPLEX_EXPR")                       \
    LYNX_DEF(SymbolLiteral, "SYMBOL_LITERAL")                   \
    LYNX_DEF(StringLiteral, "STRING_LITERAL")                   \
    LYNX_DEF(TupleLiteral, "TUPLE_LITERAL")                     \
    LYNX_DEF(ListLiteral, "LIST_LITERAL")                       \
    LYNX_DEF(SetLiteral, "SET_LITERAL")                         \
    LYNX_DEF(DictLiteral, "DICT_LITERAL")                       \
    LYNX_DEF(DestructuringExpr, "DESTRUCTURING_EXPR")           \
    LYNX_DEF(ParameterListExpr, "PARAMETER_LIST_EXPR")          \
    LYNX_DEF(StructExpr, "STRUCT_EXPR")                         \
    LYNX_DEF(LambdaDefExpr, "LAMBDA_DEF_EXPR")                  \
    LYNX_DEF(FunDefExpr, "FUN_DEF_EXPR")                        \
    LYNX_DEF(MacroDefExpr, "MACRO_DEF_EXPR")                    \
    LYNX_DEF(GetExpr, "GET_EXPR")                               \
    LYNX_DEF(SetExpr, "SETEXPR")                                \
    LYNX_DEF(FCallExpr, "FCALL_EXPR")                           \
    LYNX_DEF(MCallExpr, "MCALL_EXPR")                           \
    LYNX_DEF(Base, "BASE_AST")                                  \
    LYNX_DEF(TernaryExpr, "TERNARY_EXPR")                       \
    LYNX_DEF(BinaryExpr, "BINARY_EXPR")                         \
    LYNX_DEF(UnaryExpr, "UNARY_EXPR")                           \
    LYNX_DEF(NullaryExpr, "NULLARY_EXPR")                       \
    LYNX_DEF(Ident, "IDENTIFIER")                               \
    LYNX_DEF(MinusExpr, "MINUS_EXPR")                           \
    LYNX_DEF(PlusExpr, "PLUS_EXPR")                             \
    LYNX_DEF(AddExpr, "ADD_EXPR")                               \
    LYNX_DEF(SubExpr, "SUB_EXPR")                               \
    LYNX_DEF(DivExpr, "DIV_EXPR")                               \
    LYNX_DEF(ModExpr, "MOD_EXPR")                               \
    LYNX_DEF(PowExpr, "POW_EXPR")                               \
    LYNX_DEF(AddAssignExpr, "ADD_ASSIGN_EXPR")                  \
    LYNX_DEF(SubAssignExpr, "SUB_ASSIGN_EXPR")                  \
    LYNX_DEF(DivAssignExpr, "DIV_ASSIGN_EXPR")                  \
    LYNX_DEF(ModAssignExpr, "MOD_ASSIGN_EXPR")                  \
    LYNX_DEF(PowAssignExpr, "POW_ASSIGN_EXPR")                  \
    LYNX_DEF(LogicalOrExpr, "LOGICAL_OR_EXPR")                  \
    LYNX_DEF(LogicalAndExpr, "LOGICAL_AND_EXPR")                \
    LYNX_DEF(LogicalNotExpr, "LOGICAL_NOT_EXPR")                \
    LYNX_DEF(BitNotExpr, "BIT_NOT_EXPR")                        \
    LYNX_DEF(BitOrExpr, "BIT_OR_EXPR")                          \
    LYNX_DEF(BitAndExpr, "BIT_AND_EXPR")                        \
    LYNX_DEF(BitXorExpr, "BIT_XOR_EXPR")                        \
    LYNX_DEF(BitShlExpr, "BIT_SHL_EXPR")                        \
    LYNX_DEF(BitShrExpr, "BIT_SHR_EXPR")                        \
    LYNX_DEF(BitOrAssignExpr, "BIT_OR_ASSIGN_EXPR")             \
    LYNX_DEF(BitAndAssignExpr, "BIT_AND_ASSIGN_EXPR")           \
    LYNX_DEF(BitXorAssignExpr, "BIT_XOR_ASSIGN_EXPR")           \
    LYNX_DEF(BitShlAssignExpr, "BIT_SHL_ASSIGN_EXPR")           \
    LYNX_DEF(BitShrAssignExpr, "BIT_SHR_ASSIGN_EXPR")           \
    LYNX_DEF(CreateStructObjExpr, "CREATE_STRUCT_OBJ_EXPR")     \
    LYNX_DEF(AtInitExpr, "AT_INIT_EXPR")                        \
    LYNX_DEF(AtMainExpr, "AT_MAIN_EXPR")                        \
    LYNX_DEF(AtTestingExpr, "AT_TESTING_EXPR")                  \
    LYNX_DEF(AtTestExpr, "AT_TEST_EXPR")                        \
    LYNX_DEF(AtSetupExpr, "AT_SETUP_EXPR")                      \
    LYNX_DEF(AtTeardownExpr, "AT_TEARDOWN_EXPR")                \
    LYNX_DEF(AtDefineExpr, "AT_DEFINE_EXPR")                    \
    LYNX_DEF(AtOperatorExpr, "AT_DOC_EXPR")                     \
    LYNX_DEF(AtDoc, "AT_DOC_EXPR")                              \
    LYNX_DEF(WhenExpr, "WHEN_EXPR")                             \
    LYNX_DEF(ImportStmt, "IMPORT_STMT")                         \
    LYNX_DEF(LetStmt, "LET_STMT")                               \
    LYNX_DEF(VarStmt, "VAR_STMT")                               \
    LYNX_DEF(IfStmt, "IF_STMT")                                 \
    LYNX_DEF(ForStmt, "FOR_STMT")                               \
    LYNX_DEF(WhileStmt, "WHILE_STMT")                           \
    LYNX_DEF(BreakStmt, "BREAK_STMT")                           \
    LYNX_DEF(ContinueStmt, "CONTINUE_STMT")                     \
    LYNX_DEF(ReturnStmt, "RETURN_STMT")                         \
    LYNX_DEF(BlockStmt, "BLOCK_STMT")                           \
    LYNX_DEF(ResultExpr, "RESULT_EXPR")                         \
    LYNX_DEF(LessExpr, "LESS_EXPR")                             \
    LYNX_DEF(LessEqExpr, "LESS_EQUAL_EXPR")                     \
    LYNX_DEF(GreaterExpr, "GREATER_EXPR")                       \
    LYNX_DEF(GreaterEqExpr, "GREATER_EQUAL_EXPR")               \
    LYNX_DEF(EqualExpr, "EQUAL_EXPR")                           \
    LYNX_DEF(NotEqualExpr, "NOT_EQUAL_EXPR")                    \
    LYNX_DEF(MatchExpr, "MATCH_EXPR")                           \
    LYNX_DEF(WithExpr, "WITH_EXPR")                             \
    LYNX_DEF(AtArgvExpr, "AT_ARGV_EXPR")                        \
    LYNX_DEF(AtVersionExpr, "AT_VERSION_EXPR")


enum class AstKind{
#define LYNX_DEF(tok, _) tok,
    LYNX_AST_KINDS()
#undef LYNX_DEF
};

// -*-
#define LYNX_DECLARE_AST_STRUCT(Name) struct Name##Ast

// [000]
LYNX_DECLARE_AST_STRUCT(Identifier);
// [001]
LYNX_DECLARE_AST_STRUCT(NilLiteral);
// [002]
LYNX_DECLARE_AST_STRUCT(BoolLiteral);
// [003]
LYNX_DECLARE_AST_STRUCT(IntegerLiteral);
// [004]
LYNX_DECLARE_AST_STRUCT(FloatLiteral);
// [005]
LYNX_DECLARE_AST_STRUCT(ComplexExpr);
// [006]
LYNX_DECLARE_AST_STRUCT(SymbolLiteral);
// [007]
LYNX_DECLARE_AST_STRUCT(StringLiteral);
// [008]
LYNX_DECLARE_AST_STRUCT(TupleLiteral);
// [009]
LYNX_DECLARE_AST_STRUCT(ListLiteral);
// [010]
LYNX_DECLARE_AST_STRUCT(SetLiteral);
// [011]
LYNX_DECLARE_AST_STRUCT(DictLiteral);
// [011]
LYNX_DECLARE_AST_STRUCT(ParameterListExpr);
// [012]
LYNX_DECLARE_AST_STRUCT(StructDefExpr);
// [013]
LYNX_DECLARE_AST_STRUCT(LambdaDefExpr);
// [014]
LYNX_DECLARE_AST_STRUCT(FunDefExpr);
// [015]
LYNX_DECLARE_AST_STRUCT(MacroDefExpr);
// [016]
LYNX_DECLARE_AST_STRUCT(GetExpr);
// [017]
LYNX_DECLARE_AST_STRUCT(SetExpr);
// [018]
LYNX_DECLARE_AST_STRUCT(FCallExpr);
// [019]
LYNX_DECLARE_AST_STRUCT(MCallExpr);
// [020]
LYNX_DECLARE_AST_STRUCT(Base);
// [021]
LYNX_DECLARE_AST_STRUCT(TernaryExpr);
// [022]
LYNX_DECLARE_AST_STRUCT(BinaryExpr);
// [023]
LYNX_DECLARE_AST_STRUCT(UnaryExpr);
// [024]
LYNX_DECLARE_AST_STRUCT(NullaryExpr);
// [025]
LYNX_DECLARE_AST_STRUCT(Identifier);
// [026]
LYNX_DECLARE_AST_STRUCT(MinusExpr);
// [027]
LYNX_DECLARE_AST_STRUCT(PlusExpr);
// [028]
LYNX_DECLARE_AST_STRUCT(AddExpr);
// [029]
LYNX_DECLARE_AST_STRUCT(SubExpr);
// [030]
LYNX_DECLARE_AST_STRUCT(DivExpr);
// [031]
LYNX_DECLARE_AST_STRUCT(ModExpr);
// [032]
LYNX_DECLARE_AST_STRUCT(PowExpr);
// [033]
LYNX_DECLARE_AST_STRUCT(AddAssignExpr);
// [034]
LYNX_DECLARE_AST_STRUCT(SubAssignExpr);
// [035]
LYNX_DECLARE_AST_STRUCT(DivAssignExpr);
// [036]
LYNX_DECLARE_AST_STRUCT(ModAssignExpr);
// [037]
LYNX_DECLARE_AST_STRUCT(PowAssignExpr);
// [038]
LYNX_DECLARE_AST_STRUCT(LogicalOrExpr);
// [039]
LYNX_DECLARE_AST_STRUCT(LogicalAndExpr);
// [040]
LYNX_DECLARE_AST_STRUCT(LogicalNotExpr);
// [041]
LYNX_DECLARE_AST_STRUCT(BitNotExpr);
// [042]
LYNX_DECLARE_AST_STRUCT(BitOrExpr);
// [043]
LYNX_DECLARE_AST_STRUCT(BitAndExpr);
// [044]
LYNX_DECLARE_AST_STRUCT(BitXorExpr);
// [045]
LYNX_DECLARE_AST_STRUCT(BitShlExpr);
// [046]
LYNX_DECLARE_AST_STRUCT(BitShrExpr);
// [047]
LYNX_DECLARE_AST_STRUCT(BitOrAssignExpr);
// [048]
LYNX_DECLARE_AST_STRUCT(BitAndAssignExpr);
// [049]
LYNX_DECLARE_AST_STRUCT(BitXorAssignExpr);
// [050]
LYNX_DECLARE_AST_STRUCT(BitShlAssignExpr);
// [051]
LYNX_DECLARE_AST_STRUCT(BitShrAssignExpr);
// [052]
LYNX_DECLARE_AST_STRUCT(CreateStructObjExpr);
// [053]
LYNX_DECLARE_AST_STRUCT(AtInitExpr);
// [054]
LYNX_DECLARE_AST_STRUCT(AtMainExpr);
// [055]
LYNX_DECLARE_AST_STRUCT(AtTestingExpr);
// [056]
LYNX_DECLARE_AST_STRUCT(AtTestExpr);
// [057]
LYNX_DECLARE_AST_STRUCT(AtSetupExpr);
// [058]
LYNX_DECLARE_AST_STRUCT(AtTeardownExpr);
// [059]
LYNX_DECLARE_AST_STRUCT(AtDefineExpr);
// [060]
LYNX_DECLARE_AST_STRUCT(AtOperatorExpr);
// [061]
LYNX_DECLARE_AST_STRUCT(AtDocExpr);
// [062]
LYNX_DECLARE_AST_STRUCT(WhenExpr);
// [063]
LYNX_DECLARE_AST_STRUCT(BlockStmt);
// [064]
LYNX_DECLARE_AST_STRUCT(LetStmt);
// [065]
LYNX_DECLARE_AST_STRUCT(VarStmt);
// [066]
LYNX_DECLARE_AST_STRUCT(IfStmt);
// [067]
LYNX_DECLARE_AST_STRUCT(ForStmt);
// [068]
LYNX_DECLARE_AST_STRUCT(WhileStmt);
// [069]
LYNX_DECLARE_AST_STRUCT(BreakStmt);
// [070]
LYNX_DECLARE_AST_STRUCT(ContinueStmt);
// [071]
LYNX_DECLARE_AST_STRUCT(ReturnStmt);
// [072]
LYNX_DECLARE_AST_STRUCT(ImportStmt);
// [073]
LYNX_DECLARE_AST_STRUCT(DestructuringExpr);
// [074]
LYNX_DECLARE_AST_STRUCT(ResultExpr);
// [075]
LYNX_DECLARE_AST_STRUCT(LessExpr);
// [076]
LYNX_DECLARE_AST_STRUCT(LessEqExpr);
// [077]
LYNX_DECLARE_AST_STRUCT(GreaterExpr);
// [078]
LYNX_DECLARE_AST_STRUCT(GreaterEqExpr);
// [079]
LYNX_DECLARE_AST_STRUCT(EqualExpr);
// [080]
LYNX_DECLARE_AST_STRUCT(NotEqualExpr);
// [081]
LYNX_DECLARE_AST_STRUCT(MatchExpr);
// [082]
LYNX_DECLARE_AST_STRUCT(WithExpr);
// [083]
LYNX_DECLARE_AST_STRUCT(AtArgvExpr);
// [084]
LYNX_DECLARE_AST_STRUCT(AtVersionExpr);

// -*-
class Parameter final{
public:
    enum class Kind{Unknown, Positional, Optional, Keyword, Varargs};
    explicit Parameter() noexcept
    : m_kind{Kind::Unknown}
    , m_name{""}
    , m_value{nullptr}
    {}

    explicit Parameter(Symbol name) noexcept
    : m_kind{Kind::Positional}
    , m_name{name}
    , m_value{nullptr}
    {}

    explicit Parameter(Parameter::Kind kind, Symbol name, Self value=nullptr) noexcept
    : m_kind{kind}
    , m_name{name}
    , m_value{value}
    {}
    LYNX_DECLARE_COPY(Parameter);
    explicit Parameter(Parameter&& param) noexcept
    : m_kind{std::move(param.m_kind)}
    , m_name{std::move(param.m_name)}
    , m_value{std::move(param.m_value)}
    {}
    Parameter& operator=(Parameter&& param) noexcept{
        if(this != &param){
            this->m_kind = std::move(param.m_kind);
            this->m_name = std::move(param.m_name);
            this->m_value = std::move(param.m_value);
        }
        return *this;
    }

    bool is_unknown(void) const{
        return this->m_kind==Parameter::Kind::Unknown;
    }
    
    bool is_positional(void) const{
        return this->m_kind==Parameter::Kind::Positional;
    }

    bool is_optional(void) const{
        return this->m_kind==Parameter::Kind::Optional;
    }
    
    bool is_keyword(void) const{
        return this->m_kind==Parameter::Kind::Keyword;
    }

    bool is_varargs(void) const{
        return this->m_kind==Parameter::Kind::Varargs;
    }

    ~Parameter() = default;
    const Symbol& name(void) const{ return this->m_name; }
    const Self& value(void) const{ return this->m_value; }
    Self& value(void){ return this->m_value; }

    // -
    friend bool operator==(const Parameter& lhs, const Parameter& rhs){
        return lhs.name() == rhs.name();
    }
    friend bool operator==(const Str& lhs, const Parameter& rhs){
        return Symbol(lhs)==rhs.name();
    }
    friend bool operator==(const Parameter& lhs, const Str& rhs){
        return lhs.name()==Symbol(rhs);
    }
    friend bool operator<(const Parameter& lhs, const Parameter& rhs){
        return lhs.name() < rhs.name();
    }
    friend bool operator<(const Str& lhs, const Parameter& rhs){
        return Symbol(lhs) < rhs.name();
    }
    friend bool operator<(const Parameter& lhs, const Str& rhs){
        return lhs.name() < Symbol(rhs);
    }

private:
    Parameter::Kind m_kind;
    Symbol m_name;
    Self m_value;
};

#define LYNX_OVERLOAD_AST_COMMONS()     \
    Self eval(Lynx& interp) override;   \
    Str repr(void) const override;      \
    Str str(void) const override

#define LYNX_VIRTUAL_OVERLOAD_AST_COMMONS()     \
    virtual Self eval(Lynx& interp) override;   \
    virtual Str repr(void) const override;      \
    virtual Str str(void) const override

// -*-
struct AbstractAST{
public:
    AbstractAST(AstKind kind, Ast& root, Vec<Token>::iterator beg , Vec<Token>::iterator end);
    virtual ~AbstractAST();
    virtual Self eval(Lynx& interp) = 0;
    virtual Str repr(void) const = 0;
    virtual Str str(void) const = 0;

    const AstKind& kind(void) const { return this->m_kind; }
    AstKind& kind(void){ return this->m_kind; }
    Ast& root(void){ return this->m_root; }
    const Ast root(void) const{ return this->m_root; }
    Vec<Token>::iterator begin(void) const{ return this->m_begin; }
    Vec<Token>::iterator end(void) const{ return this->m_end; }
    Vec<Token> tokens(void) const{ return this->m_tokens; }

protected:
    AstKind m_kind;
    Ast m_root;
    Vec<Token>::iterator m_begin;
    Vec<Token>::iterator m_end;
    Vec<Token> m_tokens;
};

class Parser final{
public:
    Parser() = default;
    LYNX_DECLARE_MOVE(Parser);
    LYNX_DELETE_COPY(Parser);
    ~Parser();
    Ast parse(void) const;

private:
    using Stream = std::variant<std::istream*, std::string>;
    bool m_fromStr;
    Stream m_stream;
    Vec<Token> m_tokens;
    Token m_current;

private:
    bool match(TokenKind kind, int index=0);
    void expect(TokenKind kind, int index=0);
    Token token(void);
    void advance(void);
    void skip(void);
    // -*-
    Ast parse_parameters(void);
    Ast parse_destructuring_expr(void);

    Ast parse_nil_literal(void);
    Ast parse_identifier(void);
    Ast parse_bool_literal(void);
    Ast parse_number_literal(void);
    Ast parse_symbol(void);
    Ast parse_string_literal(void);
    Ast parse_tuple_literal(void);
    Ast parse_list_literal(void);
    Ast parse_set_literal(void);
    Ast parse_dict_literal(void);
    Ast parse_result(void);
    Ast parse_fun_expr(void);
    Ast parse_lambda_expr(void);
    Ast parse_macro_expr(void);
    Ast parse_struct_expr(void);
    Ast parse_method_expr(void);
    // -
    Ast parse_get_key_expr(void);
    Ast parse_get_index_expr(void);
    Ast parse_slice_expr(void);
    // -
    Ast parse_ternary_expr(void);
    Ast parse_method_call_expr(void);
    Ast parse_when_expr(void);
    Ast parse_set_index_expr(void);
    Ast parse_set_key_expr(void);
    Ast parse_set_struct_attr_expr(void);
    Ast parse_set_struct_prop_expr(void);
    Ast parse_set_obj_attr_expr(void);
    Ast parse_set_slice_expr(void);
    Ast parse_struct_add_expr(void);
    Ast parse_struct_sub_expr(void);
    Ast parse_struct_div_expr(void);
    Ast parse_struct_mul_expr(void);
    Ast parse_struct_mod_expr(void);
    Ast parse_struct_pow_expr(void);
    Ast parse_struct_bit_or_expr(void);
    Ast parse_struct_bit_xor_expr(void);
    Ast parse_struct_bit_and_expr(void);
    Ast parse_struct_bit_shl_expr(void);
    Ast parse_struct_bit_shr_expr(void);
    Ast parse_struct_bit_or_assign_expr(void);
    Ast parse_struct_bit_xor_assign_expr(void);
    Ast parse_struct_bit_and_assign_expr(void);
    Ast parse_struct_bit_shl_assign_expr(void);
    Ast parse_struct_bit_shr_assign_expr(void);
    Ast parse_struct_add_assign_expr(void);
    Ast parse_struct_sub_assign_expr(void);
    Ast parse_struct_div_assign_expr(void);
    Ast parse_struct_mul_assign_expr(void);
    Ast parse_struct_mod_assign_expr(void);
    Ast parse_struct_pow_assign_expr(void);
    // -
    Ast parse_binary_expr(void);
    Ast parse_funcall_expr(void);
    Ast parse_logical_or_expr(void);
    Ast parse_logical_and_expr(void);
    Ast parse_in_expr(void);
    Ast parse_get_struct_attr_expr(void);
    Ast parse_get_struct_prop_expr(void);
    Ast parse_get_obj_attr_expr(void);
    Ast parse_add_expr(void);
    Ast parse_sub_expr(void);
    Ast parse_div_expr(void);
    Ast parse_mul_expr(void);
    Ast parse_mod_expr(void);
    Ast parse_pow_expr(void);
    Ast parse_add_assign_expr(void);
    Ast parse_sub_assign_expr(void);
    Ast parse_div_assogn_expr(void);
    Ast parse_mul_assign_expr(void);
    Ast parse_mod_assign_expr(void);
    Ast parse_pow_assign_expr(void);
    
    // -
    Ast parse_unary_expr(void);
    Ast parse_bit_not_expr(void);
    Ast parse_logical_not_expr(void);

    // -
    Ast parse_let_stmt(void);
    Ast parse_var_stmt(void);
    Ast parse_for_stmt(void);
    Ast parse_while_stmt(void);
    Ast parse_if_stmt(void);
    Ast parse_break_stmt(void);
    Ast parse_continue_stmt(void);
    Ast parse_return_stmt(void);
    Ast parse_block_stmt(void);
    Ast parse_stmt_list(void);
    Ast parse_expr_stmt(void);
    Ast parse_stmt(void);

    Ast parse_at_doc_expr(void);
    Ast parse_at_define_expr(void);
    Ast parse_at_testing_expr(void);
    Ast parse_at_test_expr(void);
    Ast parse_at_expr(void);
    Ast parse_at_setup_expr(void);
    Ast parse_at_teardown_expr(void);
    Ast parse_at_main_expr(void);
};

// ----------------------------
// -*- Abstract Syntax Tree -*-
// ----------------------------
/**
 * @brief Generic variadic expression or statement AST
 * 
 * [001]
 */
struct BaseAst: public AbstractAST{
public:
    explicit BaseAst(AstKind kind, Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    explicit BaseAst(AstKind kind, Ast& root, Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BaseAst);
    LYNX_DECLARE_MOVE(BaseAst);
    virtual ~BaseAst() = default;
    LYNX_VIRTUAL_OVERLOAD_AST_COMMONS();
    const Vec<Ast>& nodes(void) const{ return this->m_nodes; }

protected:
    Vec<Ast> m_nodes;
};

/**
 * @brief Generic ternary expression AST
 * 
 * [002]
 */
struct TernaryExprAst: public AbstractAST{
public:
    explicit TernaryExprAst(AstKind kind, Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(TernaryExprAst);
    LYNX_DECLARE_MOVE(TernaryExprAst);
    virtual ~TernaryExprAst() = default;
    LYNX_VIRTUAL_OVERLOAD_AST_COMMONS();
    const Ast& first_node(void) const{ return this->m_fst; }
    const Ast& second_node(void) const{ return this->m_snd; }
    const Ast& third_node(void) const{ return this->m_trd; }

protected:
    Ast m_fst;  // First Node
    Ast m_snd;  // Second Node
    Ast m_trd;  // Third Node
};

/**
 * @brief Generic binary expression AST
 * 
 * [003]
 */
struct BinaryExprAst: public AbstractAST{
public:
    explicit BinaryExprAst(AstKind kind, Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BinaryExprAst);
    LYNX_DECLARE_MOVE(BinaryExprAst);
    virtual ~BinaryExprAst() = default;
    LYNX_VIRTUAL_OVERLOAD_AST_COMMONS();
    const Ast& lhs(void) const{ return this->m_lhs; }
    const Ast& rhs(void) const{ return this->m_rhs; }

protected:
    Ast m_lhs;
    Ast m_rhs;
};

/**
 * @brief Generic unary expression AST
 * 
 * [004]
 */
struct UnaryExprAst: public AbstractAST{
public:
    explicit UnaryExprAst(AstKind kind, Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(UnaryExprAst);
    LYNX_DECLARE_MOVE(UnaryExprAst);
    virtual ~UnaryExprAst();
    LYNX_VIRTUAL_OVERLOAD_AST_COMMONS();
    const Ast& rhs(void) const{ return this->m_rhs; }

protected:
    Ast m_rhs;
};

// [005]
struct NullaryExprAst: public AbstractAST{
public:
    explicit NullaryExprAst(AstKind kind, Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(NullaryExprAst);
    LYNX_DECLARE_MOVE(NullaryExprAst);
    virtual ~NullaryExprAst() = default;
    LYNX_VIRTUAL_OVERLOAD_AST_COMMONS();
    const Ast& value(void) const{ return this->m_value; }

protected:
    Ast m_value;
};

/**
 * @brief IdentifierAst
 * Implement Identifier AST
 * 
 * A valid identifier start with '_' or one [a-zA-Z].
 * The rest of identifier name can include one or more of [0-9a-zA-Z_]
 * 
 * [006]
 */
struct IdentifierAst final: public NullaryExprAst{
    explicit IdentifierAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_MOVE(IdentifierAst);
    LYNX_DECLARE_COPY(IdentifierAst);
    ~IdentifierAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief NilLiteralAst
 * Implement Nil literal AST.
 * 
 * Build the parser when 'nil' token is encountered.
 * 
 * [007]
 */
struct NilLiteralAst final: public NullaryExprAst{
    explicit NilLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(NilLiteralAst);
    LYNX_DECLARE_MOVE(NilLiteralAst);
    ~NilLiteralAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief BoolLiteralAst
 * Implement Bool literal AST.
 * 
 * Build the parser when 'true' or 'false' token is encountered.
 * 
 * [008]
 */
struct BoolLiteralAst final: public NullaryExprAst{
    explicit BoolLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BoolLiteralAst);
    LYNX_DECLARE_MOVE(BoolLiteralAst);
    ~BoolLiteralAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief IntegerLiteralAst
 * Implement Integer literal AST
 * 
 * Integer can be written in binary, octal, decimal, or hexadecimal format
 * 
 * Examples
 * --------
 * - Binary Format: 0b1011
 * - Octal Format: 0o01234567
 * - Decimal Format: 2025
 * - Hexadecimal Format: 0x129a0f
 * 
 * [009]
 */
struct IntegerLiteralAst final: public NullaryExprAst{
    explicit IntegerLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(IntegerLiteralAst);
    LYNX_DECLARE_MOVE(IntegerLiteralAst);
    ~IntegerLiteralAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief FloatLiteral
 * Implement Float literal AST
 * 
 * Floating-point value can be written in default format (i.e just with decimal point)
 * or by using scientific notation
 * 
 * Examples
 * --------
 * (1) Default notation: 3.14
 * (2) Scientific notation: 6.2e-23 
 * (3) Scientific notation: 9.1E-31
 * (4) Scientific notation: -1.3e2
 * 
 * [010]
 */
struct FloatLiteralAst final: public NullaryExprAst{
    explicit FloatLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(FloatLiteralAst);
    LYNX_DECLARE_MOVE(FloatLiteralAst);
    ~FloatLiteralAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief ComplexExprAst
 * Implement complex-number constructor AST
 * 
 * Usage:
 *      let z1 = complex(x, y)
 *      let z2 = complex(x, y)
 *      let z = z1 + z2
 *      z = z1 - z2
 *      z = z1 * z2
 *      Z = z1 / z2
 *      let arg = z.arg()
 *      let norm = z.norm()
 *      let abs = z.abs()
 *      let real = z.real()
 *      let imag = z.imag()
 *      let conj = z.conj()
 *      let rho = 3.0
 *      let theta = 30
 *      z = complex.polar(rho, theta)
 */
struct ComplexExprAst final: public UnaryExprAst{
    explicit ComplexExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(ComplexExprAst);
    LYNX_DECLARE_MOVE(ComplexExprAst);
    ~ComplexExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};


/**
 * @brief SymbolLiteral
 * Implement Symbol literal AST
 * 
 * Build a new symbol. A symbol literal can be defined as follow:
 * 
 * let name = :symbol-value
 * 
 * @note: It must start with colon (i.e ':' )
 * 
 * [011]
 */
struct SymbolLiteralAst final: public NullaryExprAst{
    explicit SymbolLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(SymbolLiteralAst);
    LYNX_DECLARE_MOVE(SymbolLiteralAst);
    ~SymbolLiteralAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief StringLiteralAst
 * Implement a string literal AST.
 * 
 * String a written with single quotes or double quotes
 * 
 * Examples:
 * (1) "Hello World"
 * (2) 'Hello World'
 * (3) "Hello \"Lynx\" World"
 * (4) 'Hello \'Lynx\' World'
 * 
 * [012]
 */
struct StringLiteralAst final: public NullaryExprAst{
    explicit StringLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(StringLiteralAst);
    LYNX_DECLARE_MOVE(StringLiteralAst);
    ~StringLiteralAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief TupleLiteralAst
 * Implement Tuple literal AST.
 * 
 * Tuple literal are written as follows:
 *  (1) A tuple with 3 items: (x, y, z) or (x, y, z, )
 *  (2) A tuple with 0 items: (,)
 * 
 * [013]
 */
struct TupleLiteralAst final: public NullaryExprAst{
    explicit TupleLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(TupleLiteralAst);
    LYNX_DECLARE_MOVE(TupleLiteralAst);
    ~TupleLiteralAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief ListLiteralAst
 * Implement List literal AST.
 * 
 * List literal are written as follows:
 * (1) Empty list: []
 * (2) List with 1 item: [x] or [x,]
 * (3) List wiht 3 items: [x, y, z] or [x, y, z,]
 * 
 * [014]
 */
struct ListLiteralAst final: public NullaryExprAst{
    explicit ListLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(ListLiteralAst);
    LYNX_DECLARE_MOVE(ListLiteralAst);
    ~ListLiteralAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief SetLiteralAst
 * Implement Set literal AST.
 * 
 * Set literal are written as follows:
 * (1) Empty Set: #{}
 * (2) Set with 3 items: #{'a', 'b', 'c'}
 * 
 * [015]
 */
struct SetLiteralAst final: public NullaryExprAst{
    explicit SetLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(SetLiteralAst);
    LYNX_DECLARE_MOVE(SetLiteralAst);
    ~SetLiteralAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief DictLiteralAst
 * Implement Dict literal AST
 * 
 * Dict literal are written as follows:
 * (1) Empty Dict: {}
 * (2) Dict with 1 entry: {key1 -> val1} or {key1 -> val1, }
 * (3) Dict with 3 entries: {k1 -> v1, k2 -> v2, k3 -> v3, }
 * 
 * 
 * [016]
 */
struct DictLiteralAst final: public NullaryExprAst{
    explicit DictLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(DictLiteralAst);
    LYNX_DECLARE_MOVE(DictLiteralAst);
    ~DictLiteralAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief ParameterListExprAst
 * 
 * 
 * [017]
 */
struct ParameterListExprAst final: public UnaryExprAst{
    explicit ParameterListExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(ParameterListExprAst);
    LYNX_DECLARE_MOVE(ParameterListExprAst);
    ~ParameterListExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();

    const HashSet<Parameter> params(void) const;
    const Vec<Parameter> params_vec(void) const;

private:
    HashSet<Parameter> m_params;
    Vec<Parameter> m_paramsvec;
};

/**
 * @brief StructDefExprAst
 * Implement a struct definition expression AST
 * 
 * @note: lhs : struct-name i.e IdentifierAst
 * @note: rhs : struct-body i.e BlockStmt
 * 
 * Example:
 * -------
 * struct name{
 *      ; struct doc-string
 *      @doc{...}
 *      ; struct-attributre
 *      @define(cst1, val)  ; define a struct constant
 *      let svname = sval   ; define a struct fixed-type value
 *      var svar = value    ; define a struct variable
 *      ; initializer
 *      @init(x, y, z){
 *          @x = x    // instance variable
 *          @y = y    // instance variable
 *          @z = z    // instance variable
 *      }
 *      ; method
 *      fun foo(a, b){ .... }
 *      ; operator overloading
 *      @operator+(other){ .... }
 *      ; string conversion method
 *      fun __str__(){...}
 *      ; string-repr conversion
 *      fun __repr__(){...}
 *      ; make it a callable
 *      fun __call__(args){...}
 *      ; make it a hashable
 *      fun __hash__(){...}
 *      ...
 * };
 * 
 * [018]
 */
struct StructDefExprAst final: public BinaryExprAst{
    explicit StructDefExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(StructDefExprAst);
    LYNX_DECLARE_MOVE(StructDefExprAst);
    ~StructDefExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief LambdaDefExprAst
 * Implement Lambda Expr AST.
 * 
 * Example of Lambda definition: 
 * lambda(x, y, ...){...}
 * 
 * @note: lhs => ParameterListAst
 * @note: rhs => BlockSmt
 * 
 * [019]
 */
struct LambdaDefExprAst final: public BinaryExprAst{
    explicit LambdaDefExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(LambdaDefExprAst);
    LYNX_DECLARE_MOVE(LambdaDefExprAst);
    ~LambdaDefExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
    Vec<Parameter> parameters() const;
    Ast body(void) const;
    Self operator()(Args args);
};

/**
 * @brief FunDefExprAst
 * Implement function definition expression AST.
 * 
 * - nodes[0]: function-name
 * - nodes[1]: parameters-list
 * if nodes.len() == 3
 *      - documention-string = nullptr;
 *      - nodes[2]: body
 * else if nodes.len() == 4
 *      - nodes[2]: documentation-string
 *      - nodes[3]: body
 * 
 * Examples:
 * ---------
 * (1) function with no parameter
 *      fun fname(){ ... }
 * (2) function with two positional parameters
 *      fun fname(x, y){ ...}
 * (2) function with optional parameters
 *      fun fname(:optional x){ ... }
 *      fun fname(:optional (x, val)){ ... }
 * (3) function with keyword parameters
 *      fun fname(:keys (k, val)){ ... }
 *      fun fname(:keys (k1, v1), (k2, v2)){ ... }
 * (4) variadic funtion
 *      fun fname(:varargs args){ ... }
 * (5) combination of all of the above
 *      fun fname(x, :optional opt, :keys (k, v), :varargs myargs){ ... }
 *      fun fname(x, :keys (k, v), :optional opt, :varargs myargs){ ... }
 * 
 * @note: positional are placed first in function parameters list
 * @note: varargs are always the last in function parameters list
 * 
 * [020]
 */
struct FunDefExprAst final: public BaseAst{
    explicit FunDefExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(FunDefExprAst);
    LYNX_DECLARE_MOVE(FunDefExprAst);
    ~FunDefExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
    Symbol name(void) const;
    Vec<Parameter> parameters() const;
    Ast body(void) const;
    Self operator()(Args args);
    Str docstr(void) const;
};

/**
 * @brief MacroDefExprAst
 * Implement macro definition expression AST.
 * 
 * - nodes[0]: macro-name
 * - nodes[1]: parameters-list
 * if nodes.len() == 3
 *      - documention-string = nullptr;
 *      - nodes[2]: body
 * else if nodes.len() == 4
 *      - nodes[2]: documentation-string
 *      - nodes[3]: body
 * 
 * Examples:
 * ---------
 * (1) macro with two parameters
 *      macro fname(x, y){ ... }
 * 
 * [021]
 */
struct MacroDefExprAst final: public BaseAst {
    explicit MacroDefExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(MacroDefExprAst);
    LYNX_DECLARE_MOVE(MacroDefExprAst);
    ~MacroDefExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
    // -
    Ast expand(void);
    Vec<Parameter> parameters(void) const;
    Ast body(void) const;
    Self operator()(Args args);
    Str docstr(void) const;

private:
    Vec<Parameter> m_params;
};

/**
 * @brief GetExprAst
 * Implement get-{index, key, attr} expression AST
 * 
 * - lhs: {string, dict, tuple, list, struct-obj}-like-object
 * - rhs: {index, key, attr}-identifier-expr
 * 
 * Usage:
 *  - string: string[idx]
 *  - tuple: tuple[idx]
 *  - list: list[idx]
 *  - dict: dict[key]
 *  - struct-property-or-method: obj.prop
 *  - struct-attr: obj::attr or Struct::attr
 *  - struct-obj-attr: Struct:attr
 * 
 * [022]
 */
struct GetExprAst final: public BinaryExprAst {
    explicit GetExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(GetExprAst);
    LYNX_DECLARE_MOVE(GetExprAst);
    ~GetExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
    // -
    Self get(const Self& key);
    Self put(const Self& key, const Self& value);
};

/**
 * @brief SetExprAst
 * Implement get-{index, key, attr} expression AST
 * 
 * - first-node: {dict, tuple, list, struct-obj}-like-object
 * - second-node: {index, key, attr}-identifier-expr
 * - third-node: value
 * 
 * Usage:
 *  - tuple: tuple[idx] = val
 *  - list: list[idx] = val
 *  - dict: dict[key] = val
 *  - struct-prop: obj.prop = val
 *  - struct-attr: obj::attr = val
 *  - obj-attr: obj:attr = val
 * 
 * [023]
 */
struct SetExprAst final: public TernaryExprAst {
    explicit SetExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(SetExprAst);
    LYNX_DECLARE_MOVE(SetExprAst);
    ~SetExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief FCallExprAst
 * Implement function-call expression
 * 
 * lhs: callable-identifier
 * rhs: args-list
 * 
 * Usage:
 *  func(args)
 * 
 * [024]
 */
struct FCallExprAst final: public BinaryExprAst {
    explicit FCallExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(FCallExprAst);
    LYNX_DECLARE_MOVE(FCallExprAst);
    ~FCallExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief MCallExprAst
 * Implement method-call expression AST
 * 
 * first-node: struct-obj | object
 * second-node: method-identifier
 * third-node: args-list
 * 
 * Usage:
 *  obj.method(args)
 * 
 * [025]
 */
struct MCallExprAst final: public TernaryExprAst {
    explicit MCallExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(MCallExprAst);
    LYNX_DECLARE_MOVE(MCallExprAst);
    ~MCallExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief MinusExprAst
 * Implement arithmetic negate-expression AST
 * 
 * rhs: Number-Ast
 * 
 * Usage:
 *      -num
 * 
 * [026]
 */
struct MinusExprAst final: public UnaryExprAst {
    explicit MinusExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(MinusExprAst);
    LYNX_DECLARE_MOVE(MinusExprAst);
    ~MinusExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief PlusExprAst
 * Implement arithmetic-identity expression AST
 * 
 * rhs: number-AST
 * 
 * Usage:
 *      +num
 * 
 * [027]
 */
struct PlusExprAst final: public UnaryExprAst {
    explicit PlusExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(PlusExprAst);
    LYNX_DECLARE_MOVE(PlusExprAst);
    ~PlusExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief AddExprAst
 * Implement '+' binary-operator
 * 
 * Usage:
 *  - number: lhs + rhs             (addition)
 *  - tuple: lhs + rhs              (tuple-concatenation)
 *  - list: lhs + rhs               (tuple-concatenation)
 *  - set: lhs + rhs                (set-union)
 *  - dict: lhs + rhs               (dict-merge)
 *  - struct-object: lhs + rhs      (user-defined)
 * 
 * 
 * [028]
 */
struct AddExprAst final: public BinaryExprAst {
    explicit AddExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(AddExprAst);
    LYNX_DECLARE_MOVE(AddExprAst);
    ~AddExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief SubExprAst
 * Implement '-' binary-operator
 * 
 * Usage:
 *  - number: lhs - rhs         (subtraction)
 *  - set: lhs - rhs            (set-difference)
 * 
 *  or user-defined
 *  
 * [029]
 */
struct SubExprAst final: public BinaryExprAst {
    explicit SubExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(SubExprAst);
    LYNX_DECLARE_MOVE(SubExprAst);
    ~SubExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief DivExprAst
 * Implement '/' binary-operator
 * 
 * lhs: number
 * rhs: number
 * 
 * Usage:
 *      lhs - rhs       (division)
 *  or user-defined
 * 
 * [030]
 */
struct DivExprAst final: public BinaryExprAst {
    explicit DivExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(DivExprAst);
    LYNX_DECLARE_MOVE(DivExprAst);
    ~DivExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief ModExprAst
 * Implement '%' binary-operator
 * 
 * lhs: number
 * rhs: number
 * 
 * Usage:
 *      lhs % rhs           (Modulo)
 *  or user-defined
 * 
 * [031]
 */
struct ModExprAst final: public BinaryExprAst {
    explicit ModExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(ModExprAst);
    LYNX_DECLARE_MOVE(ModExprAst);
    ~ModExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief PowExprAst
 * Implement '**' binary-operator
 * 
 * Usage:
 *      num1 ** num2
 *  or user-defined
 * 
 * [032]
 */
struct PowExprAst final: public BinaryExprAst {
    explicit PowExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(PowExprAst);
    LYNX_DECLARE_MOVE(PowExprAst);
    ~PowExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief AddAssignExprAst
 * Implement '+=' binary-operator
 * 
 * Usage:
 *      lhsNum += rhsNum        (number-inplace-addition)
 *      lhsStr += rhsStr        (string-inplace-concatenation)
 *      lhsSet += rhsSet        (set-inplace-union)
 *      lhsList += rhsList      (list-inplace-concatenation)
 *      lshDict += rhsDict      (dict-inplace-merge)
 *  or user-defined
 * 
 * [033]
 */
struct AddAssignExprAst final: public BinaryExprAst {
    explicit AddAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(AddAssignExprAst);
    LYNX_DECLARE_MOVE(AddAssignExprAst);
    ~AddAssignExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief SubAssignExprAst
 * Implement '-=' binary-operator
 * 
 * Usage:
 *      lhsNum -= rhsNum        (number-inplace-subtraction)
 *      lhsSet -= rhsSet        (set-inplace-difference) 
 *  or user-defined
 * 
 * [034]
 */
struct SubAssignExprAst final: public BinaryExprAst {
    explicit SubAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(SubAssignExprAst);
    LYNX_DECLARE_MOVE(SubAssignExprAst);
    ~SubAssignExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief DivAssignExprAst
 * Implement '/=' binary-operator
 * 
 * Usage:
 *      lhsNum /= rhsNum        (number-inplace-division)
 *  or user-defined
 * 
 * [035]
 */
struct DivAssignExprAst final: public BinaryExprAst {
    explicit DivAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(DivAssignExprAst);
    LYNX_DECLARE_MOVE(DivAssignExprAst);
    ~DivAssignExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief ModAssignExprAst
 * Implement '%=' binary-operator
 * 
 * Usage: 
 *      lhsNum %= rhsNum        (number-inplace-modulo)
 *  or user-defined
 * 
 * [036]
 */
struct ModAssignExprAst final: public BinaryExprAst {
    explicit ModAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(ModAssignExprAst);
    LYNX_DECLARE_MOVE(ModAssignExprAst);
    ~ModAssignExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief PowAssignExprAst
 * Implement '**=' binary-operator
 * 
 * Usage:
 *      lhsNum **= rhsNum
 *  or user-defined
 * 
 * [037] 
 */
struct PowAssignExprAst final: public BinaryExprAst {
    explicit PowAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(PowAssignExprAst);
    LYNX_DECLARE_MOVE(PowAssignExprAst);
    ~PowAssignExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};


/**
 * @brief LogicalOrExprAst
 * Implement 'or' logical binary-operator
 * 
 * Usage:
 *      (lhsBool or rhsBool)
 *  or user-defined
 * 
 * [038]
 */
struct LogicalOrExprAst final: public BinaryExprAst {
    explicit LogicalOrExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(LogicalOrExprAst);
    LYNX_DECLARE_MOVE(LogicalOrExprAst);
    ~LogicalOrExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief LogicalAndExprAst
 * Implement 'and' logical binary-operator
 * 
 * Usage:
 *      (lhsBoolVal and rhsBoolVal)
 * or user-defined
 * 
 * [039]
 */
struct LogicalAndExprAst final: public BinaryExprAst {
    explicit LogicalAndExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(LogicalAndExprAst);
    LYNX_DECLARE_MOVE(LogicalAndExprAst);
    ~LogicalAndExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief LogicalNotExprAst
 * Implement 'not' logical unary-operator
 * 
 * Usage:
 *      (not rhsBoolVal)
 *  or user-defined
 * 
 * [040]
 */
struct LogicalNotExprAst final: public UnaryExprAst {
    explicit LogicalNotExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(LogicalNotExprAst);
    LYNX_DECLARE_MOVE(LogicalNotExprAst);
    ~LogicalNotExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief BitNotExprAst
 * Implement '~' bitwise unary-operator
 * 
 * Usage:
 *      (~lhsNum)
 *  or user-defined
 * 
 * [041]
 */
struct BitNotExprAst final: public UnaryExprAst {
    explicit BitNotExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BitNotExprAst);
    LYNX_DECLARE_MOVE(BitNotExprAst);
    ~BitNotExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief BitOrExprAst
 * Implement '|' bitwise-like binary-operator
 * 
 * Usage:
 *      - Number: (lhsNum | rhsNum)         (integer-bitwise-or)
 *      - Set: (lhsSet | rhsSet)            (set-union)
 *  or user-defined
 * 
 * [042]
 */
struct BitOrExprAst final: public BinaryExprAst {
    explicit BitOrExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BitOrExprAst);
    LYNX_DECLARE_MOVE(BitOrExprAst);
    ~BitOrExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief BitAndExprAst
 * Implement '&' bitwise-like binary-and-operator
 * 
 * Usage:
 *      - Number: (lhsNum & rhsNum)     (integer-bitwise-and)
 *      - Set: (lhsSet & rhsSet)        (set-intersction)
 *  or user-defined
 * 
 * [043]
 */
struct BitAndExprAst final: public BinaryExprAst {
    explicit BitAndExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BitAndExprAst);
    LYNX_DECLARE_MOVE(BitAndExprAst);
    ~BitAndExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief BitXorExprAst
 * Implement '^' bitwise-like-xor binary-operator
 * 
 * Usage:
 *      - Integer: (lhs ^ rhs)      (integer-bitwise-xor)
 *      - Set:  (lhsSet ^ rhsSet)   (set-symmetric-difference)
 *  or user-defined
 * 
 * [044]
 */
struct BitXorExprAst final: public BinaryExprAst {
    explicit BitXorExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BitXorExprAst);
    LYNX_DECLARE_MOVE(BitXorExprAst);
    ~BitXorExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief BitShlExprAst
 * Implement '<<' bitwise-like shift-left binary-operator
 * 
 * Usage:
 *      (lhsInteger << rhsInteger)      (bitwise-shift)
 *  or user-define
 * 
 * @note: 0 <= rhsInteger <= 63
 * 
 * [045]
 */
struct BitShlExprAst final: public BinaryExprAst {
    explicit BitShlExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BitShlExprAst);
    LYNX_DECLARE_MOVE(BitShlExprAst);
    ~BitShlExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief BitShrExprAst
 * Implement '>>' bitwise-like right-shift binary-operator
 * 
 * Usage:
 *      (lhsInteger >> rhsInteger)
 *  or user-defined
 * 
 * @note: 0 <= rhsInteger <= 63
 * 
 * [046]
 */
struct BitShrExprAst final: public BinaryExprAst {
    explicit BitShrExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BitShrExprAst);
    LYNX_DECLARE_MOVE(BitShrExprAst);
    ~BitShrExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief BitOrAssignExprAst
 * Implement '|=' bitwise-like binary-or-operator
 * 
 * Usage:
 *      Integer: lhsInt |= rhsInteger       (integer-inplace-bitwise-or)
 *      Set: lhsSet |= rhsSet               (set-inplace-union)
 * or user-defined
 * 
 * [047]
 */
struct BitOrAssignExprAst final: public BinaryExprAst {
    explicit BitOrAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BitOrAssignExprAst);
    LYNX_DECLARE_MOVE(BitOrAssignExprAst);
    ~BitOrAssignExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief BitAndAssignExprAst
 * Implement '&=' bitwise-like-and inplace binary-operator
 * 
 * Usage:
 *      Integer: lhsInteger &= rhsInteger       (integer-inplace-bitwise-and)
 *      Set: lhsSet &= rhsSet                   (set-inplace-intersection)
 *  or user-defined
 * 
 * [048]
 */
struct BitAndAssignExprAst final: public BinaryExprAst {
    explicit BitAndAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BitAndAssignExprAst);
    LYNX_DECLARE_MOVE(BitAndAssignExprAst);
    ~BitAndAssignExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief BitXorAssignExprAst
 * Implement '^=' bitwise-like-xor inplace binary-operator
 * 
 * Usage:
 *      Integer: lhsInt ^= rhsInt       (integer-inplace-bitwise-xor)
 *      Set: lhsSet ^= rhsSet           (set-inplace-symmetric-difference)
 *  or user-defined
 * 
 * [049]
 */
struct BitXorAssignExprAst final: public BinaryExprAst {
    explicit BitXorAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BitXorAssignExprAst);
    LYNX_DECLARE_MOVE(BitXorAssignExprAst);
    ~BitXorAssignExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief BitShlAssignExprAst
 * Implement '<<=' bitwise-shift-left inplace binary-operator
 * 
 * Usage:
 *      Integer: lhsInt <<= rhsInt      (integer-inplace-shift-left)
 *  or user-defined
 * 
 * [050]
 */
struct BitShlAssignExprAst final: public BinaryExprAst {
    explicit BitShlAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BitShlAssignExprAst);
    LYNX_DECLARE_MOVE(BitShlAssignExprAst);
    ~BitShlAssignExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief BitShrAssignExprAst
 * Implement '>>=' bitwise-shift-right inplace binary-operator
 * 
 * Usage:
 *      Integer: lhsInt >>= rhsInt      (integer-inplace-bitwise-shift-right)
 *  or user-defined
 * 
 * [051]
 */
struct BitShrAssignExprAst final: public BinaryExprAst {
    explicit BitShrAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BitShrAssignExprAst);
    LYNX_DECLARE_MOVE(BitShrAssignExprAst);
    ~BitShrAssignExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief CreateStructObjExprAst
 * Implement struct-obj creation binary-operator
 * 
 * lhs: Name
 * rhs: args
 * 
 * Usage:
 *  (1) defined a struct
 *      struct Name{ ... }      ; env[Name] = std::make_shared<Structure>(Name);
 *  (2) create object
 *      let obj = Name(args)
 * 
 * [052]
 */
struct CreateStructObjExprAst final: public BinaryExprAst {
    explicit CreateStructObjExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(CreateStructObjExprAst);
    LYNX_DECLARE_MOVE(CreateStructObjExprAst);
    ~CreateStructObjExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief AtInitExprAst
 * Implement '@init' initializer
 * 
 * @init function is used to initialize structure instance-attribute
 * 
 * Usage:
 *      ; Create a point
 *      struct Point {
 *          @init(x, y){
 *              @x = x
 *              @y = y
 *          }
 *      }
 *      ; create a point object
 *      let point = Point(1, 2)
 *              ; point = std::make_shared<Structure>("Point");
 *              ; point.initialize(1, 2);
 *      println("x: {point.x}, y: {point.y}")
 * 
 * [054]
 */
struct AtInitExprAst final: public BinaryExprAst {
    explicit AtInitExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(AtInitExprAst);
    LYNX_DECLARE_MOVE(AtInitExprAst);
    ~AtInitExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief AtMainExprAst
 * Implement '@main' main-entry point for a module
 * 
 * Usage:
 *      (1) @main(){ ...}
 *      (2) @main(args){ ... }
 * 
 * [056]
 */
struct AtMainExprAst final: public BinaryExprAst {
    explicit AtMainExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(AtMainExprAst);
    LYNX_DECLARE_MOVE(AtMainExprAst);
    ~AtMainExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief AtTestingExprAst
 * Implement '@testing' unitest-block entry
 * 
 * Usage:
 * ------
 *      @testing Name { ... }
 * 
 * struct UnitTest{
 *  -m_moduleName: Str
 *  -m_testcases: HashMap<Str, TestCase>
 *  -m_env: Module::Env
 *  +assertTrue(val, expected)
 *  +asserrtFalse(val, expected)
 *  +assertEqual(val, expected)
 *  +assertNotEqual(val, expected)
 *  +assertInstanceof(val, expected)
 *  +assertError(val, expected)
 *  +setUp()
 *  +tearDown()
 * };
 * struct TestCase{
 *      +TestCase(Str testName, fnAst);
 *      +Result operator()(val, expected)
 * }
 * 
 * [057]
 */
struct AtTestingExprAst final: public BinaryExprAst {
    explicit AtTestingExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(AtTestingExprAst);
    LYNX_DECLARE_MOVE(AtTestingExprAst);
    ~AtTestingExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief AtTestExprAst
 * Implement '@test' test-case entry
 * 
 * Usage:
 *      @test(name){ body }
 * 
 * [058]
 */
struct AtTestExprAst final: public BinaryExprAst {
    explicit AtTestExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(AtTestExprAst);
    LYNX_DECLARE_MOVE(AtTestExprAst);
    ~AtTestExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief AtSetupExprAst
 * Implement '@setup' UnitTest setup entry
 * 
 * Usage:
 *      @setup{ ... }
 * 
 * [059]
 */
struct AtSetupExprAst final: public UnaryExprAst {
    explicit AtSetupExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(AtSetupExprAst);
    LYNX_DECLARE_MOVE(AtSetupExprAst);
    ~AtSetupExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief AtTeardownExprAst
 * Implement '@teardown' UnitTest tear-down entry
 * 
 * Usage:
 *      @teardown{ ... }
 * 
 * [060]
 */
struct AtTeardownExprAst final: public UnaryExprAst {
    explicit AtTeardownExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(AtTeardownExprAst);
    LYNX_DECLARE_MOVE(AtTeardownExprAst);
    ~AtTeardownExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief AtDefineExprAst
 * Implement '@define' constant definition
 * 
 * @define-expr: {node1, node2, [node3]}
 * 
 * node1: ident
 * node2: value
 * node3: doc-string
 * 
 * Usage:
 *      @define(name, value)
 *      @define(name, value, @doc{...})
 * 
 * [061]
 */
struct AtDefineExprAst final: public TernaryExprAst {
    explicit AtDefineExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(AtDefineExprAst);
    LYNX_DECLARE_MOVE(AtDefineExprAst);
    ~AtDefineExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief AtOperatorExprAst
 * Implement '@operator' for operator-overloading handler-definition
 * 
 * Usage:
 *      @operator __call__(args){ ... }
 *      @operator __str__(){ ... }
 *      @operator __repr__(){ ... }
 *      @operator __hash__(){ ... }
 *      @operator __setattr__(){ ... }
 *      @operator __getattr__(){ ... }
 *      @operator __delattr__(){ ... }
 *      @operator __bool__(){}
 *      @operator __integer__(){ ... }
 *      @operator __float__(){ ... }
 *      @operator __tuple__(){ ... }
 *      @operator __list__(){ ... }
 *      @operator __hashset__(){ ... }
 *      @operator __hashmap__(){ ... }
 *      @operator __next__(){ ... }
 *      @operator __done__(){ ... }
 *      @operator[](index){ ... }
 *      @operator[](index, val){ ... }
 *      @operator[](key){ ... }
 *      @operator[](key, val){ ... }
 *      @operator +(rhs){ ... }
 *      @operator -(rhs){ ... }
 *      @operator /(){ ... }
 *      @operator %(){ ... }
 *      @operator **(){ ... }
 *      @operator |(){ ... }
 *      @operator ^(){ ... }
 *      @operator &(){ ... }
 *      @operator <<(){ ... }
 *      @operator >>(){ ... }
 *      @operator ~(){ ... }
 *      @operator +=(rhs){ ... }
 *      @operator -=(rhs){ ... }
 *      @operator /=(){ ... }
 *      @operator %=(){ ... }
 *      @operator **=(){ ... }
 *      @operator |=(){ ... }
 *      @operator ^=(){ ... }
 *      @operator &=(){ ... }
 *      @operator <<=(){ ... }
 *      @operator >>=(){ ... }
 *      @operator ==(rhs){ ... }
 *      @operator !=(rhs){ ... }
 *      @operator <(rhs){ ... }
 *      @operator <=(rhs){ ... }
 *      @operator >(rhs){ ... }
 *      @operator >=(rhs){ ... }
 * 
 * [062]
 */
struct AtOperatorExprAst final: public TernaryExprAst {
    explicit AtOperatorExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(AtOperatorExprAst);
    LYNX_DECLARE_MOVE(AtOperatorExprAst);
    ~AtOperatorExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief AtDocExprAst
 * Implement '@doc' documentation-string expression
 * 
 * doc-expr: { rhs }
 * rhs: string
 * 
 * Usage:
 *      @doc{ string }
 * 
 * [063]
 */
struct AtDocExprAst final: public UnaryExprAst {
    explicit AtDocExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(AtDocExprAst);
    LYNX_DECLARE_MOVE(AtDocExprAst);
    ~AtDocExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief AtWhenExprAst
 * Implement '@when' ternary-expression operator
 * 
 * when-expr: {node1, node2, node3 }
 * node1: test-node
 * node2: ok-node
 * node3: alt-node
 * 
 * Usage:
 *      let name = @when(test){ok}{alt}
 * 
 * [064]
 */
struct AtWhenExprAst final: public TernaryExprAst {
    explicit AtWhenExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(AtWhenExprAst);
    LYNX_DECLARE_MOVE(AtWhenExprAst);
    ~AtWhenExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief BlockStmtAst
 * Implement a block statement
 * 
 * Usage:
 *      { ... }
 * 
 * [065]
 */
struct BlockStmtAst final: public UnaryExprAst {
    explicit BlockStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BlockStmtAst);
    LYNX_DECLARE_MOVE(BlockStmtAst);
    ~BlockStmtAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief LetStmtAst
 * Implement 'let' reserved-word handler.
 * 
 * 'let' defines fixed-type variable, i.e once defined, the variable can be
 * mutated by assigning only data of the same type.
 * 
 * Usage:
 *      let name = value
 *      let x = 1       ; x is an integer
 *      x = 3
 *      x = 3.14        ; this will result in error
 *      let y = 3.14    ; y is a float
 *      y = 2.72        ; Ok, y is still a float
 *      y = 6.2e-23     ; Ok, y is still a float
 *      y = 42          ; Error, y cannot change it type to integer
 * [066]
 */
struct LetStmtAst final: public BinaryExprAst {
    explicit LetStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(LetStmtAst);
    LYNX_DECLARE_MOVE(LetStmtAst);
    ~LetStmtAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief VarStmtAst
 * Implement 'var' reserved-word handler
 * 
 * 'var' define flexible-type variable. Once defined, the variable may be
 * update with a new value of different type
 * 
 * Usage:
 *      var name = value
 *      var x = 1.2         ; x is float
 *      x = 2               ; Ok: x is now an integer
 *      x = "Hello World"   ; Ok: x is now a string
 *      x = (1, 'Lynx', "Rust", "Python", :Java )   ; Ok: x is now a tuple
 *      x = [:Lynx, :Python, :CPP, :Java ]          ; Ok: x is now a list
 *      x = #{1, 2, 3}                              ; Ok: x is now a HashSet
 *      x = {:lang -> "Lynx", :author -> "Eyram K. Apetcho"}  ; Ok: x is now a HashMap
 * 
 * [067]
 */
struct VarStmtAst final: public BinaryExprAst {
    explicit VarStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(VarStmtAst);
    LYNX_DECLARE_MOVE(VarStmtAst);
    ~VarStmtAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};


/**
 * @brief IfStmtAst
 * Implement 'if' semantic
 * 
 * Usage:
 *      (1) if(test){ ... }
 *      (2) if(test){ ... }else{ ... }
 *      (3) if(test1){ ... }elif(test1){...} ...
 * 
 * [068]
 */
struct IfStmtAst final: public BaseAst {
    explicit IfStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(IfStmtAst);
    LYNX_DECLARE_MOVE(IfStmtAst);
    ~IfStmtAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief ForStmtAst
 * Implement 'for' statement semantic
 * 
 * lhs: tuple(dummyVar, iterable)
 * rhs: blockStmt
 * 
 * Usage:
 * ------
 *      for(x: iterable){ ... }
 * 
 * [069]
 */
struct ForStmtAst final: public BinaryExprAst {
    explicit ForStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(ForStmtAst);
    LYNX_DECLARE_MOVE(ForStmtAst);
    ~ForStmtAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief WhileStmtAst
 * Implement 'while' statement semantic
 * 
 * lhs: boolVal
 * rhs: blockStmt
 * 
 * Usage:
 * ------
 *      while(test){ ... }
 * 
 * [070]
 */
struct WhileStmtAst final: public BinaryExprAst {
    explicit WhileStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(WhileStmtAst);
    LYNX_DECLARE_MOVE(WhileStmtAst);
    ~WhileStmtAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};


/**
 * @brief BreakStmtAst
 * Implement 'break' statement semantic
 * 
 * Usage:
 * ------
 *          for(x: iterable){ .... break ....}
 * 
 * [071]
 */
struct BreakStmtAst final: public NullaryExprAst {
    explicit BreakStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(BreakStmtAst);
    LYNX_DECLARE_MOVE(BreakStmtAst);
    ~BreakStmtAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief ContinueStmtAst
 * Implement 'continue' statement semantic
 * 
 * Usage:
 * -----
 *      for(x: iterable){ ... continue ... }
 * 
 * [072]
 */
struct ContinueStmtAst final: public NullaryExprAst {
    explicit ContinueStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(ContinueStmtAst);
    LYNX_DECLARE_MOVE(ContinueStmtAst);
    ~ContinueStmtAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief ReturnStmtAst
 * Implement 'return' statement semantic
 * 
 * Usage:
 * ------
 *          { ... return ... }
 *          { ... return val ... }
 * 
 * [073]
 */
struct ReturnStmtAst final: public UnaryExprAst {
    explicit ReturnStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(ReturnStmtAst);
    LYNX_DECLARE_MOVE(ReturnStmtAst);
    ~ReturnStmtAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief ImportStmtAst
 * Implement 'import' statement semantic
 * 
 * Usage:
 *      import module
 *      import { sym1, sym2, sym3 } from module
 * 
 * [074]
 */
struct ImportStmtAst final: public BaseAst {
    explicit ImportStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(ImportStmtAst);
    LYNX_DECLARE_MOVE(ImportStmtAst);
    ~ImportStmtAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief DestructuringExprAst
 * Implement 'DestructuringExpr' handler
 * 
 * @note: Destructuring is at the moment implement for tuple and list
 * @note: Should we implement it for HashMap, and HashSet
 * 
 * Usage:
 * ------
 *      (1) let (x, y) = (1, 2)         ; x=1, y=2
 *      (2) let (x, y) = (1, 2, 3)      ; x=1, y=(2, 3)
 *      (3) let [x, y] = [1, 2]         ; x=1, y=2
 *      (4) let [x, y] = [1, 2, 3]      ; x=1, y=[2, 4]
 * 
 * [075]
 */
struct DestructuringExprAst final: public BinaryExprAst {
    explicit DestructuringExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(DestructuringExprAst);
    LYNX_DECLARE_MOVE(DestructuringExprAst);
    ~DestructuringExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief ResultExprAst
 * Implement "Result" constructor
 * 
 * Usage:
 *      let ok = Ok(arg)
 *      let err = Err(arg)
 *      let myErrSymbol = :MyError
 *      err = Err(:MyError, message)
 * 
 * [076]
 */
struct ResultExprAst final: public UnaryExprAst {
    explicit ResultExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(ResultExprAst);
    LYNX_DECLARE_MOVE(ResultExprAst);
    ~ResultExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief LessExprAst
 * Implement '<' binary-operator
 * 
 * Usage:
 *      (1) Bool: lhsBoolVal < rhsBoolVal
 *      (2) Number: lhsNum < rhsNum
 *      (3) String: lhsStr < rhsStr
 * 
 * [077]
 */
struct LessExprAst final: public BinaryExprAst {
    explicit LessExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(LessExprAst);
    LYNX_DECLARE_MOVE(LessExprAst);
    ~LessExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief LessEqExprAst
 * Implement '<=' binary-operator
 * 
 * Usage:
 *      (1) Bool: lhsBoolVal <= rhsBoolVal
 *      (2) Number: lhsNum <= rhsNum
 *      (3) String: lhsStr <= rhsStr
 * 
 * [078]
 */
struct LessEqExprAst final: public BinaryExprAst {
    explicit LessEqExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(LessEqExprAst);
    LYNX_DECLARE_MOVE(LessEqExprAst);
    ~LessEqExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief GreaterExprAst
 * Implement '>' binary-operator
 * 
 * Usage:
 *      (1) Bool: lhsBoolVal > rhsBoolVal
 *      (2) Number: lhsNum > rhsNum
 *      (3) String: lhsStr > rhsStr
 * 
 * [079]
 */
struct GreaterExprAst final: public BinaryExprAst {
    explicit GreaterExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(GreaterExprAst);
    LYNX_DECLARE_MOVE(GreaterExprAst);
    ~GreaterExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief GreaterEqExprAst
 * Implement '>=' binary-operator
 * 
 * Usage:
 *      (1) Bool: lhsBoolVal >= rhsBoolVal
 *      (2) Number: lhsNum >= rhsNum
 *      (3) String: lhsStr >= rhsStr
 * 
 * [080]
 */
struct GreaterEqExprAst final: public BinaryExprAst {
    explicit GreaterEqExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(GreaterEqExprAst);
    LYNX_DECLARE_MOVE(GreaterEqExprAst);
    ~GreaterEqExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief EqualExprAst
 * Implement '==' binary-operator
 * 
 * Usage:
 *      (1) Bool: lhsBoolVal == rhsBoolVal
 *      (2) Number: lhsNum == rhsNum
 *      (3) String: lhsStr == rhsStr
 *      (4) Symbol: lhsSym == rhsSym
 * 
 * [081]
 */
struct EqualExprAst final: public BinaryExprAst {
    explicit EqualExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(EqualExprAst);
    LYNX_DECLARE_MOVE(EqualExprAst);
    ~EqualExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief EqualExprAst
 * Implement '!=' binary-operator
 * 
 * Usage:
 *      (1) Bool: lhsBoolVal != rhsBoolVal
 *      (2) Number: lhsNum != rhsNum
 *      (3) String: lhsStr != rhsStr
 *      (4) Symbol: lhsSym != rhsSym
 * 
 * [082]
 */
struct NotEqualExprAst final: public BinaryExprAst {
    explicit NotEqualExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(NotEqualExprAst);
    LYNX_DECLARE_MOVE(NotEqualExprAst);
    ~NotEqualExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief MatchExprAst
 * Implement match-expression semantic AST
 * lhs: match-args
 * rhs: with-cases
 * 
 * Usage:
 * -----
 *      match(val){
 *          with(xval){ ... }
 *          with(complex(x, y)){ ... }
 *          with(structName(x, _, z)){ ... }
 *          with((x, y, _)){ ... }
 *          with([x, _, z]){ ...}
 *          with(#{x, _, z}){ ... }
 *          with({xk -> xv, _, zk -> zv}){ ... }
 *          with( x | y | z){ ... }
 *          with( _ ){ ...}
 *      }
 * 
 * [083]
 */
struct MatchExprAst final: public BinaryExprAst {
    explicit MatchExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(MatchExprAst);
    LYNX_DECLARE_MOVE(MatchExprAst);
    ~MatchExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief WithExprAst
 * Implement with-expression semantic AST
 * 
 * with-case := with(pattern){body}
 * lhs: with-pattern
 * rhs: with-body
 * 
 * Usage:
 *      (1) scalars | string
 *          with(value){ .... }
 *      (2) complex-number
 *          with(complex(x, y)){ ... }
 *      (3) tuple
 *          with((x, y, _)){ ... }
 *      (4) list
 *          with([x, y, _]){ ... }
 *      (5) hashset
 *          with(#{x, y, _}){ ... }
 *      (6) hashmap
 *          with({xk -> xv, yk -> yv, _}){ ... }
 *          with({k1, k2, _}){ ... }
 *      (7) Structure
 *          with(structName(field1, field2, _)){ ... }
 *      (8) Result
 *          with(Ok(value)){ ... }
 *          with(Err(error)){ ... }
 *          with(Error(Symbol, message)){ ... }
 *          with(Error(Symbol, _)){ ... }
 *      (9) WithOr-expression
 *          with(x | y | z){ ... }
 * 
 * [084]
 */
struct WithExprAst final: public BinaryExprAst {
    explicit WithExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(WithExprAst);
    LYNX_DECLARE_MOVE(WithExprAst);
    ~WithExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief AtArgvExprAst
 * Implement '@argv'-expression semantic
 * 
 * '@argv' is a builtin symbol used for collection command-line arguments
 * 
 * Usage:
 *      let argv = @argv
 *      println(typeof(argv))       ; Tuple
 * 
 * [085]
 */
struct AtArgvExprAst final: public NullaryExprAst {
    explicit AtArgvExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(AtArgvExprAst);
    LYNX_DECLARE_MOVE(AtArgvExprAst);
    ~AtArgvExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};

/**
 * @brief AtVersionExprAst
 * Implement '@version'-expression semantic AST
 * 
 * rhs: ([major], [minor], [patch])
 * 
 * Usage:
 *      let myversion = @version            ; current interpreter version
 *      let version1 = @version(2, 3, 0)
 *      let version2 = @version(2, _, 0)
 *      version1 == version2                ; false
 *      version1 >= version2                ; true
 *      version1 > version2                 ; true
 *      version1 == "2.3.0"                 ; true
 *      println(version1)                   ; "2.3.0"
 *      println(version2)                   ; "2.*.0"
 * 
 *      if myversion == @version(3){
 *           import mymodule
 *      }else{
 *           import MymoduleNewName
 *      }
 * 
 */
struct AtVersionExprAst final: public UnaryExprAst {
    explicit AtVersionExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(AtVersionExprAst);
    LYNX_DECLARE_MOVE(AtVersionExprAst);
    ~AtVersionExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};


// ------------------------
// -*- Lynx Interpreter -*-
// ------------------------
class Lynx final{
public:
    class Frame final{
    public:
        explicit Frame() noexcept;
        explicit Frame(Frame* parent);
        LYNX_DECLARE_MOVE(Frame);
        ~Frame();
        Frame& operator=(const Frame& frame);
        Self& operator[](const Str& key);
        const Self& operator[](const Str& key) const;
        Self update(const Str& key, Self value);
        bool contains(const Str& key) const;
    private:
        HashMap<Str, Self> m_bindings;
        Frame* m_parent;
    };

    // -*-
    class TestCase{
    public:
        explicit TestCase(const Str& name, Self testAst, bool skip=false);
        LYNX_DECLARE_MOVE(TestCase);
        ~TestCase();
        TestCase& operator=(const TestCase& testcase);
        // +operator()() -> Result
        Self operator()();

        Str name(void) const;
    private:
        Str m_name;
        Self m_testAst;
        bool m_skip;
    };

    // -*-
    class UnitTest{
    public:
        explicit UnitTest() noexcept;
        explicit UnitTest(const Str& moduleName) noexcept;
        explicit UnitTest(const Str& unitTestName, const Str& moduleName) noexcept;
        LYNX_DECLARE_MOVE(UnitTest);
        ~UnitTest();
        UnitTest& operator=(const UnitTest& unittest);

        // -*-
        void run(void);
        void add_testcase(const TestCase& testcase);
        Str name(void) const;
        Str module(void) const;
        // -*-
        static Self setup(Args args);
        static Self teardown(Args args);
        static Self assertTrue(Args args);
        static Self assertFalse(Args args);
        static Self assertEqual(Args args);
        static Self assertNotEqual(Args args);
        static Self assertLess(Args args);
        static Self assertLessEqual(Args args);
        static Self assertGreater(Args args);
        static Self assertGreaterEqual(Args args);
        static Self assertTypeof(Args args);
        static Self assertError(Args args);
        static Self assertErrorType(Args args);

    private:
        Str m_name;
        Str m_module;
        HashMap<Str, Self> m_setup;
        Vec<TestCase> m_testcases;
    };
    

    struct Version{
        // major: u32
        i32 major;
        i32 minor;
        i32 patch;
        Version();
        Version(i32 maj, i32 min=-1, i32 patch=-1);
        Version& operator=(const Version& version);
        ~Version() = default;
        operator Str();
        operator Object();
        friend bool operator==(const Version& lhs, const Version& rhs);
        friend bool operator!=(const Version& lhs, const Version& rhs);
        friend bool operator>(const Version& lhs, const Version& rhs);
        friend bool operator>=(const Version& lhs, const Version& rhs);

        friend bool operator==(const Version& lhs, const Str& rhs);
        friend bool operator!=(const Version& lhs, const Str& rhs);
        friend bool operator>(const Version& lhs, const Str& rhs);
        friend bool operator>=(const Version& lhs, const Str& rhs);

        friend bool operator==(const Str& lhs, const Version& rhs);
        friend bool operator!=(const Str& lhs, const Version& rhs);
        friend bool operator>(const Str& lhs, const Version& rhs);
        friend bool operator>=(const Str& lhs, const Version& rhs);

        friend bool operator==(const Version& lhs, const i32& rhs);
        friend bool operator!=(const Version& lhs, const i32& rhs);
        friend bool operator>(const Version& lhs, const i32& rhs);
        friend bool operator>=(const Version& lhs, const i32& rhs);

        friend bool operator==(const i32& lhs, const Version& rhs);
        friend bool operator!=(const i32& lhs, const Version& rhs);
        friend bool operator>(const i32& lhs, const Version& rhs);
        friend bool operator>=(const i32& lhs, const Version& rhs);
    };

    // -*-
    class Module final{
    public:
        explicit Module() noexcept;
        explicit Module(const Str& name, const fs::path& path) noexcept;
        LYNX_DECLARE_MOVE(Module);
        ~Module();
        Lynx::Frame& frame(void);
        const Lynx::Frame& frame(void) const;

        Version& version();
        const Version& version() const;

    private:
        Symbol m_name;
        fs::path m_path;
        Lynx::Frame m_frame;
        Version m_version;
    };

    using CallBackDict = HashMap<Str, Callaback>;
    
    static CallBackDict boolMethods;
    static CallBackDict integerMethods;
    static CallBackDict floatMethods;
    static CallBackDict complexMethods;
    static CallBackDict symbolMethods;
    static CallBackDict stringMethods;
    static CallBackDict tupleMethods;
    static CallBackDict listMethods;
    static CallBackDict hashsetMethods;
    static CallBackDict hashmapMethods;
    static CallBackDict resultMethods;
    static CallBackDict structMethods;

    static HashMap<Str, CFun> builtinFunctions;
    static HashMap<Str, Self> builtinConstants;
    static HashMap<Str, Symbol> builtinSymbols;


public:
    explicit Lynx() noexcept;
    LYNX_DECLARE_MOVE(Lynx);
    ~Lynx();
    Str banner(void) const;
    void run(const Str& filename, Vec<Str> argv);
    void repl(Vec<Str> argv);

    Self operator()(IdentifierAst);
    Self operator()(NilLiteralAst);
    Self operator()(BoolLiteralAst);
    Self operator()(IntegerLiteralAst);
    Self operator()(FloatLiteralAst);
    Self operator()(SymbolLiteralAst);
    Self operator()(StringLiteralAst);
    Self operator()(TupleLiteralAst);
    Self operator()(ListLiteralAst);
    Self operator()(SetLiteralAst);
    Self operator()(DictLiteralAst);
    Self operator()(ParameterListExprAst);
    Self operator()(StructDefExprAst);
    Self operator()(LambdaDefExprAst);
    Self operator()(FunDefExprAst);
    Self operator()(MacroDefExprAst);
    Self operator()(GetExprAst);
    Self operator()(SetExprAst);
    Self operator()(FCallExprAst);
    Self operator()(MCallExprAst);
    Self operator()(MinusExprAst);
    Self operator()(PlusExprAst);
    Self operator()(AddExprAst);
    Self operator()(SubExprAst);
    Self operator()(DivExprAst);
    Self operator()(ModExprAst);
    Self operator()(PowExprAst);
    Self operator()(AddAssignExprAst);
    Self operator()(SubAssignExprAst);
    Self operator()(DivAssignExprAst);
    Self operator()(ModAssignExprAst);
    Self operator()(PowAssignExprAst);
    Self operator()(LogicalOrExprAst);
    Self operator()(LogicalAndExprAst);
    Self operator()(LogicalNotExprAst);
    Self operator()(BitNotExprAst);
    Self operator()(BitOrExprAst);
    Self operator()(BitAndExprAst);
    Self operator()(BitXorExprAst);
    Self operator()(BitShlExprAst);
    Self operator()(BitShrExprAst);
    Self operator()(BitOrAssignExprAst);
    Self operator()(BitAndAssignExprAst);
    Self operator()(BitXorAssignExprAst);
    Self operator()(BitShlAssignExprAst);
    Self operator()(BitShrAssignExprAst);
    Self operator()(CreateStructObjExprAst);
    Self operator()(AtInitExprAst);
    Self operator()(AtMainExprAst);
    Self operator()(AtTestingExprAst);
    Self operator()(AtTestExprAst);
    Self operator()(AtSetupExprAst);
    Self operator()(AtTeardownExprAst);
    Self operator()(AtDefineExprAst);
    Self operator()(AtOperatorExprAst);
    Self operator()(AtDocExprAst);
    Self operator()(WhenExprAst);
    Self operator()(BlockStmtAst);
    Self operator()(LetStmtAst);
    Self operator()(VarStmtAst);
    Self operator()(IfStmtAst);
    Self operator()(ForStmtAst);
    Self operator()(WhileStmtAst);
    Self operator()(BreakStmtAst);
    Self operator()(ContinueStmtAst);
    Self operator()(ReturnStmtAst);
    Self operator()(ImportStmtAst);
    Self operator()(DestructuringExprAst);
    Self operator()(ResultExprAst);
    Self operator()(LessExprAst);
    Self operator()(LessEqExprAst);
    Self operator()(GreaterExprAst);
    Self operator()(GreaterEqExprAst);
    Self operator()(EqualExprAst);
    Self operator()(NotEqualExprAst);
    Self operator()(MatchExprAst);
    Self operator()(WithExprAst);
    Self operator()(AtArgvExprAst);
    Self operator()(AtVersionExprAst);

private:
    Str m_license;
    Lynx::Version m_version;
    Str m_authors;
    Str m_ps1;
    Str m_ps2;
    HashMap<Str, Module> m_lynxmodules;
    HashMap<Str, Module> m_sites;
    HashMap<Str, fs::path> m_paths;

    Frame m_runtime;

    static void initialize(void);
};


// -*----------------------------------------------------------------*-
}//-*- end::namespace::lynx                                         -*-
// -*----------------------------------------------------------------*-

#endif