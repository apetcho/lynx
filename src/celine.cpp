#include<filesystem>
#include<algorithm>
#include<stdexcept>
#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include<variant>
#include<complex>
#include<cstdint>
#include<memory>
#include<cmath>
#include<vector>
#include<string>
#include<stack>
#include<any>
#include<map>


// -*----------------------------------------------------------------*-
// -*- begin::namespace::cln                                        -*-
// -*----------------------------------------------------------------*-
namespace cln{
// -
class Object;
class Symbol;
class Callable;
class List;

using usize = std::size_t;
using Str = std::string;
using f64 = double;
using i64 = std::int64_t;
using i32 = std::int32_t;
using Complex = std::complex<f64>;
using Self = std::shared_ptr<Object>;
template<typename T>
using Vec = std::vector<T>;
template<typename K, typename V>
using HashMap = std::map<K, V>;
using Args = Vec<Self>;
using CFun = Self (*)(Args);

// -
class Error final{
public:
    enum Kind{TypeError, ValueError, SyntaxError, RuntimeError};
    // -
    explicit Error() noexcept = default;
    explicit Error(const Str& msg) noexcept;
    explicit Error(Kind kind, const Str& msg) noexcept;
    Error(const Error&) noexcept = default;
    Error(Error&&) noexcept = default;
    Error& operator=(const Error&) noexcept = default;
    Error& operator=(Error&&) noexcept = default;
    ~Error() = default;

    // -
    Str describe(void) const;

private:
    Kind m_kind{};
    Str m_prefix{"Error: "};
    Str m_message;
};

class Env final{
private:
    HashMap<Str, Self> m_bindings;
    Env* m_parent; 

public:
    explicit Env() noexcept = default;
    explicit Env(Env* parent) noexcept;
    Env(const Env& env) noexcept;
    Env& operator=(const Env& env) noexcept;
    ~Env() = default;

    void put(Str key, Self self);
    void update(Str key, Self self);
    Self get(Str key) const;
    bool contains(Str key) const;
};

// -
class Object{
public:
    explicit Object() noexcept = default;
    Object(const Object&) noexcept = default;
    Object(Object&&) noexcept = default;
    Object& operator=(const Object&) = default;
    Object& operator=(Object&&) = default;
    virtual ~Object() = default;
    virtual Str str(void) const;
    virtual Str repr(void) const;
    virtual Symbol type(void) const;

    const bool& fixed(void) const { return this->m_fixed; }
    bool& fixed(void){ return this->m_fixed; }
    bool is_nil(void) const;

protected:
    bool m_fixed{false}; // in (define name val [doc]) => obj.fixed() = true;
    Str m_doc{};
};

class Nil final: public Object{
public:
    explicit Nil() noexcept = default;
    Nil(const Nil& ) noexcept = default;
    Nil(Nil&&) noexcept = default;
    Nil& operator=(const Nil&) noexcept = default;
    Nil& operator=(Nil&& ) noexcept = default;
    ~Nil() = default;
    Str str(void) const override;
    Str repr(void) const override;
    Symbol type(void) const override;
};

class Bool final: public Object{
private:
    bool m_val;

public:
    explicit Bool() noexcept;
    explicit Bool(bool val) noexcept;
    Bool(const Bool& other) noexcept;
    Bool(Bool&& bther) noexcept;
    Bool& operator=(const Bool& other) noexcept;
    Bool& operator=(Bool&& other) noexcept;
    ~Bool() = default;
    Str str(void) const override;
    Str repr(void) const override;
    Symbol type(void) const override;
    bool as_bool(void) const;

    Bool& operator!();
    friend Bool operator&&(const Bool& lhs, const Bool& rhs);
    friend Bool operator||(const Bool& lhs, const Bool& rhs);
};

class Number final: public Object{
public:
    explicit Number() noexcept;
    explicit Number(i64 num) noexcept;
    explicit Number(f64 num) noexcept;
    explicit Number(f64 x, f64 y) noexcept;
    explicit Number(Complex z) noexcept;
    Number(const Number& other) noexcept;
    Number(Number&& other) noexcept;
    Number& operator=(const Number& other) noexcept;
    Number& operator=(Number&& other) noexcept;
    ~Number() = default;
    Str str(void) const override;
    Str repr(void) const override;
    Symbol type(void) const override;

    i64 as_integer(void) const;
    f64 as_float(void) const;
    bool is_integer(void) const;
    bool is_float(void) const;
    bool is_complex(void) const;

    Complex as_complex(void) const;
    static Number polar(f64 rhor, f64 theta);
    Number real(void) const;
    Number imag(void) const;
    Number conj(void) const;
    Number arg(void) const;
    Number abs(void) const;

    friend Number operator+(const Number& lhs, const Number& rhs);
    friend Number operator-(const Number& lhs, const Number& rhs);
    friend Number operator*(const Number& lhs, const Number& rhs);
    friend Number operator/(const Number& lhs, const Number& rhs);
    friend Number operator%(const Number& lhs, const Number& rhs);
    friend Number operator==(const Number& lhs, const Number& rhs);
    friend Number operator!=(const Number& lhs, const Number& rhs);
    friend Number operator<=(const Number& lhs, const Number& rhs);
    friend Number operator>=(const Number& lhs, const Number& rhs);
    friend Number operator<(const Number& lhs, const Number& rhs);
    friend Number operator>(const Number& lhs, const Number& rhs);

    friend Number operator<<(const Number& lhs, const Number& rhs);
    friend Number operator>>(const Number& lhs, const Number& rhs);
    friend Number operator|(const Number& lhs, const Number& rhs);
    friend Number operator&(const Number& lhs, const Number& rhs);
    Number& operator~(void);

    Number exp(void) const;
    Number log(void) const;
    Number log10(void) const;
    Number sqrt(void) const;
    Number pow(void) const;
    Number sin(void) const;
    Number cos(void) const;
    Number tan(void) const;
    Number asin(void) const;
    Number acos(void) const;
    Number atan(void) const;
    Number sinh(void) const;
    Number cosh(void) const;
    Number tanh(void) const;
    Number asinh(void) const;
    Number acosh(void) const;
    Number atanh(void) const;
    
    Number cbrt(void) const;
    Number hypot(void) const;
    Number erf(void) const;
    Number erfc(void) const;
    Number tgamma(void) const;
    Number ceil(void) const;
    Number floor(void) const;
    Number round(void) const;

    Bool isnan(void) const;
    Bool isinf(void) const;
    Bool isfinite(void) const;

    static Number NaN;
    static Number PI;
    static Number E;
    static Number I64_MAX;
    static Number I64_MIN;
    static Number F64_MIN;
    static Number F64_MAX;
    static Number EPSILON;


private:
    enum Kind{INT, FLOAT, COMPLEX};
    std::any m_val;
};


class Symbol final: public Object{
public:
    explicit Symbol(const Str& val) noexcept;
    Symbol(const Symbol& other) noexcept;
    Symbol(Symbol&& other) noexcept;
    Symbol& operator=(const Symbol& other) noexcept;
    Symbol& operator=(Symbol&& other) noexcept;
    ~Symbol() = default;
    Str str(void) const override;
    Str repr(void) const override;
    Symbol type(void) const override;
    
    friend Bool operator==(const Symbol& lhs, const Symbol& rhs);
    friend Bool operator!=(const Symbol& lhs, const Symbol& rhs);

private:
    Str m_val;
};

// -
class String final: public Object{
private:
    Str m_val;

public:
    explicit String() noexcept;
    explicit String(const Str& val) noexcept;
    String(const String& other) noexcept;
    String(String&& other) noexcept;
    String& operator=(const String& other) noexcept;
    String& operator=(String&& other) noexcept;
    ~String() = default;
    Str str(void) const override;
    Str repr(void) const override;
    Symbol type(void) const override;
    Str as_str(void) const;

    friend Bool operator==(const String& lhs, const String& rhs);
    friend Bool operator!=(const String& lhs, const String& rhs);
    friend Bool operator<=(const String& lhs, const String& rhs);
    friend Bool operator>=(const String& lhs, const String& rhs);
    friend Bool operator<(const String& lhs, const String& rhs);
    friend Bool operator>(const String& lhs, const String& rhs);

    friend String operator+(const String& lhs, const String& rhs);

    Number len(void) const;
    Bool startswith(const String& prefix) const;
    Bool endswith(const String& suffix) const;
    String join(List& xs) const;
    String replace(const Str& old, const Str& neo, bool all=false);
    String ltrim(void);
    String rtrim(void);
    String trim(void);
    Number find(const Str& pattern);
    String substr(Number first, Number last);
    String upper(void);
    String lower(void);
    List split(const String sep=String(" "));
    String title(void);
    String capitalize(void);
    Bool is_numeric(void);
    Bool is_space(void);
    Bool is_lower(void);
    Bool is_upper(void);
};

// -
class List final: public Object{
public:
    explicit List() noexcept = default;
    explicit List(Vec<Self> vec) noexcept;
    List(const List& other) noexcept;
    List(List&& other) noexcept;
    List& operator=(const List& other);
    List& operator=(List&& other);
    ~List() = default;
    Str str(void) const override;
    Str repr(void) const override;
    Symbol type(void) const override;

    Vec<Self> as_vec(void) const;

    friend List operator+(const List& lhs, const List& rhs);
    friend List operator==(const List& lhs, const List& rhs);
    friend List operator!=(const List& lhs, const List& rhs);

    Number len(void) const;
    Self head(void) const;
    Self last(void) const;
    List tail(void) const;
    List push(Self self) const;
    Self pop(void);
    List append(Self self) const;
    List slice(Number first, Number last);
    Number find(const Self& self);
    List insert(Number index, Self val);
    List remove(Number index, Self val);
    List sort(Callable* keyfn);
    List sort(Symbol key=Symbol(">"));

private:
    Vec<Self> m_val;
};

// -*-
class Callable: public Object{
public:
    explicit Callable() noexcept: Object{}{}
    virtual ~Callable() = default;

    Self operator()(Args args){
        return this->call(args);
    }

protected:
    virtual Self call(Args args) = 0;
};

class Lambda final: public Callable{
public:
    explicit Lambda(List& params, Self body, Env& env) noexcept;
    Lambda(const Lambda& lambda) noexcept;
    Lambda(Lambda&& lambda) noexcept;
    Lambda& operator=(const Lambda& lambda) noexcept;
    Lambda& operator=(Lambda&& lambda) noexcept; 
    ~Lambda();

    Str str(void) const override;
    Str repr(void) const override;
    Symbol type(void) const override;
    const List& params(void) const;
    const Self& body(void) const;
    const Env& env(void) const;

private:
    List m_params;
    Self m_body;
    Env m_env;

    Self call(Args args) override;
};

class Builtin final: public Callable {
public:
    explicit Builtin(CFun cfun) noexcept;
    ~Builtin() = default;

    Str str(void) const override ;
    Str repr(void) const override;
    Symbol type(void) const override;
    const CFun& cfun(void) const;

private:
    CFun m_cfun;

    Self call(Args args) override;
};


class Function final: public Callable {
public:
    explicit Function(List& params, Self body, const Env& env) noexcept;
    ~Function() = default;

    Str str(void) const override;
    Str repr(void) const override;
    Symbol type(void) const override;
    const List& params(void) const;
    const Self& body(void) const;
    const Env& env(void) const;

private:
    List m_params;
    Self m_body;
    Env m_env;

    Self call(Args args) override;
};


class Macro final: public Callable{
public:
    explicit Macro(List& params, Self body, const Env& env) noexcept;
    ~Macro() = default;

    Str str(void) const override;
    Str repr(void) const override;
    Symbol type(void) const override;
    const List& params(void) const;
    const Self& body(void) const;
    const Env& env(void) const;
    List expand(void); // (progn ...)

private:
    List m_params;
    Self m_body;
    Env m_env;

    Self call(Args args) override;
};

//! @todo
// class Stream final: public Object{};
// class Path final: public Object{};
// class File final: public Object{};

// -----------------------------
// -*- IMPLEMENTATION OF ENV -*-
// ----------------------------
/*
Env::nv(Env* parent) noexcept{}
Env::Env(const Env& env) noexcept{}
Env::Env& operator=(const Env& env) noexcept{}
void Env::put(Str key, Self self){}
void Env::update(Str key, Self self){}
Self Env::get(Str key) const{}
bool Env::contains(Str key) const{}
*/

// ---------------------------
// -*- IMPLEMENTATION BOOL -*-
// ---------------------------
/*
Bool::Bool() noexcept{}
Bool::Bool(bool val) noexcept{}
Bool::Bool(const Bool& other) noexcept{}
Bool::Bool(Bool&& bther) noexcept{}
Bool& Bool::operator=(const Bool& other) noexcept{}
Bool& Bool::operator=(Bool&& other) noexcept{}
Str Bool::str(void) const{}
Str Bool::repr(void) const{}
Symbol Bool::type(void) const{}
bool Bool::as_bool(void) const{}
Bool& Bool::operator!(){}
Bool operator&&(const Bool& lhs, const Bool& rhs){}
Bool operator||(const Bool& lhs, const Bool& rhs){}
*/

// --------------------------------
// -*- IMPLEMENTATION OF NUMBER -*-
// --------------------------------
/*
Number::Number() noexcept{}
Number::Number(i64 num) noexcept{}
Number::Number(f64 num) noexcept{}
Number::Number(f64 x, f64 y) noexcept{}
Number::Number(Complex z) noexcept{}
Number::Number(const Number& other) noexcept{}
Number::Number(Number&& other) noexcept{}
Number& Number::operator=(const Number& other) noexcept{}
Number& Number::operator=(Number&& other) noexcept{}
Str Number::str(void) const override{}
Str Number::repr(void) const override{}
Symbol Number::type(void) const override{}

i64 Number::as_integer(void) const{}
f64 Number::as_float(void) const{}
bool Number::is_integer(void) const{}
bool Number::is_float(void) const{}
bool Number::is_complex(void) const{}

Complex Number::as_complex(void) const{}
Number Number::polar(f64 rhor, f64 theta){}
Number Number::real(void) const{}
Number Number::imag(void) const{}
Number Number::conj(void) const{}
Number Number::arg(void) const{}
Number Number::abs(void) const{}

Number operator+(const Number& lhs, const Number& rhs){}
Number operator-(const Number& lhs, const Number& rhs){}
Number operator*(const Number& lhs, const Number& rhs){}
Number operator/(const Number& lhs, const Number& rhs){}
Number operator%(const Number& lhs, const Number& rhs){}
Number operator==(const Number& lhs, const Number& rhs){}
Number operator!=(const Number& lhs, const Number& rhs){}
Number operator<=(const Number& lhs, const Number& rhs){}
Number operator>=(const Number& lhs, const Number& rhs){}
Number operator<(const Number& lhs, const Number& rhs){}
Number operator>(const Number& lhs, const Number& rhs){}

Number operator<<(const Number& lhs, const Number& rhs){}
Number operator>>(const Number& lhs, const Number& rhs){}
Number operator|(const Number& lhs, const Number& rhs){}
Number operator&(const Number& lhs, const Number& rhs){}
Number& Number::operator~(void){}

Number Number::exp(void) const{}
Number Number::log(void) const{}
Number Number::log10(void) const{}
Number Number::sqrt(void) const{}
Number Number::pow(void) const{}
Number Number::sin(void) const{}
Number Number::cos(void) const{}
Number Number::tan(void) const{}
Number Number::asin(void) const{}
Number Number::acos(void) const{}
Number Number::atan(void) const{}
Number Number::sinh(void) const{}
Number Number::cosh(void) const{}
Number Number::tanh(void) const{}
Number Number::asinh(void) const{}
Number Number::acosh(void) const{}
Number Number::atanh(void) const{}
 
Number Number::cbrt(void) const{}
Number Number::hypot(void) const{}
Number Number::erf(void) const{}
Number Number::erfc(void) const{}
Number Number::tgamma(void) const{}
Number Number::ceil(void) const{}
Number Number::floor(void) const{}
Number Number::round(void) const{}

Bool Number::isnan(void) const{}
Bool Number::isinf(void) const{}
Bool Number::isfinite(void) const{}
*/

// -----------------------------
// -*- IMPLEMENTATION STRING -*-
// -----------------------------
/*
String::String() noexcept{}
String::String(const Str& val) noexcept{}
String::String(const String& other) noexcept{}
String::String(String&& other) noexcept{}
String& String::operator=(const String& other) noexcept{}
String& String::operator=(String&& other) noexcept{}
Str String::str(void) const{}
Str String::repr(void) const{}
Symbol String::type(void) const{}
Str String::as_str(void) const{}

Bool operator==(const String& lhs, const String& rhs){}
Bool operator!=(const String& lhs, const String& rhs){}
Bool operator<=(const String& lhs, const String& rhs){}
Bool operator>=(const String& lhs, const String& rhs){}
Bool operator<(const String& lhs, const String& rhs){}
Bool operator>(const String& lhs, const String& rhs){}

String operator+(const String& lhs, const String& rhs){}

Number String::len(void) const{}
Bool String::startswith(const String& prefix) const{}
Bool String::endswith(const String& suffix) const{}
String String::join(List& xs) const{}
String String::replace(const Str& old, const Str& neo, bool all){}
String String::ltrim(void){}
String String::rtrim(void){}
String String::trim(void){}
Number String::find(const Str& pattern){}
String String::substr(Number first, Number last){}
String String::upper(void){}
String String::lower(void){}
List String::split(const String sep){}
String String::title(void){}
String String::capitalize(void){}
Bool String::is_numeric(void){}
Bool String::is_space(void){}
Bool String::is_lower(void){}
Bool String::is_upper(void){}
*/

// ------------------------------
// -*- IMPLEMENTATION OF LIST -*-
// ------------------------------
/*
List::List(Vec<Self> vec) noexcept{}
List::List(const List& other) noexcept{}
List::List(List&& other) noexcept{}
List& List::operator=(const List& other){}
List& List::operator=(List&& other){}
Str List::str(void) const{}
Str List::repr(void) const{}
Symbol List::type(void) const{}

Vec<Self> List::as_vec(void) const{}

List operator+(const List& lhs, const List& rhs){}
List operator==(const List& lhs, const List& rhs){}
List operator!=(const List& lhs, const List& rhs){}

Number List::len(void) const{}
Self List::head(void) const{}
Self List::last(void) const{}
List List::tail(void) const{}
List List::push(Self self) const{}
Self List::pop(void){}
List List::append(Self self) const{}
List List::slice(Number first, Number last){}
Number List::find(const Self& self){}
List List::insert(Number index, Self val){}
List List::remove(Number index, Self val){}
List List::sort(Callable* keyfn){}
List List::sort(Symbol key){}
*/

// -----------------------------
// -*- IMPLEMENTATION LAMBDA -*-
// -----------------------------
/*
Lambda::Lambda(List& params, Self body, Env& env) noexcept{}
Lambda::Lambda(const Lambda& lambda) noexcept{}
Lambda::Lambda(Lambda&& lambda) noexcept{}
Lambda& Lambda::operator=(const Lambda& lambda) noexcept{}
Lambda& Lambda::operator=(Lambda&& lambda) noexcept{}
Str Lambda::str(void) const{}
Str Lambda::repr(void) const{}
Symbol Lambda::type(void) const{}
const List& Lambda::params(void) const{}
const Self& Lambda::body(void) const{}
const Env& Lambda::env(void) const{}
Self Lambda::call(Args args){}
*/

// ------------------------------
// -*- IMPLEMENTATION BUILTIN -*-
// ------------------------------
/*
Builtin::Builtin(CFun cfun){}
Str Builtin::str(void) const{}
Str Builtin::repr(void) const{}
Symbol Builtin::type(void) const{}
const Builtin::CFun& cfun(void) const{}
Self Builtin::call(Args args){}
*/

// -------------------------------
// -*- IMPLEMENTATION FUNCTION -*-
// -------------------------------
/*
Function::Function(List& params, Self body, const Env& env) noexcept{}
Str Function::str(void) const{}
Str Function::repr(void) const{}
Symbol Function::type(void) const{}
const List& Function::params(void) const{}
const Self& Function::body(void) const{}
const Env& Function::env(void) const{}
Self Function::call(Args args){}
*/

// ------------------------------
// -*- IMPLEMENATION OF MACRO -*-
// ------------------------------
/*
Macro::Macro(List& params, Self body, const Env& env) noexcept{}
Str Macro::str(void) const{}
Str Macro::repr(void) const{}
Symbol Macro::type(void) const{}
const List& Macro::params(void) const{}
const Self& Macro::body(void) const{}
const Env& Macro::env(void) const{}
List Macro::expand(void){}
Self Macro::call(Args args){}
*/


// -*-
/* Token: Symbol, String, Number, LParen, */
enum class TokenKind{
    Eof,
    Bool,
    Ident, // Operators, function-name, variable-name, keywords 
    Int,
    Float,
    Cmplx,
    String,
    LParen,
    RParen,
};

struct Token{
    TokenKind kind;
    Str lexeme;
    i32 row;
    i32 col;
};

class Scanner final{
private:
    Str m_src;
    i32 m_row;
    i32 m_col;
    i32 m_pos;

public:
    Scanner() = default;
    explicit Scanner(const Str& src) noexcept;
    Scanner(const Scanner&) = delete;
    Scanner& operator=(const Scanner&) = delete;
    Scanner(Scanner&& other) noexcept;
    Scanner& operator=(Scanner&& other) noexcept;

    ~Scanner() = default;

    Token scan(void);
    
private:
    bool is_keyword(const Str& key);
    bool is_operator(const Str& key);
    bool is_valid_ident_char(char c);
    bool is_valid_start_ident_char(char c);
    bool is_eos(void);
    Str read_str(void);
    Str read_float(void);
    Str read_integer(void);
    Str read_bool(void);

    void skip_whitespace(void);
    void skip_comment(void);
    void peek(i32 i=0);
    void advance(void);
};

/*
Scanner::Scanner(const Str& src) noexcept{}
Scanner::Scanner(Scanner&& other) noexcept{}
Scanner::Scanner& operator=(Scanner&& other) noexcept{}
Token Scanner::scan(void){}
bool Scanner::is_keyword(const Str& key){}
bool Scanner::is_operator(const Str& key){}
bool Scanner::is_valid_ident_char(char c){}
bool Scanner::is_valid_start_ident_char(char c){}
bool Scanner::is_eos(void){}
Str Scanner::read_str(void){}
Str Scanner::read_float(void){}
Str Scanner::read_integer(void){}
Str Scanner::read_bool(void){}
void Scanner::skip_whitespace(void){}
void Scanner::skip_comment(void){}
void Scanner::peek(i32 i=0){}
void Scanner::advance(void){}
*/

class Parser final{
private:
    Scanner m_scanner;
    Token m_curTok;

public:
    explicit Parser() = default;
    explicit Parser(Scanner&& scanner) noexcept;
    Parser(const Parser&) = delete;
    Parser& operator=(const Parser) = delete;
    Parser(Parser&& other) noexcept;
    Parser& operator=(Parser&& other) noexcept;
    ~Parser() = default;

    Self parse(void);

private:
    Self parse_bool(void);
    Self parse_integer(void);
    Self parse_float(void);
    Self parse_complex(void);
    Self parse_string(void);
    Self parse_identifier(void);
    Self parse_list(void);
    void expect(Symbol sym);
    bool match(Symbol sym, Symbol expected);
};

/*
Parser::Parser(Scanner&& scanner) noexcept{}
Parser::Parser(Parser&& other) noexcept{}
Parser& Parser::operator=(Parser&& other) noexcept{}
Self Parser::parse(void){}

Self Parser::parse_bool(void){}
Self Parser::parse_integer(void){}
Self Parser::parse_float(void){}
Self Parser::parse_complex(void){}
Self Parser::parse_string(void){}
Self Parser::parse_identifier(void){}
Self Parser::parse_list(void){}
void Parser::expect(Symbol sym){}
bool Parser::match(Symbol ident, Symbol expected){}
*/

// -*-
class Celine final{
private:
    static Env prelude;

public:
    explicit Celine() noexcept = default;
    ~Celine() = default;

    static HashMap<Str, Str> documentation;
    static Env runtime;

    /*
    ?, :h               ==> help
    :diary filename     ==> log interactive session into filename
    :q, :quit           ==> quit text session
    */
    static void repl(Vec<Str> argv);

    /*
    -v, --version       ==> print the version of the interpreter
    -e sexpr            ==> evaluate s-expression
    --license           ==> print the license
    -h, --help          ==> print this help
    --no-banner         ==> do not show the banner 
    */
    static void run(Vec<Str> args);

private:
    static void initialize_prelude(void);
    static void initialize_runtime(void);

    static Self eval(Env& env);
    static Self eval(Args args, Env& env);
    static Self eval_bool(Bool self);
    static Self eval_number(Number self);
    static Self eval_ident(Symbol self, Env& env);
    static Self eval_String(String self, Env& env);
    static Self eval_keyword(Args args, Env& env); // 
    static Self eval_list(Args args, Env& env); // function-call
    static Self eval_progn(Args args, Env& env);
    static Self eval_define(Args args, Env& env);
    static Self eval_var(Args args, Env& env);
    static Self eval_let(Args args, Env& env);
    static Self eval_for(Args args, Env& env);
    static Self eval_if(Args args, Env& env);
    static Self eval_fun(Args args, Env& env);
    static Self eval_lambda(Args args, Env& env);
    static Self eval_macro(Args args, Env& env);
    static Self eval_quote(Args args, Env& env);
    static Self eval_quasiquote(Args args, Env& env);
    static Self eval_unquote(Args args, Env& env);
    static Self eval_unquote_splicing(Args args, Env& env);
    static Self eval_cond(Args args, Env& env);
};

// -*-
Env Celine::prelude = Env();
Env Celine::runtime = Env();
HashMap<Str, Str> Celine::documentation = {};

// -*-
Self fn_print(Args args);
Self fn_println(Args args);
Self fn_eprint(Args args);
Self fn_eprintln(Args args);
Self fn_panic(Args args);
Self fn_input(Args args);

Self fn_typeof(Args args);
Self fn_eval(Args args);
Self fn_apply(Args args);
Self fn_map(Args args);
Self fn_reduce(Args args);
Self fn_filter(Args args);
Self fn_zip(Args args);
Self fn_sort(Args args);
Self fn_min(Args args);
Self fn_max(Args args);
Self fn_random(Args args);
Self fn_range(Args args);
Self fn_linspace(Args args);
Self fn_boolp(Args args);
Self fn_integerp(Args args);
Self fn_floatp(Args args);
Self fn_complexp(Args args);
Self fn_symbolp(Args args);
Self fn_keywordp(Args args);
Self fn_stringp(Args args);
Self fn_listp(Args args);
Self fn_length(Args args);
Self fn_concat(Args args);

// -*-
Self fn_add(Args args);
Self fn_sub(Args args);
Self fn_mul(Args args);
Self fn_div(Args args);
Self fn_mod(Args args);
Self fn_lsh(Args args);
Self fn_rhs(Args args);
Self fn_bit_or(Args args);
Self fn_bit_and(Args args);
Self fn_bit_not(Args args);

Self fn_less(Args args);
Self fn_greater(Args args);
Self fn_less_eq(Args args);
Self fn_greater_eq(Args args);
Self fn_equal(Args args);

Self fn_bool(Args args);
Self fn_not(Args args);
Self fn_or(Args args);
Self fn_and(Args args);

Self fn_integer(Args args);
Self fn_float(Args args);
Self fn_complex(Args args);
Self fn_abs(Args args);
Self fn_ceil(Args args);        // -cmplx
Self fn_round(Args args);       // -cmplx
Self fn_floor(Args args);       // -cmplx
Self fn_abs(Args args);
Self fn_cmplx_real(Args args);
Self fn_cmplx_imag(Args args);
Self fn_cmplx_arg(Args args);
Self fn_cmplx_conj(Args args);
Self fn_cmplx_polar(Args args);
Self fn_cmplx_norm(Args args);
Self fn_exp(Args args);
Self fn_log(Args args);
Self fn_log10(Args args);
Self fn_sqrt(Args args);
Self fn_cbrt(Args args);    // -cmplx
Self fn_pow(Args args);
Self fn_sin(Args args);
Self fn_cos(Args args);
Self fn_tan(Args args);
Self fn_asin(Args args);
Self fn_acos(Args args);
Self fn_atan(Args args);
Self fn_atan2(Args args);   // -cmplx
Self fn_sinh(Args args);
Self fn_cosh(Args args);
Self fn_tanh(Args args);
Self fn_asinh(Args args);
Self fn_acosh(Args args);
Self fn_atanh(Args args);
Self fn_exp2(Args args);    // -cmplx
Self fn_expm1(Args args);   // -cmplx
Self fn_log2(Args args);    // -cmplx
Self fn_log1p(Args args);   // -cmplx
Self fn_hypot(Args args);   // -cmplx
Self fn_erf(Args args);     // -cmplx
Self fn_erfc(Args args);    // -cmplx
Self fn_tgamma(Args args);  // -cmplx
Self fn_lgamma(Args args);  // -cmplx
Self fn_isinf(Args args);   // -cmplx
Self fn_isfinite(Args args);// -cmplx
Self fn_isnan(Args args);   // -cmplx

Self fn_string(Args args);
Self fn_string_startswith(Args args);
Self fn_string_endswith(Args args);
Self fn_string_join(Args args);
Self fn_string_concat(Args args);
Self fn_string_replace(Args args);
Self fn_string_ltrim(Args args);
Self fn_string_rtrim(Args args);
Self fn_string_trim(Args args);
Self fn_string_find(Args args);
Self fn_string_substr(Args args);
Self fn_string_upper(Args args);
Self fn_string_lower(Args args);
Self fn_string_split(Args args);
Self fn_string_title(Args args);
Self fn_string_capitalize(Args args);
Self fn_string_is_numeric(Args args);
Self fn_string_is_space(Args args);
Self fn_string_is_lower(Args args);
Self fn_string_is_upper(Args args);

Self fn_list(Args args);
Self fn_list_head(Args args);
Self fn_list_last(Args args);
Self fn_list_tail(Args args);
Self fn_list_push(Args args);
Self fn_list_pop(Args args);
Self fn_list_append(Args args);
Self fn_list_concat(Args args);
Self fn_list_slice(Args args);
Self fn_list_find(Args args);
Self fn_list_insert(Args args);
Self fn_list_remove(Args args);

// -------------------------------------
// -*- IMPLEMENTATION OF INTERPRETER -*-
// -------------------------------------
/*
void Celine::repl(Vec<Str> argv){}
void Celine::run(Vec<Str> args){}

void Celine::initialize_prelude(void){
    PI, E, INT-MAX, INT-MIN, FLOAT-MIN, FLOAT_MAX, EPSILON
    fn_print(Args args);
    fn_println(Args args);
    fn_eprint(Args args);
    fn_eprintln(Args args);
    fn_panic(Args args);
    fn_input(Args args);

    fn_typeof(Args args);
    fn_eval(Args args);
    fn_apply(Args args);
    fn_map(Args args);
    fn_reduce(Args args);
    fn_filter(Args args);
    fn_zip(Args args);
    fn_sort(Args args);
    fn_min(Args args);
    fn_max(Args args);
    fn_random(Args args);
    fn_range(Args args);
    fn_linspace(Args args);
    fn_boolp(Args args);
    fn_integerp(Args args);
    fn_floatp(Args args);
    fn_complexp(Args args);
    fn_symbolp(Args args);
    fn_keywordp(Args args);
    fn_stringp(Args args);
    fn_listp(Args args);
    fn_length(Args args);
    fn_concat(Args args);

    // -*-
    fn_add(Args args);
    fn_sub(Args args);
    fn_mul(Args args);
    fn_div(Args args);
    fn_mod(Args args);
    fn_lsh(Args args);
    fn_rhs(Args args);
    fn_bit_or(Args args);
    fn_bit_and(Args args);
    fn_bit_not(Args args);

    fn_less(Args args);
    fn_greater(Args args);
    fn_less_eq(Args args);
    fn_greater_eq(Args args);
    fn_equal(Args args);

    fn_bool(Args args);
    fn_not(Args args);
    fn_or(Args args);
    fn_and(Args args);

    fn_integer(Args args);
    fn_float(Args args);
    fn_complex(Args args);
    fn_abs(Args args);
    fn_ceil(Args args);        // -cmplx
    fn_round(Args args);       // -cmplx
    fn_floor(Args args);       // -cmplx
    fn_abs(Args args);
    fn_cmplx_real(Args args);
    fn_cmplx_imag(Args args);
    fn_cmplx_arg(Args args);
    fn_cmplx_conj(Args args);
    fn_cmplx_polar(Args args);
    fn_cmplx_norm(Args args);
    fn_exp(Args args);
    fn_log(Args args);
    fn_log10(Args args);
    fn_sqrt(Args args);
    fn_cbrt(Args args);         // -cmplx
    fn_pow(Args args);
    fn_sin(Args args);
    fn_cos(Args args);
    fn_tan(Args args);
    fn_asin(Args args);
    fn_acos(Args args);
    fn_atan(Args args);
    fn_atan2(Args args);        // -cmplx
    fn_sinh(Args args);
    fn_cosh(Args args);
    fn_tanh(Args args);
    fn_asinh(Args args);
    fn_acosh(Args args);
    fn_atanh(Args args);
    fn_exp2(Args args);         // -cmplx
    fn_expm1(Args args);        // -cmplx
    fn_log2(Args args);         // -cmplx
    fn_log1p(Args args);        // -cmplx
    fn_hypot(Args args);        // -cmplx
    fn_erf(Args args);          // -cmplx
    fn_erfc(Args args);         // -cmplx
    fn_tgamma(Args args);       // -cmplx
    fn_lgamma(Args args);       // -cmplx
    fn_isinf(Args args);        // -cmplx
    fn_isfinite(Args args);     // -cmplx
    fn_isnan(Args args);        // -cmplx

    fn_string(Args args);
    fn_string_startswith(Args args);
    fn_string_endswith(Args args);
    fn_string_join(Args args);
    fn_string_concat(Args args);
    fn_string_replace(Args args);
    fn_string_ltrim(Args args);
    fn_string_rtrim(Args args);
    fn_string_trim(Args args);
    fn_string_find(Args args);
    fn_string_substr(Args args);
    fn_string_upper(Args args);
    fn_string_lower(Args args);
    fn_string_split(Args args);
    fn_string_title(Args args);
    fn_string_capitalize(Args args);
    fn_string_is_numeric(Args args);
    fn_string_is_space(Args args);
    fn_string_is_lower(Args args);
    fn_string_is_upper(Args args);

    fn_list(Args args);
    fn_list_head(Args args);
    fn_list_last(Args args);
    fn_list_tail(Args args);
    fn_list_push(Args args);
    fn_list_pop(Args args);
    fn_list_append(Args args);
    fn_list_concat(Args args);
    fn_list_slice(Args args);
    fn_list_find(Args args);
    fn_list_insert(Args args);
    fn_list_remove(Args args);

}

void Celine::initialize_runtime(void){}
Self Celine::eval(Args args, Env& env){}
Self Celine::eval_bool(Bool self){}
Self Celine::eval_number(Number self){}
Self Celine::eval_ident(Symbol self, Env& env){}
Self Celine::eval_String(String self, Env& env){}
Self Celine::eval_keyword(Args args, Env& env){} 
Self Celine::eval_list(Args args, Env& env){}
Self Celine::eval_progn(Args args, Env& env){}
Self Celine::eval_define(Args args, Env& env){}
Self Celine::eval_var(Args args, Env& env){}
Self Celine::eval_let(Args args, Env& env){}
Self Celine::eval_for(Args args, Env& env){}
Self Celine::eval_if(Args args, Env& env){}
Self Celine::eval_fun(Args args, Env& env){}
Self Celine::eval_lambda(Args args, Env& env){}
Self Celine::eval_macro(Args args, Env& env){}
Self Celine::eval_quote(Args args, Env& env){}
Self Celine::eval_quasiquote(Args args, Env& env){}
Self Celine::eval_unquote(Args args, Env& env){}
Self Celine::eval_unquote_splicing(Args args, Env& env){}
Self Celine::eval_cond(Args args, Env& env){}
*/


// -*----------------------------------------------------------------*-
}//-*- end::namespace::cln                                          -*-
// -*----------------------------------------------------------------*-

// -*-
int main(int argc, char** argv){
    //! @todo

    return EXIT_SUCCESS;
}