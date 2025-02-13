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
// -*-
Error::Error() noexcept
: m_kind{Error::Kind::Error}
, m_reason{nullptr}
, m_msg{""}
, m_symbol{}
{}

// -*-
Error::Error(Error::Kind kind, Str msg) noexcept
: m_kind{kind}
, m_reason{nullptr}
, m_msg{msg}
, m_symbol{}
{}

// -*-
Error::Error(Kind kind, const Self& reason) noexcept
: m_kind{kind}
, m_reason{reason}
, m_msg{""}
, m_symbol{}
{}

Error::Error(const Symbol& sym, Str msg) noexcept
: m_kind{Error::Kind::Error}
, m_reason{nullptr}
, m_msg{msg}
, m_symbol{sym}
{}

// -*-
Error::Error(const Symbol& sym, const Self& reason) noexcept
: m_kind{Error::Kind::Error}
, m_reason{reason}
, m_msg{""}
, m_symbol{sym}
{}

Error::Error(const Error& error) noexcept
: m_kind{error.m_kind}
, m_reason{error.m_reason}
, m_msg{error.m_msg}
, m_symbol{error.m_symbol}
{}

Error::Error(Error&& error) noexcept
: m_kind{std::move(error.m_kind)}
, m_reason{std::move(error.m_reason)}
, m_msg{std::move(error.m_msg)}
, m_symbol{std::move(error.m_symbol)}
{}

Error& Error::operator=(const Error& error) noexcept{
    if(this != &error){
        this->m_kind = error.m_kind;
        this->m_reason = error.m_reason;
        this->m_msg = error.m_msg;
        this->m_symbol = error.m_symbol;
    }
    return *this;
}

Error& Error::operator=(Error&& error) noexcept{
    if(this != &error){
        this->m_kind = std::move(error.m_kind);
        this->m_reason = std::move(error.m_reason);
        this->m_msg = std::move(error.m_msg);
        this->m_symbol = std::move(error.m_symbol);
    }
    return *this;
}

Symbol Error::kind() const{
    if(this->m_symbol.is_defined()){
        return this->m_symbol;
    }
    static HashMap<Error::Kind, Symbol> ErrorKinds = {
        {Error::Kind::Error, Symbol("Error")},
        {Error::Kind::Error, Symbol("TypeError")},
        {Error::Kind::Error, Symbol("ValueError")},
        {Error::Kind::Error, Symbol("SyntaxError")},
        {Error::Kind::Error, Symbol("IndexError")},
        {Error::Kind::Error, Symbol("KeyError")},
        {Error::Kind::Error, Symbol("SymbolError")},
        {Error::Kind::Error, Symbol("AttributeError")},
        {Error::Kind::Error, Symbol("RuntimeError")},
    };

    if(ErrorKinds.find(this->m_kind)!= ErrorKinds.end()){
        return ErrorKinds[this->m_kind];
    }
    return ErrorKinds[Error::Kind::Error];
}

// -*-
const Self& Error::reason(void) const{
    return this->m_reason;
}

// -*-
const Str& Error::message(void) const{ return this->m_msg; }

// -*-
Str Error::describe(void) const{
    std::stringstream stream;
    auto sym = Object(this->kind());
    stream << sym << ":\n";
    if(this->reason()){
        stream << *this->reason();
    }
    stream << this->m_msg;
    return stream.str();
}

// --------------
// -*- Result -*-
// --------------
// -*- value = nil, is_ok() == true
Result::Result() noexcept
: m_value{std::make_shared<Object>()}
, m_ok{true}{}

// -*- value = nil, is_ok() == true
Result::Result(const Self& self) noexcept
: m_value{self}
, m_ok{true}{}

// -*- value = nil, is_ok() == true
Result::Result(const Error& error) noexcept
: m_value{error}
, m_ok{false}{}

// -*- value = nil, is_ok() == true
Result::Result(const Symbol& sym, const Error& error) noexcept
: m_value{error}
, m_ok{false}{
    std::get<Error>(this->m_value).m_symbol = sym;
}

// -*-
Result::Result(const Result& result) noexcept
: m_value{result.m_value}
, m_ok{result.m_ok}{}


// -*-
Result::Result(Result&& result) noexcept
: m_value{std::move(result.m_value)}
, m_ok{std::move(result.m_ok)}{}

// -*-
Result& Result::operator=(const Result& result) noexcept{
    if(this != &result){
        this->m_value = result.m_value;
        this->m_ok = result.m_ok;
    }
    return *this;
}

// -*-
Result& Result::operator=(Result&& result) noexcept{
    if(this != &result){
        this->m_value = std::move(result.m_value);
        this->m_ok = std::move(result.m_ok);
    }
    return *this;
}

// -*-
Self Result::ok(void) const{
    Self self = nullptr;
    if(this->m_ok){
        self = std::get<Self>(this->m_value);
    }else{
        // Result is not an error-reference
        // return a new-error object
        auto result = Result(std::get<Error>(this->m_value));
        self = std::make_shared<Object>(result);
    }

    return self;
}

Error Result::err(void) const{
    Error myErr;
    if(!this->is_ok()){
        myErr = std::get<Error>(this->m_value);
    }else{
        // object is not an error, but and ok-value
        Self okVal = std::get<Self>(this->m_value);
        myErr = Error(Error::Kind::Error, okVal);
    }
    return myErr;
}

// -----------------
// -*- Structure -*-
// -----------------
// -*-
Structure::Structure(const Str& name) noexcept
: m_name{Symbol(name)}
, m_instance_name{Symbol("")}
, m_attributes{}
, m_properties{}
, m_methods{}
, m_object_attributes{}{}

// -*-
Structure::Structure(const Structure& structure) noexcept
: m_name{structure.m_name}
, m_instance_name{structure.m_instance_name}
, m_attributes{structure.m_attributes}
, m_properties{structure.m_properties}
, m_methods{structure.m_methods}
, m_object_attributes{structure.m_object_attributes}{}

// -*-
Structure::Structure(Structure&& structure) noexcept
: m_name{std::move(structure.m_name)}
, m_instance_name{std::move(structure.m_instance_name)}
, m_attributes{std::move(structure.m_attributes)}
, m_properties{std::move(structure.m_properties)}
, m_methods{std::move(structure.m_methods)}
, m_object_attributes{std::move(structure.m_object_attributes)}{}

// -*-
Structure& Structure::operator=(const Structure& structure) noexcept{
    if(this != &structure){
        this->m_name = structure.m_name;
        this->m_instance_name = structure.m_instance_name;
        this->m_attributes = structure.m_attributes;
        this->m_properties = structure.m_properties;
        this->m_methods = structure.m_methods;
        this->m_object_attributes = structure.m_object_attributes;
    }
    return *this;
}

Structure& Structure::operator=(Structure&& structure) noexcept{
    if(this != &structure){
        this->m_name = std::move(structure.m_name);
        this->m_instance_name = std::move(structure.m_instance_name);
        this->m_attributes = std::move(structure.m_attributes);
        this->m_properties = std::move(structure.m_properties);
        this->m_methods = std::move(structure.m_methods);
        this->m_object_attributes = std::move(structure.m_object_attributes);
    }
    return *this;
}

// -*-
Symbol Structure::name(void) const{
    return this->m_name;
}

// -*-
HashMap<Symbol, Self> Structure::attributes(void) const{
    return this->m_attributes;
}

// -*-
HashMap<Symbol, Self> Structure::properties(void) const{
    return this->m_properties;
}

// -*-
HashMap<Symbol, Self> Structure::object_attribues(void) const{
    return this->m_object_attributes;
}

// -*-
HashMap<Symbol, Self> Structure::methods(void) const{
    return this->m_methods;
}

// -*-
void Structure::define_initilizer(Self initfn){
    this->m_methods[Symbol("@init")] = initfn;
}

// -*-
void Structure::initialize(Args args){
    auto fn = this->m_methods[Symbol("@init")];
    auto fun = *fn;
    fun(args);
}

// -*-
void Structure::define_attribute(Self attr){
    if(!attr->is_tuple()){
        std::stringstream stream;
        stream << "Structure::define_attribute():\n";
        stream << "argument-type mismatch: expect a 'Tuple' but got ";
        stream << "'" << static_cast<Str>(attr->type()) << "'";
        throw std::runtime_error(stream.str());
    }
    auto len = static_cast<List>(*attr).size();
    if(len != 2){
        std::stringstream stream;
        stream << "Structure::define_attribute():\n";
        stream << "argument must be a tuple of two elements but got ";
        stream << "\na tuple with " << len << " elemenets";
        throw std::runtime_error(stream.str());
    }
    auto vec = static_cast<List>(*attr);
    auto name = vec[0];
    if(!name->is_symbol()){
        throw std::runtime_error(
            "Structure::define_attribute():\n"
            "attribute name is not an identifier"
        );
    }
    auto value = vec[1];
    this->m_attributes[static_cast<Symbol>(*name)] = value;
}

// -*-
void Structure::define_property(Self prop){
    if(!prop->is_tuple()){
        std::stringstream stream;
        stream << "Structure::define_property():\n";
        stream << "argument-type mismatch: expect a 'Tuple' but got ";
        stream << "'" << static_cast<Str>(prop->type()) << "'";
        throw std::runtime_error(stream.str());
    }
    auto len = static_cast<List>(*prop).size();
    if(len != 2){
        std::stringstream stream;
        stream << "Structure::define_property():\n";
        stream << "argument must be a tuple of two elements but got ";
        stream << "\na tuple with " << len << " elemenets";
        throw std::runtime_error(stream.str());
    }
    auto vec = static_cast<List>(*prop);
    auto name = vec[0];
    if(!name->is_symbol()){
        throw std::runtime_error(
            "Structure::define_property():\n"
            "property name is not an identifier"
        );
    }
    auto value = vec[1];
    this->m_properties[static_cast<Symbol>(*name)] = value;
}

// -*-
void Structure::define_object_attribute(Self attr){
    if(!attr->is_tuple()){
        std::stringstream stream;
        stream << "Structure::define_object_attribute():\n";
        stream << "argument-type mismatch: expect a 'Tuple' but got ";
        stream << "'" << static_cast<Str>(attr->type()) << "'";
        throw std::runtime_error(stream.str());
    }
    auto len = static_cast<List>(*attr).size();
    if(len != 2){
        std::stringstream stream;
        stream << "Structure::define_object_attribute():\n";
        stream << "argument must be a tuple of two elements but got ";
        stream << "\na tuple with " << len << " elemenets";
        throw std::runtime_error(stream.str());
    }
    auto vec = static_cast<List>(*attr);
    auto name = vec[0];
    if(!name->is_symbol()){
        throw std::runtime_error(
            "Structure::define_object_attribute():\n"
            "attribute name is not an identifier"
        );
    }
    auto value = vec[1];
    this->m_object_attributes[static_cast<Symbol>(*name)] = value;
}

// -*-
void Structure::define_method(Self method){
    if(!method->is_function()){
        std::stringstream stream;
        stream << "Structure::define_method():\n";
        stream << "argument-type mismatch: expect a 'Function' but got ";
        stream << "'" << static_cast<Str>(method->type()) << "'";
        throw std::runtime_error(stream.str());
    }
    auto ast = static_cast<Ast>(*method);
    auto fnast = dynamic_cast<FunDefExprAst*>(ast.get());
    auto name = fnast->name();
    this->m_object_attributes[name] = std::move(method);
}

// -*-
void Structure::delete_attribute(const Str& key){
    Symbol sym(key);
    if(this->m_attributes.find(sym)==this->m_attributes.end()){
        std::stringstream stream;
        stream << "undefined attribute: '" << key << "'\n";
        stream << static_cast<Str>(this->m_name) << " does not have ";
        stream << "attribute '" << key << "'";
        throw std::runtime_error(stream.str());
    }
    this->m_attributes.erase(sym);
}

// -*-
void Structure::delete_property(const Str& key){
    Symbol sym(key);
    if(this->m_properties.find(sym)==this->m_properties.end()){
        std::stringstream stream;
        stream << "undefined property: '" << key << "'\n";
        stream << static_cast<Str>(this->m_name) << " does not have ";
        stream << "property '" << key << "'";
        throw std::runtime_error(stream.str());
    }
    this->m_properties.erase(sym);
}

// -*-
void Structure::delete_object_attribute(const Str& key){
    Symbol sym(key);
    if(this->m_object_attributes.find(sym)==this->m_object_attributes.end()){
        std::stringstream stream;
        stream << "undefined object attribute: '" << key << "'\n";
        stream << static_cast<Str>(this->m_instance_name);
        stream << " does not have attribute '" << key << "'";
        throw std::runtime_error(stream.str());
    }
    this->m_object_attributes.erase(sym);
}

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