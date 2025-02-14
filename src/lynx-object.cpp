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

// -----------
// -*- Set -*-
// ------------
/**
 * @brief Compute the hash-value of 'key'
 * 
 * Builitn hashable objects include boolean, integer, tuple, string, and symbol.
 * 
 * @param lhs 
 * @return usize 
 */
usize Set::Hasher::operator()(const Self& key){
    Args args = {key};
    Self self = key->__hash__(args);
    auto hashVal = static_cast<usize>(static_cast<i64>(*self));
    return hashVal;
}

/**
 * @brief Implement Set-key '==' comparator.
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
bool Set::Equal::operator()(const Self& lhs, const Self& rhs){
    return (*lhs) == (*rhs);
}

/**
 * @brief Construct a Set empty object.
 * 
 */
Set::Set() noexcept: m_data{}{}

/**
 * @brief Construct a new Set:: Set object
 * 
 * Construct a Set object from iterable (assumed to iterable) object.
 * 
 * Each item in the iterable is assumed to be hashable.
 * @param iterable 
 */
Set::Set(Self iterable) noexcept: Set{}{
    Iterator iterator = std::get<Iterator>(iterable->m_value);
    while(static_cast<bool>(iterator->done())){
        Self key = iterator->next();
        this->m_data.insert(key);
    }
}

/*
Set::Set(const Set& hset) noexcept{}
Set::Set(Set&& hset) noexcept{}
Set& Sef::operator=(const Set& hset){}
Set& Sef::operator=(Set&& hset){}
Set::~Set(){}
bool Set::contains(const Self& key) const{}
usize Set::len(void) const{}
void Set::insert(const Self& item){}
bool Set::is_disjoint(const Set& hset) const{}
bool Set::is_superset(const Set& hset) const{}
bool Set::is_subset(const Set& hset) const{}
void Set::remove(const Self& item){}
void Set::clear(void){}
Set::UserSet Set::data(void) const{}

Set operator+(const Set& lhs, const Set& rhs){}
Set operator-(const Set& lhs, const Set& rhs){}
Set operator|(const Set& lhs, const Set& rhs){}
Set operator^(const Set& lhs, const Set& rhs){}
Set operator&(const Set& lhs, const Set& rhs){}
*/


// ------------
// -*- Dict -*-
// ------------

/*
usize Dict::Hasher::operator()(const Self& lhs){}
bool Dict::Equal::operator()(const Self& lhs, const Self& rhs){}

Dict::Dict() noexcept{}
Dict::Dict(Self iterable) noexcept{}
Dict::Dict(const Dict& dict) noexcept{}
Dict::Dict(Dict&& dict) noexcept{}
Dict& Dict::operator=(const Dict& dict) noexcept{}
Dict& Dict::operator=(Dict&& dict) noexcept{}
Dict::~Dict(){}
bool Dict::contains(const Self& key) const{}
usize Dict::len(void) const{}
void Dict::remove(const Self& key){}
Self Dict::popitem(const Self& key){}
Self& Dict::operator[](const Self& key){}
const Self& Dict::operator[](const Self& key) const{}
Self Dict::values(void) const{}
Self Dict::keys(void) const{}
Self Dict::items(void) const{}
Self Dict::update(const Self& key, const Self value){}
Dict::UserDict Dict::data(void) const{}
Self operator+(const Dict& lhs, const Dict& rhs){}
*/

// --------------
// -*- Object -*-
// --------------
/**
 * @brief Construct a new Object:: Object object
 * 
 * Create Nil object.
 */
Object::Object() noexcept
: m_kind{Object::Kind::None}
, m_value{Nil{}}
, m_name{Symbol("")}
, m_is_version{false}
, m_newtype{false}
, m_constant{false}
, m_fixed_type{false}{}

/**
 * @brief Construct a new Object:: Object object
 * 
 * Create a boolean object
 * 
 * @param val 
 */
Object::Object(bool val) noexcept
: m_kind{Object::Kind::Bool}
, m_value{val}
, m_name{Symbol("")}
, m_is_version{false}
, m_newtype{false}
, m_constant{false}
, m_fixed_type{false}{}

/**
 * @brief Construct a new Object:: Object object
 * 
 * Create an integer object
 * 
 * @param num 
 */
Object::Object(i64 num) noexcept
: m_kind{Object::Kind::Integer}
, m_value{num}
, m_name{Symbol("")}
, m_is_version{false}
, m_newtype{false}
, m_constant{false}
, m_fixed_type{false}{}

/**
 * @brief Construct a new Object:: Object object
 * 
 * Create a Float object.
 * @param num 
 */
Object::Object(f64 num) noexcept
: m_kind{Object::Kind::Float}
, m_value{num}
, m_name{Symbol("")}
, m_is_version{false}
, m_newtype{false}
, m_constant{false}
, m_fixed_type{false}{}

/**
 * @brief Construct a new Object:: Object object
 * 
 * Create a Complex-number object.
 * @param z 
 */
Object::Object(Complex z) noexcept
: m_kind{Object::Kind::Complex}
, m_value{z}
, m_name{Symbol("")}
, m_is_version{false}
, m_newtype{false}
, m_constant{false}
, m_fixed_type{false}{}


/**
 * @brief Construct a new Object:: Object object
 * 
 * Create Symbol object.
 * @param sym 
 */
Object::Object(Symbol sym) noexcept
: m_kind{Object::Kind::Sym}
, m_value{sym}
, m_name{Symbol("")}
, m_is_version{false}
, m_newtype{false}
, m_constant{false}
, m_fixed_type{false}{}

/**
 * @brief Construct a new Object:: Object object
 * 
 * Create a String object
 * @param str 
 */
Object::Object(Str str) noexcept
: m_kind{Object::Kind::String}
, m_value{str}
, m_name{Symbol("")}
, m_is_version{false}
, m_newtype{false}
, m_constant{false}
, m_fixed_type{false}{}

/**
 * @brief Construct a new Object:: Object object
 * 
 * Create a Tuple or List object.
 * @param kind  Object::Kind::Tuple or Object::Kind::List
 * @param data 
 */
Object::Object(Object::Kind kind, List data) noexcept
: m_kind{kind}
, m_value{data}
, m_name{Symbol("")}
, m_is_version{false}
, m_newtype{false}
, m_constant{false}
, m_fixed_type{false}{}

/**
 * @brief Construct a new Object:: Object object
 * 
 * Create a HashSet object.
 * @param data 
 */
Object::Object(Set data) noexcept
: m_kind{Object::Kind::Set}
, m_value{data}
, m_name{Symbol("")}
, m_is_version{false}
, m_newtype{false}
, m_constant{false}
, m_fixed_type{false}{}


/**
 * @brief Construct a new Object:: Object object
 * 
 * Create a HashMap object;
 * @param data 
 */
Object::Object(Dict data) noexcept
: m_kind{Object::Kind::Dict}
, m_value{data}
, m_name{Symbol("")}
, m_is_version{false}
, m_newtype{false}
, m_constant{false}
, m_fixed_type{false}{}


/**
 * @brief Construct a new Object:: Object object
 * 
 * Create a Structure object.
 * @param klass 
 */
Object::Object(Structure klass) noexcept
: m_kind{Object::Kind::Struct}
, m_value{klass}
, m_name{Symbol("")}
, m_is_version{false}
, m_newtype{false}
, m_constant{false}
, m_fixed_type{false}{}


/**
 * @brief Construct a new Object:: Object object
 * 
 * Create an Iterator object.
 * @param iter 
 */
Object::Object(Iterator iter) noexcept
: m_kind{Object::Kind::Iter}
, m_value{iter}
, m_name{Symbol("")}
, m_is_version{false}
, m_newtype{false}
, m_constant{false}
, m_fixed_type{false}{}


/**
 * @brief Construct a new Object:: Object object
 * 
 * Create a builtin function object.
 * 
 * @param name 
 * @param cfun 
 */
Object::Object(const Str& name, CFun cfun) noexcept
: m_kind{Object::Kind::None}
, m_value{cfun}
, m_name{Symbol(name)}
, m_is_version{false}
, m_newtype{false}
, m_constant{false}
, m_fixed_type{false}{}


/**
 * @brief Construct a new Object:: Object object
 * 
 * Create a user-defined callable, i.e Lambda, Function, or Macro
 * @param ast 
 */
Object::Object(Ast ast) noexcept
: m_kind{Object::Kind::None}
, m_value{ast}
, m_name{Symbol("")}
, m_is_version{false}
, m_newtype{false}
, m_constant{false}
, m_fixed_type{false}{
    if(ast->kind() == AstKind::LambdaDefExpr){
        this->m_kind = Object::Kind::Lambda;
    }else if(ast->kind() == AstKind::FunDefExpr){
        this->m_kind = Object::Kind::Fun;
    }else if(ast->kind() == AstKind::MacroDefExpr){
        this->m_kind = Object::Kind::Macro;
    }
}

/**
 * @brief Construct a new Object:: Object object
 * 
 * Create a Result object.
 * @param result 
 */
Object::Object(Result result) noexcept
: m_kind{Object::Kind::Result}
, m_value{result}
, m_name{Symbol("")}
, m_is_version{false}
, m_newtype{false}
, m_constant{false}
, m_fixed_type{false}{}

/**
 * @brief Construct a new Object:: Object object
 * 
 * Contruct a copy of object.
 * @param obj 
 */
Object::Object(const Object& obj) noexcept
: m_kind{obj.m_kind}
, m_value{obj.m_value}
, m_name{obj.m_name}
, m_is_version{obj.m_is_version}
, m_newtype{obj.m_newtype}
, m_constant{obj.m_constant}
, m_fixed_type{obj.m_fixed_type}{}

/**
 * @brief Construct a new Object:: Object object
 * 
 * Move object to a new target.
 * @param obj 
 */
Object::Object(Object&& obj) noexcept
: m_kind{std::move(obj.m_kind)}
, m_value{std::move(obj.m_value)}
, m_name{std::move(obj.m_name)}
, m_is_version{std::move(obj.m_is_version)}
, m_newtype{std::move(obj.m_newtype)}
, m_constant{std::move(obj.m_constant)}
, m_fixed_type{std::move(obj.m_fixed_type)}{}

/**
 * @brief Create a copy of object.
 * 
 * @param obj 
 * @return Object& 
 */
Object& Object::operator=(const Object& obj) noexcept{
    if(this != &obj){
        this->m_kind = obj.m_kind;
        this->m_value = obj.m_value;
        this->m_name = obj.m_name;
        this->m_is_version = obj.m_is_version;
        this->m_newtype = obj.m_newtype;
        this->m_constant = obj.m_constant;
        this->m_fixed_type = obj.m_fixed_type;
    }

    return *this;
}

/**
 * @brief Move object to a new target.
 * 
 * @param obj 
 * @return Object& 
 */
Object& Object::operator=(Object&& obj) noexcept{
    if(this != &obj){
        this->m_kind = std::move(obj.m_kind);
        this->m_value = std::move(obj.m_value);
        this->m_name = std::move(obj.m_name);
        this->m_is_version = std::move(obj.m_is_version);
        this->m_newtype = std::move(obj.m_newtype);
        this->m_constant = std::move(obj.m_constant);
        this->m_fixed_type = std::move(obj.m_fixed_type);
    }

    return *this;
}

Object::~Object(){
    if(this->is_callable() && !this->is_builtin()){
        Ast ast = std::get<Ast>(this->m_value);
        ast.reset();
    }else if(this->is_tuple() || this->is_list()){
        List vec = std::get<List>(this->m_value);
        for(auto& item: vec){ item.reset();}
    }else if(this->is_hashset()){
        //! @todo
        // std::get<HSet>(this->m_value).clear();
    }else if(this->is_hashmap()){
        //! @todo
        // std::get<HMap>(this->m_value).clear();
    }
}

/**
 * @brief Convert object a boolean value.
 * 
 * @return true 
 * @return false 
 */
Object::operator bool(){
    if(this->is_bool()){ return std::get<bool>(this->m_value); }
    else if(this->is_integer()){
        auto num = std::get<i64>(this->m_value);
        return (num == 0);
    }else if(this->is_float()){
        auto num = std::get<f64>(this->m_value);
        return (num == 0.0);
    }else if(this->is_nil()){ return false; }
    return true;
}

/**
 * @brief Convert object into an integer
 * 
 * @return i64 
 */
Object::operator i64(){
    i64 num{};
    if(this->is_integer()){
        num = std::get<i64>(this->m_value);
    }else if(this->is_bool()){
        num = static_cast<i64>(std::get<bool>(this->m_value));
    }else if(this->is_float()){
        num = static_cast<i64>(std::get<f64>(this->m_value));
    }else if(this->is_string()){
        Str numstr = std::get<Str>(this->m_value);
        usize pos = 0;
        if(numstr.find('.')==std::string::npos){
            try{
                num = std::stol(numstr, &pos);
                if(numstr.length() != pos){
                    throw std::runtime_error("invalid numeric string");
                }
            }catch(const std::invalid_argument& err){
                throw std::runtime_error("invalid numeric string");
            }catch(const std::out_of_range& err){
                throw std::runtime_error("integer value overflow");
            }catch(const std::runtime_error& err){
                throw std::runtime_error(err.what());
            }catch(...){
                throw std::runtime_error("unable to convert object into number");
            }
        }else if(numstr.find('.')!=std::string::npos){
            f64 fval{};
            try{
                fval = std::stod(numstr, &pos);
                if(numstr.length() != pos){
                    throw std::runtime_error("invalid numeric string");
                }
            }catch(const std::invalid_argument& err){
                throw std::runtime_error("invalid numeric string");
            }catch(const std::out_of_range& err){
                throw std::runtime_error("floating-point value overflow");
            }catch(const std::runtime_error& err){
                throw std::runtime_error(err.what());
            }catch(...){
                throw std::runtime_error("unable to convert object into number");
            }
            num = static_cast<i64>(fval);
        }
    }else{
        throw std::runtime_error("unable to convert object into number");
    }
    return num;
}

/**
 * @brief Convert object to a floating-point value.
 * 
 * @return f64 
 */
Object::operator f64(){
    f64 num{};
    if(this->is_integer()){
        num = static_cast<f64>(std::get<i64>(this->m_value));
    }else if(this->is_bool()){
        num = static_cast<f64>(static_cast<i64>(std::get<bool>(this->m_value)));
    }else if(this->is_float()){
        num = std::get<f64>(this->m_value);
    }else if(this->is_string()){
        Str numstr = std::get<Str>(this->m_value);
        usize pos = 0;
        if(numstr.find('.')==std::string::npos){
            i64 inum{};
            try{
                inum = std::stol(numstr, &pos);
                if(numstr.length() != pos){
                    throw std::runtime_error("invalid numeric string");
                }
            }catch(const std::invalid_argument& err){
                throw std::runtime_error("invalid numeric string");
            }catch(const std::out_of_range& err){
                throw std::runtime_error("integer value overflow");
            }catch(const std::runtime_error& err){
                throw std::runtime_error(err.what());
            }catch(...){
                throw std::runtime_error("unable to convert object into number");
            }
            num = static_cast<f64>(inum);
        }else if(numstr.find('.')!=std::string::npos){
            try{
                num = std::stod(numstr, &pos);
                if(numstr.length() != pos){
                    throw std::runtime_error("invalid numeric string");
                }
            }catch(const std::invalid_argument& err){
                throw std::runtime_error("invalid numeric string");
            }catch(const std::out_of_range& err){
                throw std::runtime_error("floating-point value overflow");
            }catch(const std::runtime_error& err){
                throw std::runtime_error(err.what());
            }catch(...){
                throw std::runtime_error("unable to convert object into number");
            }
        }
    }else{
        throw std::runtime_error("unable to convert object into number");
    }
    return num;
}

/**
 * @brief Convert object into a complex-number value.
 * 
 * @return Complex 
 */
Object::operator Complex(){
    Complex z{};
    if(this->is_integer()){
        f64 x = static_cast<f64>(std::get<i64>(this->m_value));
        z = Complex(x, 0.0);
    }else if(this->is_float()){
        f64 x = std::get<f64>(this->m_value);
        z = Complex(x, 0.0);
    }else if(this->is_complex()){
        z = std::get<Complex>(this->m_value);
    }else{
        throw std::runtime_error("unable to convert object into a complex-number");
    }
    return z;
}

/**
 * @brief Convert object to a Symbol-value
 * 
 * @return Symbol 
 */
Object::operator Symbol(){
    if(!this->is_symbol()){
        throw std::runtime_error("unable to extract a symbol-value from object");
    }
    return std::get<Symbol>(this->m_value);
}

/**
 * @brief Convert object to String-value
 * 
 * @return Str 
 */
Object::operator Str(){
    Str str{};
    if(this->is_nil()){
        str = Str{"nil"};
    }else if(this->is_bool()){
        bool flag = std::get<bool>(this->m_value);
        if(flag){ str = Str{"true"}; }
        else{ str = Str{"false"}; }
    }else if(this->is_integer()){
        std::stringstream stream;
        stream << std::get<i64>(this->m_value);
        str = stream.str();
    }else if(this->is_float()){
        std::stringstream stream;
        stream << std::get<f64>(this->m_value);
        str = stream.str();
    }else if(this->is_complex()){
        Complex z = std::get<Complex>(this->m_value);
        std::stringstream stream;
        stream << "Complex(" << z.real() << ", " << z.imag() << ")";
        str = stream.str();
    }else if(this->is_symbol()){
        Symbol sym = std::get<Symbol>(this->m_value);
        str = sym.m_data;
    }else if(this->is_string()){
        str = std::get<Str>(this->m_value);
    }else{
        Args args{};
        args.emplace_back(std::make_shared<Object>(*this));
        Self self = this->__str__(args);
        str = static_cast<Str>(*self);
    }

    return str;
}

/**
 * @brief Convert object into a list.
 * 
 * This operation is valid for String, Tuple, List, HashSet, and HashMap.
 * 
 * @return List 
 */
Object::operator List(){
    List xs{};
    if(this->is_tuple() || this->is_list()){
        xs = std::get<List>(this->m_value);
    }else if(this->is_string()){
        Str str = std::get<Str>(this->m_value);
        for(auto iter = str.cbegin(); iter != str.cend(); iter++){
            xs.emplace_back(std::make_shared<Object>(Str{*iter}));
        }
    }else if(this->is_hashset()){
        //! @todo
        // HSet hset = std::get<HSet>(this->m_value);
        // for(auto iter = hset.cbegin(); iter != hset.cend(); iter++){
        //     Self self = *iter;
        //     xs.emplace_back(std::make_shared<Object>(*self));
        // }
    }else if(this->is_hashmap()){
        //! @todo
        // HMap hmap = std::get<HMap>(this->m_value);
        // for(auto iter = hmap.cbegin(); iter != hmap.cend(); iter++){
        //     Self key = iter->first;
        //     Self val = iter->second;
        //     Vec<Self> item{};
        //     item.push_back(key);
        //     item.push_back(val);
        //     xs.emplace_back(std::make_shared<Object>(Object::Kind::Tuple, item));
        // }
    }else if(this->is_iterable()){
        Iterator iterator = std::get<Iterator>(this->m_value);
        while(!static_cast<bool>(iterator->done())){
            Self self = iterator->next();
            xs.emplace_back(std::make_shared<Object>(*self));
        }
    }else{
        Args args{};
        args.emplace_back(std::make_shared<Object>(*this));
        Self self = this->__list__(args);
        xs.emplace_back(std::make_shared<Object>(*self));
    }
    return xs;
}

/**
 * @brief Convert object into a HSet object.
 * 
 * @return HSet 
 */
Object::operator HSet(){
    HSet hset{};

    return hset;
}


/*
Object::operator HMap(){}
Object::operator CFun(){}
Object::operator Result(){}
Object::operator Ast(){}

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