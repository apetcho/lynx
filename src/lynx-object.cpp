#include "lynx.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::lynx                                       -*-
// -*----------------------------------------------------------------*-
namespace lynx{
// --------------
// -*- Symbol -*-
// --------------
// -*-
Symbol::Symbol(const Str& data) noexcept: m_data{data}{}

// -*-
Symbol::Symbol(const Symbol& sym) noexcept: m_data{sym.m_data}{}

// -*-
Symbol::Symbol(Symbol&& sym) noexcept: m_data{std::move(sym.m_data)} {}

// -*-
Symbol& Symbol::operator=(const Symbol& sym) noexcept{
    if(this != &sym){
        this->m_data = sym.m_data;
    }
    return *this;
}

// -*-
Symbol& Symbol::operator=(Symbol&& sym) noexcept{
    if(this != &sym){
        this->m_data = std::move(sym.m_data); 
    }
    return *this;
}

// -*-
Symbol::operator Str(){
    return this->m_data;
}

// -*-
bool operator==(const Symbol& lhs, const Symbol& rhs){
    return lhs.m_data == rhs.m_data;
}

// -*-
bool operator<(const Symbol& lhs, const Symbol& rhs){
    return lhs.m_data < rhs.m_data;
}

// -*-
bool operator==(const Symbol& lhs, const Str& rhsStr){
    return lhs.m_data == rhsStr;
}

bool operator==(const Str& lhsStr, const Symbol& rhs){
    return lhsStr == rhs.m_data;
}

// -*-
bool operator<(const Str& lhsStr, const Symbol& rhs){
    return lhsStr < rhs.m_data;
}

bool operator<(const Symbol& lhs, const Str& rhsStr){
    return lhs.m_data < rhsStr;
}

// -------------
// -*- Error -*-
// -------------
/*
Error::Error() noexcept{}
Error::Error(Error::Kind kind, Str msg) noexcept{}
Error::Error(Kind kind, const Self& reason) noexcept{}
Error::Error(const Symbol& sym, Str msg) noexcept{}
Error::Error(const Symbol& sym, const Self& reason) noexcept{}
Error::Error(const Error& error) noexcept{}
Error::Error(Error&& error) noexcept{}
Error& Error::operator=(const Error& error) noexcept;
Error::Error(Error&& error) noexcept{}
Symbol Error::kind() const{}
const Self& Error::reason(void) const{}
const Str& Error::message(void) const{}
Str Error::describe(void) const;
*/

// --------------
// -*- Result -*-
// --------------
/*
Result::Result() noexcept{}
Result::Result(const Self& self) noexcept{}
Result::Result(const Error& error) noexcept{}
Result::Result(const Result& result) noexcept{}
Result::Result(Result&& result) noexcept{}
Result& Result::operator=(const Result& result) noexcept{}
Result& Result::operator=(Result& result) noexcept{}
Self Result::ok(void) const;
Error Result::err(void) const;
bool Result::is_ok(void) const;
*/

// -----------------
// -*- Structure -*-
// -----------------
/*

Structure::Structure(const Str& name) noexcept{}
Structure::Structure(const Structure& structure) noexcept{}
Structure::Structure(Structure&& structure) noexcept{}
Structure& Structure::operator=(const Structure& structure) noexcept{}
Structure& Structure::operator=(Structure&& structure) noexcept{}
Str Structure::name(void) const{}
HashMap<Str, Self> Structure::properties(void) const{}
HashMap<Str, Self> Structure::objvars(void) const{}
HashMap<Str, Ast> Structure::methods(void) const{}
void Structure::define_initilizer(Self initfn){}
void Structure::initialize(Args args){}
void Structure::push_struct_attr(Self attr){}
void Structure::push_property(Self property){}
void Structure::push_objvar(Self var){}
void Structure::push_method(Self method){}
void Structure::pop_struct_attr(){}
void Structure::pop_property(const Str& key){}
void Structure::pop_objvar(const Str& key){}
void Structure::pop_method(const Str& key){}
*/


// --------------
// -*- Object -*-
// --------------
/*
Object::Object() noexcept{}
Object::Object(bool val) noexcept{}
Object::Object(i64 num) noexcept{}
Object::Object(f64 num) noexcept{}
Object::Object(Complex z) noexcept{}
Object::Object(Symbol sym) noexcept{}
Object::Object(Str str) noexcept{}
Object::Object(TypeKind kind, List data) noexcept{}
Object::Object(HSet data) noexcept{}
Object::Object(HMap data) noexcept{}
Object::Object(Structure klass) noexcept{}
Object::Object(Iterator iter) noexcept{}
Object::Object(const Str& name, CFun cfun) noexcept{}
Object::Object(Ast ast) noexcept{}
Object::Object(Result result) noexcept{}
Object::Object(const Object& obj) noexcept{}
Object::Object(Object&& obj) noexcept{}
Object& Object::operator=(const Object& obj) noexcept{}
Object& Object::operator=(Object&& obj) noexcept{}
Object::~Object(){}

Object::operator bool(){}
Object::operator i64(){}
Object::operator f64(){}
Object::operator Complex(){}
Object::operator Symbol(){}
Object::operator Str(){}
Object::operator List(){}
Object::operator HSet(){}
Object::operator HMap(){}
Object::operator CFun(){}
Object::operator Result(){}

Object& Object::operator!(){}
Object operator&&(const Object& lhs, const Object& rhs){}
Object operator||(const Object& lhs, const Object& rhs){}

Object& Object::operator~(){}
Object operator|(const Object& lhs, const Object& rhs){}
Object operator&(const Object& lhs, const Object& rhs){}
Object operator^(const Object& lhs, const Object& rhs){}
Object operator<<(const Object& lhs, const Object& rhs){}
Object operator>>(const Object& lhs, const Object& rhs){}

bool operator==(const Object& lhs, const Object& rhs){}
bool operator!=(const Object& lhs, const Object& rhs){}
bool operator<=(const Object& lhs, const Object& rhs){}
bool operator>=(const Object& lhs, const Object& rhs){}
bool operator<(const Object& lhs, const Object& rhs){}
bool operator>(const Object& lhs, const Object& rhs){}

Object operator+(const Object& lhs, const Object& rhs){}
Object operator-(const Object& lhs, const Object& rhs){}
Object operator/(const Object& lhs, const Object& rhs){}
Object operator%(const Object& lhs, const Object& rhs){}
Object operator*(const Object& lhs, const Object& rhs){}
Object& Object::operator+=(const Object& lhs){}
Object& Object::operator-=(const Object& lhs){}
Object& Object::operator/=(const Object& lhs){}
Object& Object::operator%=(const Object& lhs){}
Object& Object::operator*=(const Object& lhs){}

// -----------------------
// -*- Index Operators -*-
// -----------------------
Self& Object::operator[](int idx){}
const Self& Object::operator[](int idx) const{}
Self& Object::operator[](const Self& key){}
const Self& Object::operator[](const Self& key) const{}

// ------------------
// -*- Predicates -*-
// ------------------
bool Object::is_nil(void) const{}
bool Object::is_bool(void) const{}
bool Object::is_integer(void) const{}
bool Object::is_float(void) const{}
bool Object::is_complex(void) const{}
bool Object::is_number(void) const{}
bool Object::is_symbol(void) const{}
bool Object::is_string(void) const{}
bool Object::is_tuple(void) const{}
bool Object::is_list(void) const{}
bool Object::is_hashset(void) const{}
bool Object::is_hashmap(void) const{}
bool Object::is_builtin(void) const{}
bool Object::is_function(void) const{}
bool Object::is_macro(void) const{}
bool Object::is_lambda(void) const{}
bool Object::is_callable(void) const{}
bool Object::is_hashable(void) const{}
bool Object::is_ok(void) const{}
bool Object::is_structure(void) const{}


// ------------------------------
// -*- fn, fun, lambda, macro -*-
// ------------------------------
Self Object::operator()(Args args){}

// ------------------------
// -*- call type method -*-
// ------------------------
Self Object::operator()(const Str& fname, Args args){}

Str Object::format(void) const{}
Self Object::ok(void) const{}
Error Object::err(void) const{}

Symbol Object::type(void) const{}
Str Object::repr(void) const{}
usize Object::hash(void) const{}
Iterator Object::iter(void) const{}

// -------------------------------
// -*- Complex-numbers methods -*-
// -------------------------------
Self Object::real(Args);
Self Object::imag(Args);
Self Object::abs(Args);
Self Object::arg(Args);
Self Object::norm(Args);
Self Object::conj(Args);
Self Object::polar(Args);

// -*---------------------------------
// -*- Sequence Specific Operators -*-
// -----------------------------------
Self Object::clear(Args args){}
Self Object::len(Args args){}
Self Object::find(Args args){}
Self Object::find_all(Args args){}
Self Object::find_last(Args args){}
Self Object::slice(Args args){}
Self Object::sort(Args args){}

// ---------------------------------
// -*- String Specific Operators -*-
// ---------------------------------
Self Object::upper(Args args){}
Self Object::lower(Args args){}
Self Object::capitalize(Args args){}
Self Object::title(Args args){}
Self Object::split(Args args){}
Self Object::join(Args args){}
Self Object::endswith(Args args){}
Self Object::startswith(Args args){}
Self Object::chr(Args args){}
Self Object::ord(Args args){}

Self Object::index(Args args){}
Self Object::remove(Args args){}
Self Object::insert(Args args){}
Self Object::head(Args args){}
Self Object::tail(Args args){}
Self Object::last(Args args){}
Self Object::extend(Args args){}
Self Object::append(Args args){}
Self Object::pop(Args args){}
Self Object::push(Args args){}

Self Object::keys(Args args){}
Self Object::values(Args args){}
Self Object::items(Args args){}
// dict[key]        <Self get(Args args){}> | const Self& Obj::op[](Self k)
// dict[key] = val  <Self put(Args args){}> | const Self& Obj::op[](Self k)
Self Object::popitem(Args args){}
Self Object::update(Args args){}

Self Object::setattr(Args args){}
Self Object::getattr(Args args){}
Self Object::hasattr(Args args){}
Self Object::delattr(Args args){}

std::ostream& operator<<(std::ostream& os, const Object& obj){}

// Arithmethic-ops
Self Object::__add__(Args args){}
Self Object::__sub__(Args args){}
Self Object::__mul__(Args args){}
Self Object::__div__(Args args){}
Self Object::__pow__(Args args){}
Self Object::__add_assign__(Args args){}
Self Object::__sub_assign__(Args args){}
Self Object::__mul_assign__(Args args){}
Self Object::__div_assign__(Args args){}
Self Object::__mod_assign__(Args args){}
Self Object::__pow_assign__(Args args){}
// Logical-ops
Self Object::__logical_or__(Args args){}
Self Object::__logical_and__(Args args){}
Self Object::__logical_not__(Args args){}
// Relational-ops
Self Object::__lt__(Args args){}
Self Object::__le__(Args args){}
Self Object::__gt__(Args args){}
Self Object::__ge__(Args args){}
Self Object::__eq__(Args args){}
Self Object::__ne__(Args args){}
// Bitwise-ops
Self Object::__bit_not__(Args args){}
Self Object::__bit_or__(Args args){}
Self Object::__bit_xor__(Args args){}
Self Object::__bit_and__(Args args){}
Self Object::__bit_shl__(Args args){}
Self Object::__bit_shr__(Args args){}
Self Object::__bit_or_assign__(Args args){}
Self Object::__bit_xor_assign__(Args args){}
Self Object::__bit_and_assign__(Args args){}
Self Object::__bit_shl_assign__(Args args){}
Self Object::__bit_shr_assign__(Args args){}
// Indexing
Self Object::__getitem__(Args args){}
Self Object::__setitem__(Args args){}
// Slicing-ops
Self Object::__slice__(Args args){}
// Iterable
Self Object::__next__(Args args){}
Self Object::__done__(Args args){}
// Property-handler
Self Object::__setattr__(Args args){}
Self Object::__getattr__(Args args){}
Self Object::__delattr__(Args args){}
// String
Self Object::__str__(Args args){}
// Parse-able string
Self Object::__repr__(Args args){}
// Hashable
Self Object::__hash__(Args args){}
// Callable
Self Object::__call__(Args args){}
// typecat-operators
Self Object::__bool__(Args args){}
Self Object::__integer__(Args args){}
Self Object::__float__(Args args){}
Self Object::__tuple__(Args args){}
Self Object::__list__(Args args){}
Self Object::__hashset__(Args args){}
Self Object::__hashmap__(Args args){}
*/

// -----------------
// -*- Iterators -*-
// -----------------
/*
StringIterator::StringIterator(const Self& data) noexcept{}
StringIterator::StringIterator(StringIterator&& iterator) noexcept{}
StringIterator& StringIterator::operator=(StringIterator&& iterator) noexcept{}
StringIterator::~StringIterator(){}
Self StringIterator::next(void){}
Self StringIterator::done(void){}

TupleIterator::TupleIterator(const Self& data) noexcept{}
TupleIterator::TupleIterator(TupleIterator&& iterator) noexcept{}
TupleIterator& TupleIterator::operator=(TupleIterator&& iterator) noexcept{}
TupleIterator::~TupleIterator(){}
Self TupleIterator::next(void){}
Self TupleIterator::done(void){}

// -*-
ListIterator::ListIterator(const Self& data) noexcept{}
ListIterator::ListIterator(ListIterator&& iterator) noexcept{}
ListIterator& ListIterator::operator=(ListIterator&& iterator) noexcept{}
ListIterator::~ListIterator(){}
Self ListIterator::next(void){}
Self ListIterator::done(void){}

// -*-
DictIterator::DictIterator(const Self& data) noexcept{}
DictIterator::DictIterator(DictIterator&& iterator) noexcept{}
DictIterator& DictIterator::operator=(const Self& data) noexcept{}
DictIterator::~DictIterator(){}
Self DictIterator::next(void){}
Self DictIterator::done(void){}

// -*-
SetIterator::SetIterator(const Self& data) noexcept{}
SetIterator::SetIterator(const Self& data) noexcept{}
SetIterator& SetIterator::operator=(const Self& data) noexcept{}
SetIterator::~SetIterator(){}
Self SetIterator::next(void){}
Self SetIterator::done(void){}

// -*-
RangeIterator::RangeIterator(const Self& start, const Self& stop) noexcept;
RangeIterator::RangeIterator(const Self& start, const Self& stop, const Self& step) noexcept;
RangeIterator::RangeIterator(RangeIterator&& iterator){}
RangeIterator& RangeIterator::operator=(RangeIterator&& iterator){}
RangeIterator::~RangeIterator(){}
Self RangeIterator::next(void){}
Self RangeIterator::done(void){}

*/


// -*----------------------------------------------------------------*-
}//-*- end::namespace::lynx                                         -*-
// -*----------------------------------------------------------------*-