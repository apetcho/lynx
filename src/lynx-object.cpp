#include "lynx.hpp"
#include<algorithm>
#include<cctype>
#include<cstdlib>

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
HashMap<Str, Self> Structure::attributes(void) const{
    return this->m_attributes;
}

// -*-
HashMap<Str, Self> Structure::properties(void) const{
    return this->m_properties;
}

// -*-
HashMap<Str, Self> Structure::object_attribues(void) const{
    return this->m_object_attributes;
}

// -*-
HashMap<Str, Self> Structure::methods(void) const{
    return this->m_methods;
}

// -*-
void Structure::define_initilizer(Self initfn){
    this->m_methods["@init"] = initfn;
}

// -*-
void Structure::initialize(Args args){
    auto fn = this->m_methods["@init"];
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
    this->m_attributes[static_cast<Str>(*name)] = value;
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
    this->m_properties[static_cast<Str>(*name)] = value;
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
    this->m_object_attributes[static_cast<Str>(*name)] = value;
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
    auto name = fnast->name().str();
    this->m_object_attributes[name] = std::move(method);
}

// -*-
void Structure::delete_attribute(const Str& key){
    if(this->m_attributes.find(key)==this->m_attributes.end()){
        std::stringstream stream;
        stream << "undefined attribute: '" << key << "'\n";
        stream << static_cast<Str>(this->m_name) << " does not have ";
        stream << "attribute '" << key << "'";
        throw std::runtime_error(stream.str());
    }
    this->m_attributes.erase(key);
}

// -*-
void Structure::delete_property(const Str& key){
    if(this->m_properties.find(key)==this->m_properties.end()){
        std::stringstream stream;
        stream << "undefined property: '" << key << "'\n";
        stream << static_cast<Str>(this->m_name) << " does not have ";
        stream << "property '" << key << "'";
        throw std::runtime_error(stream.str());
    }
    this->m_properties.erase(key);
}

// -*-
void Structure::delete_object_attribute(const Str& key){
    if(this->m_object_attributes.find(key)==this->m_object_attributes.end()){
        std::stringstream stream;
        stream << "undefined object attribute: '" << key << "'\n";
        stream << static_cast<Str>(this->m_instance_name);
        stream << " does not have attribute '" << key << "'";
        throw std::runtime_error(stream.str());
    }
    this->m_object_attributes.erase(key);
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
usize Set::Hasher::operator()(const Self& key) const {
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
bool Set::Equal::operator()(const Self& lhs, const Self& rhs) const{
    return ((*lhs) == (*rhs));
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
    while(!static_cast<bool>(iterator->done())){
        Self key = iterator->next();
        this->m_data.insert(key);
    }
}

/**
 * @brief Construct a new Set:: Set object
 * 
 * @param hset 
 */
Set::Set(const Set& hset) noexcept: m_data{hset.m_data}{}

Set::Set(Set&& hset) noexcept: m_data{std::move(hset.m_data)}{}

Set& Set::operator=(const Set& hset) noexcept {
    if(this != &hset){ this->m_data = hset.m_data; }
    return *this;
}

Set& Set::operator=(Set&& hset) noexcept {
    if(this != &hset){ this->m_data = std::move(hset.m_data);}
    return *this;
}

Set::~Set(){
    for(auto keyIter=this->m_data.begin(); keyIter != this->m_data.end(); keyIter++){
        Self self = *keyIter;
        self.reset();
    }
}

/**
 * @brief Check whether this Set-object contains 'key'.
 * 
 * @param key 
 * @return true 
 * @return false 
 */
bool Set::contains(const Self& key) const{
    if(this->m_data.find(key) != this->m_data.end()){ return true; }
    return false;
}

// -*-
usize Set::len(void) const{ return this->m_data.size(); }

// -*-
void Set::insert(const Self& item){
    this->m_data.insert(item);
}

// -*-
bool Set::is_disjoint(const Set& hset) const{
    for(auto key=this->m_data.begin(); key != this->m_data.end(); key++){
        if(hset.contains(*key)){ return false; }
    }
    for(auto key=hset.m_data.begin(); key != hset.m_data.end(); key++){
        if(this->contains(*key)){ return false;}
    }

    return true;
}

// -*-
bool Set::is_superset(const Set& hset) const{
    if(this->len() < hset.len()){ return false; }
    for(auto key=hset.m_data.cbegin(); key != hset.m_data.cend(); key++){
        if(!this->contains(*key)){ return false; }
    }

    return true;
}

// -*-
bool Set::is_subset(const Set& hset) const{
    if(hset.len() < this->len()){ return false; }
    for(auto key=this->m_data.cbegin(); key != this->m_data.cend(); key++){
        if(!hset.contains(*key)){ return false; }
    }

    return true;
}

// -*-
void Set::remove(const Self& item){
    if(this->contains(item)){
        this->m_data.erase(item);
    }
}

// -*-
void Set::clear(void){ this->m_data.clear(); }

// -*-
Set::UserSet Set::data(void) const{ return this->m_data; }

// -*-
Set operator+(const Set& lhs, const Set& rhs){
    Set hset{};
    for(auto key=lhs.m_data.cbegin(); key != lhs.m_data.cend(); key++){
        hset.insert(*key);
    }
    for(auto key=rhs.m_data.cbegin(); key != rhs.m_data.cend(); key++){
        hset.insert(*key);
    }

    return hset;
}

Set operator-(const Set& lhs, const Set& rhs){
    Set hset{};
    for(auto key=lhs.m_data.cbegin(); key != lhs.m_data.cend(); key++){
        hset.insert(*key);
    }
    for(auto key=rhs.m_data.cbegin(); key != rhs.m_data.cend(); key++){
        if(hset.contains(*key)){ hset.remove(*key); }
    }

    return hset;
}

// -*-
Set operator|(const Set& lhs, const Set& rhs){
    Set hset = lhs + rhs;
    return hset;
}

Set operator^(const Set& lhs, const Set& rhs){
    Set hset{};
    for(auto key=lhs.m_data.cbegin(); key != lhs.m_data.cend(); key++){
        if(!rhs.contains(*key)){ hset.insert(*key); }
    }
    for(auto key=rhs.m_data.cbegin(); key != rhs.m_data.cend(); key++){
        if(!lhs.contains(*key)){ hset.insert(*key); }
    }

    return hset;
}

/*
Set operator&(const Set& lhs, const Set& rhs){}
*/


// ------------
// -*- Dict -*-
// ------------
// -*-
usize Dict::Hasher::operator()(const Self& lhs) const{
    Self self = lhs->__hash__(Args{lhs});
    return static_cast<usize>(std::get<i64>(self->m_value));
}

// -*-
bool Dict::Equal::operator()(const Self& lhs, const Self& rhs) const{
    return ((*lhs) == (*rhs));
}

/**
 * @brief Construct a new Dict:: Dict object
 * 
 */
Dict::Dict() noexcept: m_data{}{}

/**
 * @brief Construct a new Dict:: Dict object
 * 
 * Construct a Dict from an iterable.
 * 
 * Assumes iterable is a ListIterator or TupleIterator. Each elements
 * of the iterator it expected to be a Tuple of two items in the form (key, value) pair.
 * 
 * @param iterable 
 */
Dict::Dict(Self iterable) noexcept: Dict{}{
    Iterator iterator = std::get<Iterator>(iterable->m_value);
    while(!static_cast<bool>(iterator->done())){
        Self self = iterator->next();
        // assume self is a pair: (key, value)
        List vec = static_cast<List>(*self);
        auto key = vec[0];
        auto val = vec[1];
        this->m_data[key] = val;
    }
}

// -*-
Dict::Dict(const Dict& dict) noexcept: m_data{dict.m_data}{}

// -*-
Dict::Dict(Dict&& dict) noexcept: m_data{std::move(dict.m_data)}{}

// -*-
Dict& Dict::operator=(const Dict& dict) noexcept{
    if(this != &dict){ this->m_data = dict.m_data; }
    return *this;
}

// -*-
Dict& Dict::operator=(Dict&& dict) noexcept{
    if(this != &dict){ this->m_data = std::move(dict.m_data); }
    return *this;
}

// -*-
Dict::~Dict(){
    this->m_data.clear();
}

// -*-
bool Dict::contains(const Self& key) const{
    if(this->m_data.find(key) != this->m_data.end()){
        return true;
    }
    return false;
}

// -*-
usize Dict::len(void) const{
    return this->m_data.size();
}

void Dict::remove(const Self& key){
    if(this->contains(key)){
        this->m_data.erase(key);
    }
}

/**
 * @brief Remove the entry with 'key'.
 * 
 * If 'key' is not in 'Dict' then return Error object
 * 
 * @param key 
 * @return Self 
 */
Self Dict::popitem(const Self& key){
    Self self = nullptr;
    if(this->contains(key)){
        auto entry = this->m_data.find(key);
        List xs{}; // = {*entry->first, *entry->second};
        xs.push_back(std::make_shared<Object>(*entry->first));
        xs.push_back(std::make_shared<Object>(*entry->second));
        Self obj = std::make_shared<Object>(Object::Kind::Tuple, xs);
        self = std::make_shared<Object>(Result(obj));
        this->m_data.erase(key);
    }else{
        // key not in dict
        Error err = Error(Error::Kind::KeyError, "key not found");
        self = std::make_shared<Object>(Result(err));
    }
    return std::move(self);
}

/**
 * @brief Add (key, value) entry of Dict
 * 
 * @param key 
 * @return Self& 
 */
Self& Dict::operator[](const Self& key){
    if(this->contains(key)){
        return this->m_data[key];
    }
    // -*- object not found. We create a new entry with 'nil' value
    this->m_data[key] = std::make_shared<Object>(); // nil object
    return this->m_data[key];
}

/**
 * @brief Get the value of entry with key.
 * 
 * If key not found return Error value with KeyError symbol and message.
 * @param key 
 * @return const Self& 
 */
const Self& Dict::operator[](const Self& key) const{
    if(this->contains(key)){
        auto entry = this->m_data.find(key);
        return entry->second;
    }
    throw std::runtime_error("key not found");
}

/**
 * @brief Return a list of value entries.
 * 
 * @return Self 
 */
Self Dict::values(void) const{
    List vec{};
    for(auto entry: this->m_data){
        vec.push_back(std::make_shared<Object>(*entry.second));
    }
    return std::make_shared<Object>(Object::Kind::Vector, vec);
}

/**
 * @brief Return a list of keys in the dictionary.
 * 
 * @return Self 
 */
Self Dict::keys(void) const{
    List vec{};
    for(auto entry: this->m_data){
        vec.push_back(std::make_shared<Object>(*entry.first));
    }
    return std::make_shared<Object>(Object::Kind::Vector, vec);
}

/**
 * @brief Return a list (key, value) pairs
 * 
 * @return Self 
 */
Self Dict::items(void) const{
    List vec{};
    for(auto entry: this->m_data){
        List xs{};
        xs.push_back(std::make_shared<Object>(*entry.first));
        xs.push_back(std::make_shared<Object>(*entry.second));
        auto item = std::make_shared<Object>(Object::Kind::Tuple, xs);
        vec.push_back(std::move(item));
    }

    return std::make_shared<Object>(Object::Kind::Vector, vec);
}


/**
 * @brief Update an existing entry of key with new values. 
 * 
 * If 'key' is not found in the dictionary, then return an Error object.
 * 
 * @param key 
 * @param value 
 * @return Self 
 */
Self Dict::update(const Self& key, const Self& value){
    if(this->contains(key)){
        auto old = this->m_data[key];
        this->m_data[key] = value;
        return std::move(old);
    }
    Error err = Error(Error::Kind::KeyError, "key not found");
    return std::make_shared<Object>(Result(err));
}

/**
 * @brief Concatenate two dictionaries.
 * 
 * @param lhs 
 * @param rhs 
 * @return Self 
 */
Self operator+(const Dict& lhs, const Dict& rhs){
    Dict dict{};
    for(auto entry: lhs.m_data){
        dict.m_data[entry.first] = entry.second;
    }
    for(auto entry: rhs.m_data){
        dict.m_data[entry.first] = entry.second;
    }
    return std::make_shared<Object>(dict);
}

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
, m_usertype{false}
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
, m_usertype{false}
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
, m_usertype{false}
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
, m_usertype{false}
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
, m_usertype{false}
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
, m_usertype{false}
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
, m_usertype{false}
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
, m_usertype{false}
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
, m_usertype{false}
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
, m_usertype{false}
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
, m_usertype{false}
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
, m_usertype{false}
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
Object::Object(const Str& name, CFun cfun, int argc) noexcept
: m_kind{Object::Kind::None}
, m_value{cfun}
, m_name{Symbol(name)}
, m_argc{argc}
, m_is_version{false}
, m_usertype{false}
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
, m_usertype{false}
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
, m_usertype{false}
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
, m_usertype{obj.m_usertype}
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
, m_usertype{std::move(obj.m_usertype)}
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
        this->m_usertype = obj.m_usertype;
        this->m_constant = obj.m_constant;
        this->m_fixed_type = obj.m_fixed_type;
    }

    return *this;
}

// -*-
int Object::argc(void) const{
    if(this->is_lambda()){
        auto ast = std::get<Ast>(this->m_value);
        auto lambda = reinterpret_cast<LambdaDefExprAst*>(ast.get());
        return lambda->argc();
    }else if(this->is_macro()){
        auto ast = std::get<Ast>(this->m_value);
        auto macro = reinterpret_cast<MacroDefExprAst*>(ast.get());
        return macro->argc();
    }else if(this->is_function()){
        auto ast = std::get<Ast>(this->m_value);
        auto fun = reinterpret_cast<FunDefExprAst*>(ast.get());
        return fun->argc();
    }
    return this->m_argc;
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
        this->m_usertype = std::move(obj.m_usertype);
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
        Self self = this->__string__(args);
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
 * @brief Convert object into a Set object.
 * 
 * @return Set 
 */
Object::operator Set(){
    Set hset{};
    hset = std::get<Set>(this->m_value);
    return hset;
}


/**
 * @brief Convert object into a Dict object.
 * 
 * @return Dict 
 */
Object::operator Dict(){
    return std::get<Dict>(this->m_value);
}

// -*-
Object::operator CFun(){
    return std::get<CFun>(this->m_value);
}

// -*-
Object::operator Result(){
    return std::get<Result>(this->m_value);
}

// -*-
Object::operator Ast(){
    return std::get<Ast>(this->m_value);
}

/**
 * @brief Apply logical not operator on object.
 * 
 * @return Object& 
 */
Object& Object::operator!(){
    auto val = static_cast<bool>(*this);
    this->m_value = val;
    this->m_kind = Object::Kind::Bool;
    return *this;
}

/**
 * @brief Implement logical 'and' (&&) operation on Object.
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator&&(const Object& lhs, const Object& rhs){
    Object x = lhs;
    Object y = rhs;
    return Object((static_cast<bool>(x) && static_cast<bool>(y)));
}

/**
 * @brief Implement logical 'or' (||) operation on Object.
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator||(const Object& lhs, const Object& rhs){
    Object x = lhs;
    Object y = rhs;
    return Object((static_cast<bool>(x) || static_cast<bool>(y)));
}

/**
 * @brief Implement bitwise 'not' (~) operation on Object.
 * 
 * Assume object is an integer.
 * @return Object& 
 */
Object& Object::operator~(){
    auto num = std::get<i64>(this->m_value);
    num = ~num;
    this->m_value = num;
    return *this;
}

/**
 * @brief Implement bitwise 'or' operation on object.
 * 
 * Assumes object is an Integer or a HashSet.
 * 
 * (1) As Integer: '|' is just a bitwise or operation
 * (2) As HashSet: '|' is used as union operation on HashSet
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator|(const Object& lhs, const Object& rhs){
    if(lhs.is_integer() && rhs.is_integer()){   
        Object x = lhs;
        Object y = rhs;
        auto xnum = static_cast<i64>(x);
        auto ynum = static_cast<i64>(y);
        return Object((xnum | ynum));
    }
    // Assumes lhs and rhs are Set-object.
    Args args{};
    args.push_back(std::make_shared<Object>(lhs));
    args.push_back(std::make_shared<Object>(rhs));
    Object obj = lhs;
    return Object(*obj.__bit_or__(args));
}

/**
 * @brief Implement bitwise 'and' (&) operation on object.
 * 
 * Assumes object is an Integer or a HashSet.
 * 
 * (1) As Integer: '&' is just a bitwise or operation
 * (2) As HashSet: '&' is used as intersection operation on HashSet
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator&(const Object& lhs, const Object& rhs){
    if(lhs.is_integer() && rhs.is_integer()){
        Object x = lhs;
        Object y = rhs;
        auto xnum = static_cast<i64>(x);
        auto ynum = static_cast<i64>(y);
        return Object((xnum & ynum));
    }
    // Assumes lhs and rhs are Set-object.
    Args args{};
    args.push_back(std::make_shared<Object>(lhs));
    args.push_back(std::make_shared<Object>(rhs));
    Object obj = lhs;
    return Object(*obj.__bit_and__(args));
}

/**
 * @brief Implement bitwise 'xor' (^) operation on object.
 * 
 * Assumes object is an Integer or a HashSet.
 * 
 * (1) As Integer: '^' is just a bitwise or operation
 * (2) As HashSet: '^' is used as symmetric-difference operation on HashSet
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator^(const Object& lhs, const Object& rhs){
    if(lhs.is_integer() && rhs.is_integer()){   
        Object x = lhs;
        Object y = rhs;
        auto xnum = static_cast<i64>(x);
        auto ynum = static_cast<i64>(y);
        return Object((xnum ^ ynum));
    }
    // Assumes lhs and rhs are Set-object.
    Args args{};
    args.push_back(std::make_shared<Object>(lhs));
    args.push_back(std::make_shared<Object>(rhs));
    Object obj = lhs;
    return Object(*obj.__bit_xor__(args));
}

/**
 * @brief Implement bitwise left-shift on object.
 * 
 * Assumes object is an Integer
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator<<(const Object& lhs, const Object& rhs){
    Object x = lhs;
    Object y = rhs;
    auto xnum = static_cast<i64>(x);
    auto ynum = static_cast<i64>(y);
    return Object((xnum << ynum));
}

/**
 * @brief Implement bitwise right-shift on object.
 * 
 * Assumes object is an Integer.
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator>>(const Object& lhs, const Object& rhs){
    Object x = lhs;
    Object y = rhs;
    auto xnum = static_cast<i64>(x);
    auto ynum = static_cast<i64>(y);
    return Object((xnum >> ynum));
}

/**
 * @brief Implement equal-to "==" operation on object.
 * 
 * The equal-to is applicable on the following builtins objects:
 *      - Nil
 *      - Bool
 *      - Integer
 *      - Float
 *      - Complex
 *      - Symbol
 *      - String
 *      - <Tuple> ???
 * If object does not support
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
Object operator==(const Object& lhs, const Object& rhs){
    Object x = lhs;
    Object y = rhs;
    Args args{};
    args.push_back(std::make_shared<Object>(x));
    args.push_back(std::make_shared<Object>(y));
    Object obj{};
    return Object(*obj.__eq__(args));
}


/**
 * @brief Implement "not-equal" (!=) operation on object.
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator!=(const Object& lhs, const Object& rhs){
    return Object(!(lhs == rhs));
}


Object operator<(const Object& lhs, const Object& rhs){
    if(lhs.is_number() && rhs.is_number()){
        if(lhs.is_integer() && rhs.is_integer()){
            auto xnum = static_cast<i64>(Object(lhs));
            auto ynum = static_cast<i64>(Object(rhs));
            return Object((xnum < ynum));
        }else{
            auto xnum = static_cast<f64>(Object(lhs));
            auto ynum = static_cast<f64>(Object(rhs));
            return Object((xnum < ynum));
        }
    }

    // -
    Object obj{};
    Args args{};
    args.push_back(std::make_shared<Object>(lhs));
    args.push_back(std::make_shared<Object>(rhs));
    return Object((*Object().__lt__(args)));
}

/**
 * @brief Implement "<=" operation on object.
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator<=(const Object& lhs, const Object& rhs){
    auto x = (lhs < rhs);
    auto y = (lhs == rhs);
    return Object((x || y));
}

/**
 * @brief Implement ">=" operation on object.
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator>=(const Object& lhs, const Object& rhs){
    auto x = (lhs > rhs);
    auto y = (lhs == rhs);
    return Object((x || y));
}

/**
 * @brief Implement ">" on object.
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator>(const Object& lhs, const Object& rhs){
    return Object((!(lhs <= rhs)));
}


/**
 * @brief Implement "+" operatin on object.
 * 
 * The '+' operator is applicable on the following builtin types:
 * 
 * (1) Integer: (x + y) -> Integer
 * (2) Float: (x + y) -> Float
 *      - Float + Integer -> Float
 *      - Integer + Float -> Float
 * (3) Complex: (z1 + z2) -> Complex
 *      - Number + Complex -> Complex
 *      - Complex + Number -> Complex
 * (4) String: (xstr + ystr) -> String
 * (5) Tuple: (lhs + rhs) -> Tuple
 * (5) List: (lhs + rhs) -> List
 * (5) HashSet: (lhs + rhs) -> HashSet
 * (6) HashMap: (lhs + rhs) -> HashMap 
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator+(const Object& lhs, const Object& rhs){
    Args args{};
    args.push_back(std::make_shared<Object>(lhs));
    args.push_back(std::make_shared<Object>(rhs));
    return Object(*Object().__add__(args));
}


/**
 * @brief Implement "-" operatin on object.
 * 
 * The '-' operator is applicable on the following builtin types:
 * 
 * (1) Integer: (x - y) -> Integer
 * (2) Float: (x - y) -> Float
 *      - Float - Integer -> Float
 *      - Integer - Float -> Float
 * (3) Complex: (z1 - z2) -> Complex
 *      - Number - Complex -> Complex
 *      - Complex - Number -> Complex
 * (4) HashSet: (lhs - rhs) -> HashSet
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator-(const Object& lhs, const Object& rhs){
    Args args{};
    args.push_back(std::make_shared<Object>(lhs));
    args.push_back(std::make_shared<Object>(rhs));
    return Object(*Object().__sub__(args));
}

/**
 * @brief Implement "/" operatin on object.
 * 
 * The '/' operator is applicable on the following builtin types:
 * 
 * (1) Integer: (x / y) -> Integer
 * (2) Float: (x / y) -> Float
 *      - Float / Integer -> Float
 *      - Integer / Float -> Float
 * (3) Complex: (z1 / z2) -> Complex
 *      - Number / Complex -> Complex
 *      - Complex / Number -> Complex
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator/(const Object& lhs, const Object& rhs){
    Args args{};
    args.push_back(std::make_shared<Object>(lhs));
    args.push_back(std::make_shared<Object>(rhs));
    return Object(*Object().__div__(args));
}

/**
 * @brief Implement "%" operatin on object.
 * 
 * The '%' operator is applicable on the following builtin types:
 * 
 * (1) Integer: (x % y) -> Integer
 * (2) Float: (x % y) -> Float
 *      - Float % Integer -> Float
 *      - Integer % Float -> Float
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator%(const Object& lhs, const Object& rhs){
    Args args{};
    args.push_back(std::make_shared<Object>(lhs));
    args.push_back(std::make_shared<Object>(rhs));
    return Object(*Object().__mod__(args));
}

/**
 * @brief Implement "*" operatin on object.
 * 
 * The '*' operator is applicable on the following builtin types:
 * 
 * (1) Integer: (x * y) -> Integer
 * (2) Float: (x * y) -> Float
 *      - Float * Integer -> Float
 *      - Integer * Float -> Float
 * (3) Complex: (z1 * z2) -> Complex
 *      - Number * Complex -> Complex
 *      - Complex * Number -> Complex
 * 
 * @param lhs 
 * @param rhs 
 * @return Object 
 */
Object operator*(const Object& lhs, const Object& rhs){
    Args args{};
    args.push_back(std::make_shared<Object>(lhs));
    args.push_back(std::make_shared<Object>(rhs));
    return Object(*Object().__mod__(args));
}

/**
 * @brief Implement "+=" operatin on object.
 * 
 * The '+=' operator is applicable on the following builtin types:
 * 
 * (1) Integer: (x += y) -> Integer
 * (2) Float: (x += y) -> Float
 *      - Float += Integer -> Float
 *      - Integer += Float -> Float
 * (3) Complex: (z1 += z2) -> Complex
 *      - Complex += Number -> Complex
 * (4) String: (xstr += ystr) -> String
 * (5) Tuple: (lhs += rhs) -> Tuple
 * (5) List: (lhs += rhs) -> List
 * (5) HashSet: (lhs += rhs) -> HashSet
 * (6) HashMap: (lhs += rhs) -> HashMap 
 * 
 * @param rhs 
 * @return Object 
 */
Object& Object::operator+=(const Object& rhs){
    Args args{};
    args.push_back(std::make_shared<Object>(*this));
    args.push_back(std::make_shared<Object>(rhs));
    Self self = Object().__add__(args);
    this->m_value = std::move(self->m_value);

    return *this;
}

/**
 * @brief Implement "-=" operatin on object.
 * 
 * The '-=' operator is applicable on the following builtin types:
 * 
 * (1) Integer: (x -= y) -> Integer
 * (2) Float: (x -= y) -> Float
 *      - Float -= Integer -> Float
 *      - Integer -= Float -> Float
 * (3) Complex: (z1 -= z2) -> Complex
 *      - Complex -= Number -> Complex
 * (4) HashSet: (lhs -= rhs) -> HashSet
 * 
 * @param rhs 
 * @return Object 
 */
Object& Object::operator-=(const Object& rhs){
    Args args{};
    args.push_back(std::make_shared<Object>(*this));
    args.push_back(std::make_shared<Object>(rhs));
    Self self = Object().__sub__(args);
    this->m_value = std::move(self->m_value);

    return *this;
}

/**
 * @brief Implement "/=" operatin on object.
 * 
 * The '/=' operator is applicable on the following builtin types:
 * 
 * (1) Integer: (x /= y) -> Integer
 * (2) Float: (x /= y) -> Float
 *      - Float /= Integer -> Float
 *      - Integer /= Float -> Float
 * (3) Complex: (z1 /= z2) -> Complex
 *      - Complex /= Number -> Complex
 * 
 * @param rhs 
 * @return Object 
 */
Object& Object::operator/=(const Object& rhs){
    Args args{};
    args.push_back(std::make_shared<Object>(*this));
    args.push_back(std::make_shared<Object>(rhs));
    Self self = Object().__div__(args);
    this->m_value = std::move(self->m_value);

    return *this;
}

/**
 * @brief Implement "%=" operatin on object.
 * 
 * The '%=' operator is applicable on the following builtin types:
 * 
 * (1) Integer: (x %= y) -> Integer
 * (2) Float: (x %= y) -> Float
 *      - Float %= Integer -> Float
 *      - Integer %= Float -> Float
 * 
 * @param rhs 
 * @return Object 
 */
Object& Object::operator%=(const Object& rhs){
    Args args{};
    args.push_back(std::make_shared<Object>(*this));
    args.push_back(std::make_shared<Object>(rhs));
    Self self = Object().__mod__(args);
    this->m_value = std::move(self->m_value);

    return *this;
}

/**
 * @brief Implement "*=" operatin on object.
 * 
 * The '*=' operator is applicable on the following builtin types:
 * 
 * (1) Integer: (x *= y) -> Integer
 * (2) Float: (x *= y) -> Float
 *      - Float *= Integer -> Float
 *      - Integer *= Float -> Float
 * 
 * @param rhs 
 * @return Object 
 */
Object& Object::operator*=(const Object& rhs){
    Args args{};
    args.push_back(std::make_shared<Object>(*this));
    args.push_back(std::make_shared<Object>(rhs));
    Self self = Object().__mul__(args);
    this->m_value = std::move(self->m_value);

    return *this;
}

// -*-
Object& Object::operator|=(const Object& rhs){
    Args args{};
    args.push_back(std::make_shared<Object>(*this));
    args.push_back(std::make_shared<Object>(rhs));
    Self self = Object().__bit_or__(args);
    this->m_value = std::move(self->m_value);

    return *this;
}

// -*-
Object& Object::operator&=(const Object& rhs){
    Args args{};
    args.push_back(std::make_shared<Object>(*this));
    args.push_back(std::make_shared<Object>(rhs));
    Self self = Object().__bit_and__(args);
    this->m_value = std::move(self->m_value);

    return *this;
}

// -*-
Object& Object::operator^=(const Object& rhs){
    Args args{};
    args.push_back(std::make_shared<Object>(*this));
    args.push_back(std::make_shared<Object>(rhs));
    Self self = Object().__bit_xor__(args);
    this->m_value = std::move(self->m_value);

    return *this;
}

// -*-
Object& Object::operator<<=(const Object& rhs){
    Args args{};
    args.push_back(std::make_shared<Object>(*this));
    args.push_back(std::make_shared<Object>(rhs));
    Self self = Object().__bit_shl__(args);
    this->m_value = std::move(self->m_value);

    return *this;
}

// -*-
Object& Object::operator>>=(const Object& rhs){
    Args args{};
    args.push_back(std::make_shared<Object>(*this));
    args.push_back(std::make_shared<Object>(rhs));
    Self self = Object().__bit_shr__(args);
    this->m_value = std::move(self->m_value);

    return *this;
}

// --------------------------
// -*- Indexind Operators -*-
// --------------------------
/**
 * @brief Implement indexing for Tuple and List
 * 
 * @param idx 
 * @return Self& 
 */
Self& Object::operator[](int idx){
    return std::get<List>(this->m_value)[idx];
}

const Self& Object::operator[](int idx) const{
    return std::get<List>(this->m_value)[idx];
}

/**
 * @brief Implement key-access on dictionary object
 * 
 * @param key 
 * @return Self& 
 */
Self& Object::operator[](const Self& key){
    return std::get<Dict>(this->m_value)[key];
}

const Self& Object::operator[](const Self& key) const{
    return std::get<Dict>(this->m_value)[key];
}

// -------------------------
// -*- Object Predicates -*-
// -------------------------
bool Object::is_nil(void) const{
    return this->m_kind == Object::Kind::None;
}

// -*-
bool Object::is_bool(void) const{
    return this->m_kind == Object::Kind::Bool;
}

// -*-
bool Object::is_integer(void) const{
    return this->m_kind == Object::Kind::Integer;
}

// -*-
bool Object::is_float(void) const{
    return this->m_kind == Object::Kind::Float;
}

// -*-
bool Object::is_complex(void) const{
    return this->m_kind == Object::Kind::Complex;
}

// -*-
bool Object::is_number(void) const{
    return (
        this->is_integer() ||
        this->is_float() ||
        this->is_complex()
    );
}

// -*-
bool Object::is_symbol(void) const{
    return this->m_kind == Object::Kind::Sym;
}

// -*-
bool Object::is_string(void) const{
    return this->m_kind == Object::Kind::String;
}

// -*-
bool Object::is_tuple(void) const{
    return this->m_kind == Object::Kind::Tuple;
}

// -*-
bool Object::is_list(void) const{
    return this->m_kind == Object::Kind::Vector;
}

// -*-
bool Object::is_hashset(void) const{
    return this->m_kind == Object::Kind::Set;
}

// -*-
bool Object::is_hashmap(void) const{
    return this->m_kind == Object::Kind::Dict;
}

// -*-
bool Object::is_builtin(void) const{
    return this->m_kind == Object::Kind::CFun;
}

// -*-
bool Object::is_function(void) const{
    return this->m_kind == Object::Kind::Fun;
}

// -*-
bool Object::is_macro(void) const{
    return this->m_kind == Object::Kind::Macro;
}

// -*-
bool Object::is_lambda(void) const{
    return this->m_kind == Object::Kind::Lambda;
}

// -*-
bool Object::is_callable(void) const{
    bool flag = (
        this->is_builtin() ||
        this->is_function() ||
        this->is_lambda() ||
        this->is_macro()
    );
    if(flag){ return true; }
    if(this->is_structure()){
        auto cls = std::get<Structure>(this->m_value);
        if(cls.methods().find(Symbol("__call__")) != cls.methods().end()){
            return true;
        }
    }
    return false;
}

// -*-
bool Object::is_hashable(void) const{
    bool flag = (
        this->is_bool() ||
        this->is_integer() ||
        this->is_symbol() ||
        this->is_string() ||
        this->is_tuple()
    );
    if(flag){ return true; }
    if(this->is_structure()){
        auto cls = std::get<Structure>(this->m_value);
        if(cls.methods().find(Symbol("__hash__")) != cls.methods().end()){
            return true;
        }
    }

    return false;
}

// -*-
bool Object::is_iterable(void) const{
    bool flag = (
        this->is_string() ||
        this->is_tuple() ||
        this->is_list() ||
        this->is_hashset() ||
        this->is_hashmap()
    );
    if(flag){ return true; }
    if(this->is_structure()){
        auto cls = std::get<Structure>(this->m_value);
        flag = (
            (cls.methods().find(Symbol("__next__")) != cls.methods().end()) &&
            (cls.methods().find(Symbol("__done__")) != cls.methods().end())
        );
        if(flag){ return true; }
    }
    return false;
}

// -*-
bool Object::is_iterator(void) const{
    return this->m_kind == Object::Kind::Iter;
}

// -*-
bool Object::is_result(void) const{
    return this->m_kind == Object::Kind::Result;
}

// -*-
bool Object::is_ok(void) const{
    // Assumes object is a Result kind
    auto result = std::get<Result>(this->m_value);
    return result.is_ok();
}

// -*-
bool Object::is_structure(void) const{
    return this->m_kind == Object::Kind::Struct;
}

// -*-
bool Object::is_builitn_type(void) const{
    return (
        this->is_nil() ||
        this->is_integer() ||
        this->is_float() ||
        this->is_complex() ||
        this->is_symbol() ||
        this->is_string() ||
        this->is_tuple() ||
        this->is_list() ||
        this->is_hashset() ||
        this->is_hashmap() ||
        this->is_result()
    );
}

// ------------------------------
// -*- fn, fun, lambda, macro -*-
// ------------------------------
Self Object::operator()(Args args){
    if(this->is_callable()){
        if(this->is_builtin()){
            auto cfun = std::get<CFun>(this->m_value);
            return cfun(args);
        }
        if(this->is_function()){
            auto ast = std::get<Ast>(this->m_value);
            auto fun = reinterpret_cast<FunDefExprAst*>(ast.get());
            auto fn = *fun;
            return fn(args);
        }
        if(this->is_macro()){
            auto ast = std::get<Ast>(this->m_value);
            auto macro = reinterpret_cast<MacroDefExprAst*>(ast.get());
            auto fn = *macro;
            return fn(args);
        }
        if(this->is_lambda()){
            auto ast = std::get<Ast>(this->m_value);
            auto lambda = reinterpret_cast<LambdaDefExprAst*>(ast.get());
            auto fn = *lambda;
            return fn(args);
        }
        if(this->is_structure()){
            auto cls = std::get<Structure>(this->m_value);
            auto ast = cls.methods()[Symbol("__call__")];
            auto fun = reinterpret_cast<FunDefExprAst*>(ast.get());
            auto fn = *fun;
            return fn(args);
        }
    }

    auto err = Error(Error::Kind::SyntaxError, "cannot call a non-callable object.");
    return std::make_shared<Object>(Result(err));
}

// ------------------------
// -*- call type method -*-
// ------------------------
/**
 * @brief Call a method, a special-method or an operator object.
 * 
 * (1) Method form: `obj.method(args)`
 * (2) Special-Method form: special-method(args)
 *     Special method includes:
 *      - `__hash__` ==> hash()
 *      - '__repr__` ==> repr()
 *      - `__bool__` ==> Bool()
 *      - `__integer__` ==> Integer()
 *      - `__float__` ==> Float()
 *      - `__string__` ==> String()
 *      - `__tuple__` ==> Tuple()
 *      - `__list__` ==> List()
 *      - `__hashset__` ==> HashSet()
 *      - `__hashmap__` ==> HashMap()
 *      - `__next__` && `__done__` ==> Iterator()
 * 
 * (3) Operator-form:
 *      - binary-operator: lsh `op` rhs
 *      - unary-operator: `op` rhs
 * 
 *  Main operators includes:
 *      +, -, *, /, %, ~, |, &, ^, <<, >>,
 *      +=, -=, *=, /=, %=, |=, &=, ^=, <<=, >>=
 *      and, or, not
 * 
 * @param fname 
 * @param args 
 * @return Self 
 */
Self Object::operator()(const Str& fname, Args args){
    if(this->is_bool()){
        if(Lynx::boolMethods.find(fname) != Lynx::boolMethods.end()){
            Self (Object::*fun)(Args);
            fun = Lynx::boolMethods[fname];
            return (this->*fun)(args);
        }
    }else if(this->is_integer()){
        if(Lynx::integerMethods.find(fname) != Lynx::integerMethods.end()){
            Self (Object::*fun)(Args);
            fun = Lynx::integerMethods[fname];
            return (this->*fun)(args);
        }
    }else if(this->is_float()){
        if(Lynx::floatMethods.find(fname) != Lynx::floatMethods.end()){
            Self (Object::*fun)(Args);
            fun = Lynx::floatMethods[fname];
            return (this->*fun)(args);
        }
    }else if(this->is_complex()){
        if(Lynx::complexMethods.find(fname) != Lynx::complexMethods.end()){
            Self (Object::*fun)(Args);
            fun = Lynx::complexMethods[fname];
            return (this->*fun)(args);
        }
    }else if(this->is_tuple()){
        if(Lynx::tupleMethods.find(fname) != Lynx::tupleMethods.end()){
            Self (Object::*fun)(Args);
            fun = Lynx::tupleMethods[fname];
            return (this->*fun)(args);
        }
    }else if(this->is_list()){
        if(Lynx::listMethods.find(fname) != Lynx::listMethods.end()){
            Self (Object::*fun)(Args);
            fun = Lynx::listMethods[fname];
            return (this->*fun)(args);
        }
    }else if(this->is_string()){
        if(Lynx::stringMethods.find(fname) != Lynx::stringMethods.end()){
            Self (Object::*fun)(Args);
            fun = Lynx::stringMethods[fname];
            return (this->*fun)(args);
        }
    }else if(this->is_hashset()){
        if(Lynx::hashsetMethods.find(fname) != Lynx::hashsetMethods.end()){
            Self (Object::*fun)(Args);
            fun = Lynx::hashsetMethods[fname];
            return (this->*fun)(args);
        }
    }else if(this->is_hashmap()){
        if(Lynx::hashmapMethods.find(fname) != Lynx::hashmapMethods.end()){
            Self (Object::*fun)(Args);
            fun = Lynx::hashmapMethods[fname];
            return (this->*fun)(args);
        }
    }else if(this->is_result()){
        if(Lynx::resultMethods.find(fname) != Lynx::resultMethods.end()){
            Self (Object::*fun)(Args);
            fun = Lynx::complexMethods[fname];
            return (this->*fun)(args);
        }
    }else if(this->is_structure()){
        auto cls = std::get<Structure>(this->m_value);
        if(cls.methods().find(Symbol(fname)) != cls.methods().end()){
            auto self = cls.methods()[Symbol(fname)];
            auto ast = std::get<Ast>(self->m_value);
            auto fun = reinterpret_cast<FunDefExprAst*>(ast.get());
            auto fn = *fun;
            return fn(args);
        }
    }

    std::stringstream stream;
    stream << static_cast<Str>(this->type());
    stream << " does not implement method '" << fname << "'";
    auto err = Error(Error::Kind::SyntaxError, stream.str());
    return std::make_shared<Object>(Result(err));
}

/**
 * @brief Unwrap the value encapsulated in Result-object.
 * 
 * Assumes object is a result-type
 * if result-object rather encapsulates an Error, the throw std::runtime_error
 * 
 * @return Self 
 */
Self Object::unwrap(void) const{
    auto result = std::get<Result>(this->m_value);
    if(!result.is_ok()){
        std::stringstream stream;
        stream << "Fatal error: " << result.err().message();
        throw std::runtime_error(stream.str());
    }
    return std::move(result.ok());
}

/**
 * @brief Return object type-name.
 * 
 * @return Symbol 
 */
Symbol Object::type(void) const{
    if(this->is_nil()){ return Symbol("nil"); }
    if(this->is_bool()){ return Symbol("Bool"); }
    if(this->is_integer()){ return Symbol("Integer"); }
    if(this->is_float()){ return Symbol("Float"); }
    if(this->is_complex()){ return Symbol("Complex"); }
    if(this->is_symbol()){ return Symbol("Symbol"); }
    if(this->is_string()){ return Symbol("String"); }
    if(this->is_tuple()){ return Symbol("Tuple"); }
    if(this->is_list()){ return Symbol("List"); }
    if(this->is_hashmap()){ return Symbol("HashMap"); }
    if(this->is_hashset()){ return Symbol("HashSet"); }
    if(this->is_iterator()){
        std::stringstream stream;
        stream << static_cast<Str>(this->type()) << "Iterator";
        return Symbol(stream.str());
    }
    // assume user-defined type through struct-syntax
    auto cls = std::get<Structure>(this->m_value);
    return cls.name();
}

// -*-
Str Object::repr(void) const{
    Args args{};
    args.push_back(std::make_shared<Object>(*this));
    Self self = Object().__repr__(args);
    return static_cast<Str>(*self);
}

// -*-
usize Object::hash(void) const{
    Args args{};
    args.push_back(std::make_shared<Object>(*this));
    Self self = Object().__repr__(args);
    auto val = static_cast<i64>(*self);
    return static_cast<usize>(val);
}

// -*-
Iterator Object::iter(void) const{
    return std::get<Iterator>(this->m_value);
}

// -*-
static bool check_argcount(Args args, i32 count){
    return (args.size() == count);
}

// -------------------------------
// -*- Complex-numbers methods -*-
// -------------------------------
Self Object::real(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "invalid number of argument");
        return std::make_shared<Object>(Result(err));
    }
    auto z = std::get<Complex>(args[0]->m_value);
    return std::make_shared<Object>(z.real());
}

// -*-
Self Object::imag(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "invalid number of argument");
        return std::make_shared<Object>(Result(err));
    }
    auto z = std::get<Complex>(args[0]->m_value);
    return std::make_shared<Object>(z.imag());
}

// -*-
Self Object::abs(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "invalid number of argument");
        return std::make_shared<Object>(Result(err));
    }
    auto z = std::get<Complex>(args[0]->m_value);
    return std::make_shared<Object>(std::abs(z));
}

// -*-
Self Object::arg(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "invalid number of argument");
        return std::make_shared<Object>(Result(err));
    }
    auto z = std::get<Complex>(args[0]->m_value);
    return std::make_shared<Object>(std::arg(z));
}

// -*-
Self Object::norm(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "invalid number of argument");
        return std::make_shared<Object>(Result(err));
    }
    auto z = std::get<Complex>(args[0]->m_value);
    return std::make_shared<Object>(std::norm(z));
}

// -*-
Self Object::conj(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "invalid number of argument");
        return std::make_shared<Object>(Result(err));
    }
    auto z = std::get<Complex>(args[0]->m_value);
    return std::make_shared<Object>(std::conj(z));
}

// -*-
Self Object::polar(Args args){
    if(!check_argcount(args, 2)){
        Error err(Error::Kind::ValueError, "invalid number of argument");
        return std::make_shared<Object>(Result(err));
    }
    auto rho = std::get<f64>(args[0]->m_value);
    auto theta = std::get<f64>(args[1]->m_value);
    return std::make_shared<Object>(std::polar(rho, theta));
}

// -*---------------------------------
// -*- Sequence Specific Operators -*-
// -----------------------------------
Self Object::clear(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "invalid number of argument");
        return std::make_shared<Object>(Result(err));
    }
    Self self = args[0];
    if(self->is_list()){
        std::get<List>(self->m_value).clear();    
    }else if(self->is_hashset()){
        std::get<Set>(self->m_value).clear();
    }else if(self->is_hashmap()){
        std::get<Dict>(self->m_value).m_data.clear();
    }
    return std::make_shared<Object>(*self);
}

// -*-
Self Object::len(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "invalid number of argument");
        return std::make_shared<Object>(Result(err));
    }
    usize xlen{};
    auto self = args[0];
    if(self->is_list()){
        xlen = std::get<List>(self->m_value).size();    
    }else if(self->is_hashset()){
        xlen = std::get<Set>(self->m_value).m_data.size();
    }else if(self->is_hashmap()){
        xlen = std::get<Dict>(self->m_value).m_data.size();
    }else if(self->is_string()){
        xlen = std::get<Str>(self->m_value).length();
    }
    return std::make_shared<Object>(static_cast<i64>(xlen));
}

// -*-
Self Object::find(Args args){
    if(!check_argcount(args, 2)){
        Error err(Error::Kind::ValueError, "invalid number of argument");
        return std::make_shared<Object>(Result(err));
    }
    Self self = args[0];
    Self needle = args[1];
    if(self->is_string() && needle->is_string()){
        i64 pos{};
        auto str = std::get<Str>(self->m_value);
        auto key = std::get<Str>(self->m_value);
        auto ptr = str.find(key);
        if(ptr != std::string::npos){
            pos = static_cast<i64>(ptr);
        }else{
            pos = -1;
        }
        return std::make_shared<Object>(pos);
    }else if(self->is_tuple() || self->is_list()){
        i64 pos{};
        auto vec = std::get<List>(self->m_value);
        Vec<Object> myvec{};
        for(auto x: vec){ myvec.push_back(Object(*x)); }
        auto val = *needle;
        auto ptr = std::find(myvec.begin(), myvec.end(), val);
        if(ptr == myvec.end()){ pos = -1; }
        else{ pos = std::distance(ptr, myvec.end()); }
        return std::make_shared<Object>(pos);
    }else if(self->is_hashset()){
        auto hset = std::get<Set>(self->m_value);
        return std::make_shared<Object>(hset.contains(needle));
    }else if(self->is_hashmap()){
        auto hmap = std::get<Dict>(self->m_value);
        return std::make_shared<Object>(hmap.contains(needle));
    }

    Error err(Error::Kind::SyntaxError, " method `find` is not implemented");
    return std::make_shared<Object>(Result(err));
}


// -*-
Self Object::find_all(Args args){
    if(!check_argcount(args, 2)){
        Error err(Error::Kind::ValueError, "invalid number of argument");
        return std::make_shared<Object>(Result(err));
    }
    auto self = args[0];
    auto needle = args[1];
    if(self->is_string() && needle->is_string()){
        auto str = std::get<Str>(self->m_value);
        auto key = std::get<Str>(needle->m_value);
        List vec{};
        auto pos = str.find(key);
        while((pos != Str::npos)){
            vec.push_back(std::make_shared<Object>(static_cast<i64>(pos)));
            pos += key.length();
            pos = str.find(key, pos);
        }
        return std::make_shared<Object>(Object::Kind::Tuple, vec);
    }else if(self->is_list() || self->is_tuple()){
        auto xlist = std::get<List>(self->m_value);
        List vec{};
        for(auto key=xlist.begin(); key != xlist.end(); key++){
            auto obj = *key;
            if((*obj == *needle)){
                auto pos = std::distance(key, xlist.begin());
                vec.push_back(std::make_shared<Object>(static_cast<i64>(pos)));
            }
        }
        return std::make_shared<Object>(Object::Kind::Tuple, vec);
    }
    Error err(Error::Kind::SyntaxError, " method `find` is not implemented");
    return std::make_shared<Object>(Result(err));
}


// -*-
Self Object::find_last(Args args){
    if(!check_argcount(args, 2)){
        Error err(Error::Kind::ValueError, "invalid number of argument");
        return std::make_shared<Object>(Result(err));
    }
    auto rv = this->find_all(args);
    if(rv->is_result()){
        return std::make_shared<Object>(*rv);
    }
    auto vec = std::get<List>(rv->m_value);
    if(vec.size()==0){
        auto obj = Object(static_cast<i64>(-1));
        return std::make_shared<Object>(obj);
    }
    auto pos = *(vec[vec.size()-1]);
    auto idx = static_cast<i64>(pos);
    return std::make_shared<Object>(idx);
}

// -*-
Self Object::slice(Args args){
    auto self = args[0];
    if(check_argcount(args, 2)){
        // [start:]
        auto idx = static_cast<int>(static_cast<i64>(*(args[1])));
        if(self->is_string()){
            auto str = std::get<Str>(self->m_value);
            if(idx >= 0 && idx < str.length()){
                auto xstr = str.substr(idx);
                return std::make_shared<Object>(xstr);
            }
        }else if(self->is_list() || self->is_tuple()){
            auto vec = std::get<List>(self->m_value);
            if(idx >= 0 && idx < vec.size()){
                auto ptr = vec.begin() + idx;
                List xs{};
                for(; ptr != vec.end(); ptr++){
                    auto obj = *ptr;
                    xs.push_back(std::make_shared<Object>(*obj));
                }
                if(self->is_list()){
                    return std::make_shared<Object>(Object::Kind::Vector, xs);
                }
                return std::make_shared<Object>(Object::Kind::Tuple, xs);
            }
        }
    }else if(check_argcount(args, 3)){
        // obj[start:stop]
        // assumes args[1] and args[2] are integers
        auto start = static_cast<int>(static_cast<i64>(*args[1]));
        auto stop = static_cast<int>(static_cast<i64>(*args[2]));
        if(start > stop){
            Error err(Error::Kind::SyntaxError, "incorred slicing-syntax");
            return std::make_shared<Object>(Result(err));        
        }
        auto n = stop - start;
        if(self->is_string()){
            auto str = std::get<Str>(self->m_value);
            auto xstr = str.substr(start, n);
            return std::make_shared<Object>(xstr);
        }else if(self->is_list() || self->is_tuple()){
            auto vec = std::get<List>(self->m_value);
            auto ptr = vec.begin();
            List xs{};
            for(auto i=0; i <= n; i++){
                auto x = ptr + (start + i);
                auto val = *x;
                auto obj = *val;
                xs.push_back(std::make_shared<Object>(obj));
            }
            if(self->is_list()){
                return std::make_shared<Object>(Object::Kind::Vector, xs);
            }
            return std::make_shared<Object>(Object::Kind::Tuple, xs);
        }
    }else if(check_argcount(args, 4)){
        // obj[start:step:stop];
        // assumes `start`, `step`, and `stop` are integers
        auto start = static_cast<int>(static_cast<i64>(*args[1]));
        auto step = static_cast<int>(static_cast<i64>(*args[2]));
        auto stop = static_cast<int>(static_cast<i64>(*args[3]));
        if((start < 0 || start > stop) || (step < 0)){
            Error err(Error::Kind::SyntaxError, "incorrect slicing-syntax");
            return std::make_shared<Object>(Result(err));
        }
        auto n = stop - start;
        if(self->is_string()){
            auto str = std::get<Str>(self->m_value);
            auto ptr = str.begin() + start;
            Str xstr{};
            for(auto i = 0; i <= n; i += step){
                auto c = *(ptr + i);
                xstr += c;
            }
            return std::make_shared<Object>(xstr);
        }else if(self->is_tuple() || self->is_list()){
            auto vec = std::get<List>(self->m_value);
            auto ptr = vec.begin() + start;
            List xs{};
            for(auto i=0; i <= n; i += step){
                auto xp = *(ptr + i);
                auto x = *xp;
                xs.push_back(std::make_shared<Object>(x));
            }
            if(self->is_list()){
                return std::make_shared<Object>(Object::Kind::Vector, xs);
            }
            return std::make_shared<Object>(Object::Kind::Tuple, xs);
        }
    }

    Error err(Error::Kind::SyntaxError, "invalid number of arguments");
    return std::make_shared<Object>(Result(err));
}


// -*-
Self Object::sort(Args args){
    // assumes args has at least 1-element and it is a list or a tuple
    auto self = args[0];
    if(self->is_list() || self->is_tuple()){
        if(check_argcount(args, 1)){
            // obj.sort()
            auto vec = std::get<List>(self->m_value);
            Vec<Object> xs{};
            for(const auto& x: vec){
                xs.push_back(Object(*x));
            }
            std::sort(xs.begin(), xs.end());
            vec = {};
            for(const auto& x: xs){
                vec.push_back(std::make_shared<Object>(x));
            }
            if(self->is_list()){
                return std::make_shared<Object>(Object::Kind::Vector, vec);
            }
            return std::make_shared<Object>(Object::Kind::Tuple, vec);
        }else if(check_argcount(args, 2)){
            // obj.sort(predicate);
            auto predicate = args[1];
            if(!predicate->is_callable() || predicate->argc() != 2){
                Error err(Error::Kind::TypeError, "expect a callable as argument to `sort` method.");
                return std::make_shared<Object>(Result(err));
            }
            auto keyfn = [&predicate](const Self& lhs, const Self& rhs){
                Args argv{};
                argv.push_back(std::make_shared<Object>(*lhs));
                argv.push_back(std::make_shared<Object>(*rhs));
                auto fn = *predicate;
                auto rv = fn(argv);
                return static_cast<bool>(*rv);
            };
            auto vec = std::get<List>(self->m_value);
            List xs{};
            for(const auto& x: vec){
                xs.push_back(std::make_shared<Object>(*x));
            }
            std::sort(xs.begin(), xs.end(), keyfn);
            if(self->is_list()){
                return std::make_shared<Object>(Object::Kind::Vector, xs);
            }
            return std::make_shared<Object>(Object::Kind::Tuple, xs);
        }
    }

    Error err(Error::Kind::SyntaxError, "invalid `sort` method call.");
    return std::make_shared<Object>(Result(err));
}


// -*-
Self Object::reverse(Args args){
    // assumes args.size() == 1
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::SyntaxError, "invalid `reverse` method call.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = args[0];
    if(self->is_string()){
        auto str = std::get<Str>(self->m_value);
        Str xstr{};
        for(auto ptr = str.rbegin(); ptr != str.rend(); ptr++){
            auto c = *ptr;
            xstr += c;
        }
        return std::make_shared<Object>(xstr);
    }else if(self->is_list() || self->is_tuple()){
        auto vec = std::get<List>(self->m_value);
        List xs{};
        for(const auto& x: vec){
            xs.push_back(std::make_shared<Object>(*x));
        }
        std::reverse(xs.begin(), xs.end());
        if(self->is_list()){
            return std::make_shared<Object>(Object::Kind::Vector, xs);
        }
        return std::make_shared<Object>(Object::Kind::Tuple, xs);
    }
    Error err(Error::Kind::SyntaxError, "invalid `sort` method call.");
    return std::make_shared<Object>(Result(err));
}

// ---------------------------------
// -*- String Specific Operators -*-
// ---------------------------------
Self Object::upper(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "invalid number of arguments to `upper`");
        return std::make_shared<Object>(Result(err));
    }
    // obj.upper()
    auto self = args[0];
    if(self->is_string()){
        auto str = std::get<Str>(self->m_value);
        Str xstr{};
        for(auto ptr = str.begin(); ptr != str.end(); ptr++){
            auto c = *ptr;
            xstr += std::toupper(c);
        }
        return std::make_shared<Object>(xstr);
    }

    Error err(Error::Kind::SyntaxError, "invalid `upper` method call.");
    return std::make_shared<Object>(Result(err));
}

// -*-
Self Object::lower(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "invalid number of arguments to `lower`");
        return std::make_shared<Object>(Result(err));
    }
    // obj.upper()
    auto self = args[0];
    if(self->is_string()){
        auto str = std::get<Str>(self->m_value);
        Str xstr{};
        for(auto ptr = str.begin(); ptr != str.end(); ptr++){
            auto c = *ptr;
            xstr += std::tolower(c);
        }
        return std::make_shared<Object>(xstr);
    }

    Error err(Error::Kind::SyntaxError, "invalid `lower` method call.");
    return std::make_shared<Object>(Result(err));
}

// -*-
Self Object::capitalize(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "invalid number of arguments to `capitalize`");
        return std::make_shared<Object>(Result(err));
    }
    // obj.upper()
    auto self = args[0];
    if(self->is_string()){
        auto str = std::get<Str>(self->m_value);
        Str xstr{};
        bool done = false;
        for(auto ptr = str.begin(); ptr != str.end(); ptr++){
            auto c = *ptr;
            if(!done){
                xstr += std::isspace(c) ? c : std::toupper(c);
            }else{
                xstr += std::tolower(c);
            }
        }
        return std::make_shared<Object>(xstr);
    }

    Error err(Error::Kind::SyntaxError, "invalid `capitalize` method call.");
    return std::make_shared<Object>(Result(err));
}

// -*-
Self Object::title(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "invalid number of arguments to `title`");
        return std::make_shared<Object>(Result(err));
    }
    auto self = args[0];
    if(self->is_string()){
        auto str = std::get<Str>(self->m_value);
        Str xstr{};
        bool flag{};
        bool fcar{true};
        for(auto ptr=str.begin(); ptr != str.end(); ptr++){
            auto c = *ptr;
            flag = std::isspace(c) ? true: false;
            if(!flag && fcar){
                xstr += std::toupper(c);
                fcar = false;
            }
            if(flag){ fcar = true; }
        }
        return std::make_shared<Object>(xstr);
    }

    Error err(Error::Kind::SyntaxError, "invalid `title` method call.");
    return std::make_shared<Object>(Result(err));
}

// -*-
Self Object::split(Args args){
    // obj.split(delim)
    auto self = args[0];
    if(check_argcount(args, 1)){
        // delim = ' '
        auto str = std::get<Str>(self->m_value);
        char delim = ' ';
        List vec{};
        Str word{};
        for(auto ptr=str.begin(); ptr != str.end(); ptr++){
            auto c = *ptr;
            if(word.length() != 0 && std::isspace(c)){
                vec.push_back(std::make_shared<Object>(word));
                word = "";
            }
            word += c;
        }
        return std::make_shared<Object>(Object::Kind::Tuple, vec);
    }else if(check_argcount(args, 2)){
        auto _delim = args[1];
        if(!_delim->is_string()){
            Error err(Error::Kind::TypeError, "invalid argument type to method `split`.");
            return std::make_shared<Object>(Result(err));
        }
        auto delim = std::get<Str>(_delim->m_value);
        auto str = std::get<Str>(self->m_value);
        List vec{};
        auto pos = str.find(delim);
        if(pos == Str::npos){
            vec.push_back(std::make_shared<Object>(str));
        }else{
            auto lastIdx = 0;
            while(pos != Str::npos){
                auto word = str.substr(lastIdx, pos);
                lastIdx += delim.length();
                pos = str.find(delim, lastIdx);
                vec.push_back(std::make_shared<Object>(word));
            }
            if(lastIdx < str.length()){
                auto word = str.substr(lastIdx);
                vec.push_back(std::make_shared<Object>(word));
            }
        }
        return std::make_shared<Object>(Object::Kind::Tuple, vec);
    }

    Error err(Error::Kind::SyntaxError, "invalid `split` method call.");
    return std::make_shared<Object>(Result(err));
}

// -*-
Self Object::join(Args args){
    // obj.join(["1", "aa", ...])
    if(!check_argcount(args, 2)){
        Error err(Error::Kind::ValueError, "invalid number of arguments to `join`");
        return std::make_shared<Object>(Result(err));
    }
    auto self = args[0];
    if(!self->is_string()){
        Error err(Error::Kind::TypeError, "`join()`: invalid method.");
        return std::make_shared<Object>(Result(err));
    }
    auto argv = args[1];
    if(!argv->is_list()){
        Error err(Error::Kind::TypeError, "`join()`: invalid argument type.");
        return std::make_shared<Object>(Result(err));
    }
    auto vec = static_cast<List>(*argv);
    if(vec.size()==0){
        return std::make_shared<Object>(Str{});
    }
    if(vec.size() == 1){
        if(!vec[0]->is_string()){
            Error err(Error::Kind::TypeError, "`join()`: invalid list element type.");
            return std::make_shared<Object>(Result(err));
        }
        auto val = *vec[0];
        auto str = static_cast<Str>(val);
        return std::make_shared<Object>(str);
    }
    std::stringstream stream;
    auto delim = std::get<Str>(self->m_value);
    auto str = static_cast<Str>(*vec[0]);
    stream << str;
    for(auto i=1; i < vec.size(); i++){
        str = static_cast<Str>(*vec[i]);
        stream << delim << str;
    }

    return std::make_shared<Object>(stream.str());
}

// -*- obj.replace(old, new)
Self Object::replace(Args args){
    if(!check_argcount(args, 3)){
        Error err(Error::Kind::ValueError, "`replace()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = *args[0];
    auto old = *args[1];
    auto neo = *args[2];
    if(self.is_string() && old.is_string() && neo.is_string()){
        auto str = static_cast<Str>(self);
        auto lhs = static_cast<Str>(old);
        auto rhs = static_cast<Str>(neo);
        auto pos = str.find(lhs);
        if(pos != Str::npos){
            str.replace(pos, lhs.length(), rhs);
            return std::make_shared<Object>(str);
        }
        return std::make_shared<Object>(str);
    }
    Error err(Error::Kind::TypeError, "`replace()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// -*-
Self Object::replace_all(Args args){
    if(!check_argcount(args, 3)){
        Error err(Error::Kind::ValueError, "`replace_all()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = *args[0];
    auto old = *args[1];
    auto neo = *args[2];
    if(self.is_string() && old.is_string() && neo.is_string()){
        auto str = static_cast<Str>(self);
        auto lhs = static_cast<Str>(old);
        auto rhs = static_cast<Str>(neo);
        auto pos = str.find(lhs);
        while(pos != Str::npos){
            str.replace(pos, lhs.length(), rhs);
            pos = str.find(lhs);
        }
        return std::make_shared<Object>(str);
    }
    Error err(Error::Kind::TypeError, "`replace_all()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// -*-
Self Object::endswith(Args args){
    if(!check_argcount(args, 2)){
        Error err(Error::Kind::ValueError, "`endswith()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = *args[0];
    auto rhs = *args[1];
    if(self.is_string() && rhs.is_string()){
        auto str = static_cast<Str>(self);
        auto suffix = static_cast<Str>(rhs);
        if(suffix.length() > str.length()){
            return std::make_shared<Object>(false);    
        }
        auto xlen = str.length();
        auto ylen = suffix.length();
        auto pos = xlen - ylen;
        auto sub = str.substr(pos);

        return std::make_shared<Object>((sub == suffix));
    }
    Error err(Error::Kind::TypeError, "`endswith()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// -*- str.startswith(prefix)
Self Object::startswith(Args args){
    if(!check_argcount(args, 2)){
        Error err(Error::Kind::ValueError, "`startswith()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = *args[0];
    auto rhs = *args[1];
    if(self.is_string() && rhs.is_string()){
        auto str = static_cast<Str>(self);
        auto prefix = static_cast<Str>(rhs);
        if(prefix.length() > str.length()){
            return std::make_shared<Object>(false);    
        }
        auto len = prefix.length();
        auto sub = str.substr(0, len);
        return std::make_shared<Object>((sub == prefix));
    }
    Error err(Error::Kind::TypeError, "`startswith()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// -*-
Self Object::isnumeric(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "`isnumeric()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = *args[0];
    if(self.is_string()){
        auto str = static_cast<Str>(self);
        auto flag = false;
        auto isfloat = (
            (str.find(".") != Str::npos) ||
            (str.find("e") != Str::npos) ||
            (str.find("E")!=Str::npos)
        );
        if(isfloat){
            try{
                usize pos{};
                auto num = std::stod(str, &pos);
                if(pos == str.length()){ flag = true; }
                LYNX_UNUSED(num);
            }catch(...){}
        }else{
            try{
                usize pos{};
                auto num = std::stoll(str, &pos);
                if(pos == str.length()){ flag = true; }
                LYNX_UNUSED(num);
            }catch(...){}
        }
        return std::make_shared<Object>(flag);
    }
    Error err(Error::Kind::TypeError, "`isnumeric()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// -*- str.isupper()
Self Object::isupper(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "`isupper()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = *args[0];
    if(self.is_string()){
        auto flag = true;
        auto str = static_cast<Str>(self);
        for(auto ptr=str.cbegin(); ptr != str.cend(); ptr++){
            auto c = *ptr;
            if(!std::isupper(c)){
                flag = false;
                break;
            }
        }
        return std::make_shared<Object>(flag);
    }
    Error err(Error::Kind::TypeError, "`isupper()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// -*-
Self Object::islower(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "`islower()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = *args[0];
    if(self.is_string()){
        auto flag = true;
        auto str = static_cast<Str>(self);
        for(auto ptr=str.cbegin(); ptr != str.cend(); ptr++){
            auto c = *ptr;
            if(!std::islower(c)){
                flag = false;
                break;
            }
        }
        return std::make_shared<Object>(flag);
    }
    Error err(Error::Kind::TypeError, "`islower()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// -*-
Self Object::isspace(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "`isspace()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = *args[0];
    if(self.is_string()){
        auto flag = true;
        auto str = static_cast<Str>(self);
        for(auto ptr=str.cbegin(); ptr != str.cend(); ptr++){
            auto c = *ptr;
            if(!std::isspace(c)){
                flag = false;
                break;
            }
        }
        return std::make_shared<Object>(flag);
    }
    Error err(Error::Kind::TypeError, "`isspace()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// -*-
Self Object::ltrim(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "`ltrim()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = *args[0];
    if(self.is_string()){
        auto str = static_cast<Str>(self);
        str.erase(
            str.begin(), std::find_if(
            str.begin(), str.end(),
            [](unsigned char ch){ return ch != ' '; })
        );
        return std::make_shared<Object>(str);
    }
    Error err(Error::Kind::TypeError, "`ltrim()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}


// -*-
Self Object::rtrim(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "`ltrim()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = *args[0];
    if(self.is_string()){
        auto str = static_cast<Str>(self);
        str.erase(
            std::find_if(
                str.rbegin(), str.rend(),
                [](unsigned char ch){ return ch != ' '; }
            ).base(),
            str.end()
        );
        return std::make_shared<Object>(str);
    }
    Error err(Error::Kind::TypeError, "`ltrim()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// -*-
Self Object::trim(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "`ltrim()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = args[0];
    if(self->is_string()){
        auto str = static_cast<Str>(*self);
        self = Object().ltrim(args);
        args = {};
        args.push_back(std::make_shared<Object>(*self));
        self = Object().rtrim(args);
        return std::make_shared<Object>(*self);
    }
    Error err(Error::Kind::TypeError, "`ltrim()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// ---------------------------------
// -*- String Specific Operators -*-
// ---------------------------------
Self Object::index(Args args){
    if(!check_argcount(args, 2)){
        Error err(Error::Kind::ValueError, "`index()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = args[0];
    if(self->is_string() || self->is_list() || self->is_tuple()){
        self = Object().find(args);
        return std::make_shared<Object>(*self);
    }
    Error err(Error::Kind::TypeError, "`index()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// -*- list.remove(item), hset.remove(key)
Self Object::remove(Args args){
    if(!check_argcount(args, 2)){
        Error err(Error::Kind::ValueError, "`remove()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = args[0];
    if(self->is_list()){
        auto vec = std::get<List>(self->m_value);
        Vec<Object> data{};
        for(const auto& x: vec){
            data.push_back(Object(*x));
        }
        auto item = *args[1];
        for(auto ptr=data.begin(); ptr != data.end(); ptr++){
            auto obj = *ptr;
            if(obj == item){
                data.erase(ptr);
                break;
            }
        }
        vec = {};
        for(const auto& x: data){
            vec.push_back(std::make_shared<Object>(x));
        }
        return std::make_shared<Object>(Object::Kind::Vector, vec);
    }else if(self->is_hashset()){
        auto hset = std::get<Set>(self->m_value);
        auto key = args[1];
        hset.remove(key);
        return std::make_shared<Object>(hset);
    }
    Error err(Error::Kind::TypeError, "`remove()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}


// -*- list.insert(idx, item), hset.insert(key)
Self Object::insert(Args args){
    if(args.size() < 2 || args.size() > 3){
        Error err(Error::Kind::ValueError, "`insert()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = args[0];
    if(check_argcount(args, 2)){ // hset.insert(key)
        if(!self->is_hashset()){
            Error err(Error::Kind::ValueError, "`insert()`: incorrect method call.");
            return std::make_shared<Object>(Result(err));
        }
        auto hset = std::get<Set>(self->m_value);
        auto key = args[1];
        hset.insert(key);
        return std::make_shared<Object>(hset);
    }

    if(check_argcount(args, 3)){ // list.insert(idx, item)
        if(!self->is_list()){
            Error err(Error::Kind::ValueError, "`insert()`: incorrect method call.");
            return std::make_shared<Object>(Result(err));
        }
        auto obj = *self;
        auto key = args[1];
        if(!key->is_integer()){
            Error err(Error::Kind::TypeError, "`insert()`: invalid argument type for index.");
            return std::make_shared<Object>(Result(err));
        }
        auto idx = static_cast<int>(std::get<i64>(key->m_value));
        obj[idx] = args[2];
        
        return std::make_shared<Object>(obj);
    }
    
    Error err(Error::Kind::TypeError, "`insert()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// -*- list.head()
Self Object::head(Args args){
    if(!check_argcount(args, 0)){
        Error err(Error::Kind::ValueError, "`head()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = args[0];
    if(!self->is_list()){
        Error err(Error::Kind::SyntaxError, "`head()`: incorrect method call.");
        return std::make_shared<Object>(Result(err));
    }else{
        auto vec = std::get<List>(self->m_value);
        if(vec.size()==0){
            Error err(Error::Kind::ValueError, "`head()`: invalid method call.");
            return std::make_shared<Object>(Result(err));
        }
        auto obj = *self;
        auto item = obj[0];
        return std::make_shared<Object>(*item);
    }
    Error err(Error::Kind::TypeError, "`head()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// -*- list.tail() -> list
Self Object::tail(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "`tail()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = args[0];
    if(!self->is_list()){
        Error err(Error::Kind::TypeError, "`tail()`: incorrect method call.");
        return std::make_shared<Object>(Result(err));
    }else{
        auto vec = std::get<List>(self->m_value);
        if(vec.size()==0){
            Error err(Error::Kind::ValueError, "`tail()`: empty list.");
            return std::make_shared<Object>(Result(err));
        }else if(vec.size()==1){
            List xs{};
            return std::make_shared<Object>(Object::Kind::Vector, xs);
        }
        List data{};
        auto ptr = vec.cbegin() + 1;
        while(ptr!=vec.cend()){
            auto x = *ptr;
            auto obj = *x;
            data.push_back(std::make_shared<Object>(obj));
        }
        return std::make_shared<Object>(Object::Kind::Vector, data);
    }
    Error err(Error::Kind::TypeError, "`tail()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// -*- list.last() -> Self
Self Object::last(Args args){
    if(!check_argcount(args, 1)){
        Error err(Error::Kind::ValueError, "`last()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = args[0];
    if(!self->is_list()){
        Error err(Error::Kind::TypeError, "`last()`: incorrect method call.");
        return std::make_shared<Object>(Result(err));
    }else{
        auto vec = std::get<List>(self->m_value);
        if(vec.size()==0){
            Error err(Error::Kind::ValueError, "`last()`: empty list.");
            return std::make_shared<Object>(Result(err));
        }
        auto item = vec[vec.size()-1];
        auto obj = *item;
        return std::make_shared<Object>(obj);
    }
    Error err(Error::Kind::TypeError, "`last()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// -*- list.pop(), list.pop(idx)
Self Object::pop(Args args){
    if(args.size() == 0 || args.size() > 2){
        Error err(Error::Kind::ValueError, "`pop()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = args[0];
    if(!self->is_list()){
        Error err(Error::Kind::TypeError, "`pop()`: incorrect method call.");
        return std::make_shared<Object>(Result(err));
    }else{
        auto vec = std::get<List>(self->m_value);
        if(vec.size()==0){
            Error err(Error::Kind::ValueError, "`pop()`: cannot pop empty list.");
            return std::make_shared<Object>(Result(err));
        }
        Object item{};
        if(args.size()==1){
            vec.erase(vec.begin());
        }else{
            auto key = *args[1];
            auto idx = static_cast<int>(static_cast<i64>(key));
            vec.erase((vec.begin()+idx));
        }
        return std::make_shared<Object>(Object::Kind::Vector, vec);
    }
    Error err(Error::Kind::TypeError, "`pop()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}


// -*- list.push(item)
Self Object::push(Args args){
    if(!check_argcount(args, 2)){
        Error err(Error::Kind::ValueError, "`push()`: invalid number of arguments.");
        return std::make_shared<Object>(Result(err));
    }
    auto self = args[0];
    if(!self->is_list()){
        Error err(Error::Kind::TypeError, "`push()`: incorrect method call.");
        return std::make_shared<Object>(Result(err));
    }else{
        auto vec = std::get<List>(self->m_value);
        auto item = *args[1];
        vec.push_back(std::make_shared<Object>(item));
        return std::make_shared<Object>(Object::Kind::Vector, vec);
    }
    Error err(Error::Kind::TypeError, "`push()`: invalid method call.");
    return std::make_shared<Object>(Result(err));
}

// --------------------------------
// -*- HashMap specific methods -*-
// --------------------------------
Self Object::keys(Args args){
    if(check_argcount(args, 1)){
        std::stringstream stream;
        stream << "ValueError: incorrent number of arguments. ";
        stream << "`.keys()` expects 0 argument.";
        throw std::runtime_error(stream.str());
    }
    auto self = args[0];
    if(!self->is_hashmap()){
        std::stringstream stream;
        stream << "SyntaxError: `.keys()` method is not supported.\n";
        stream << "expected a HashMap object, but got object of type '";
        stream << self->type().m_data << "'";
        throw std::runtime_error(stream.str());
    }
    auto dict = std::get<Dict>(self->m_value);
    auto xs = dict.keys();
    auto vec = std::get<List>(xs->m_value);
    return std::make_shared<Object>(Object::Kind::Tuple, vec);
}

// -*-
Self Object::values(Args args){
    if(check_argcount(args, 1)){
        std::stringstream stream;
        stream << "ValueError: incorrent number of arguments. ";
        stream << "`.values()` expects 0 argument.";
        throw std::runtime_error(stream.str());
    }
    auto self = args[0];
    if(!self->is_hashmap()){
        std::stringstream stream;
        stream << "SyntaxError: `.values()` method is not supported.\n";
        stream << "expected a HashMap object, but got object of type '";
        stream << self->type().m_data << "'";
        throw std::runtime_error(stream.str());
    }
    auto dict = std::get<Dict>(self->m_value);
    auto xs = dict.values();
    auto vec = std::get<List>(xs->m_value);
    return std::make_shared<Object>(Object::Kind::Tuple, vec);
}

// -*-
Self Object::items(Args args){
    // assumes args.size() == 1
    if(check_argcount(args, 1)){
        std::stringstream stream;
        stream << "ValueError: incorrent number of arguments. ";
        stream << "`.items()` expects 0 argument.";
        throw std::runtime_error(stream.str());
    }
    auto self = args[0];
    if(!self->is_hashmap()){
        std::stringstream stream;
        stream << "SyntaxError: `.items()` method is not supported.\n";
        stream << "expected a HashMap object, but got object of type '";
        stream << self->type().m_data << "'";
        throw std::runtime_error(stream.str());
    }
    auto dict = std::get<Dict>(self->m_value);
    auto xs = dict.items();
    auto vec = std::get<List>(xs->m_value);
    return std::make_shared<Object>(Object::Kind::Tuple, vec);
}

// -*-
Self Object::popitem(Args args){
    if(check_argcount(args, 2)){
        std::stringstream stream;
        stream << "ValueError: incorrent number of arguments. ";
        stream << "`.popitem()` expects 1 argument.";
        throw std::runtime_error(stream.str());
    }
    auto self = args[0];
    auto key = args[1];
    if(!self->is_hashmap()){
        std::stringstream stream;
        stream << "SyntaxError: `.items()` method is not supported.\n";
        stream << "expected a HashMap object, but got object of type '";
        stream << self->type().m_data << "'";
        throw std::runtime_error(stream.str());
    }
    auto dict = std::get<Dict>(self->m_value);
    auto xs = dict.popitem(key);
    auto keyval = *xs;
    return std::make_shared<Object>(keyval);
}

// -*- dict.update(key, val)
Self Object::update(Args args){
    if(check_argcount(args, 3)){
        std::stringstream stream;
        stream << "ValueError: incorrent number of arguments. ";
        stream << "`.update()` expects 2 arguments.";
        throw std::runtime_error(stream.str());
    }
    auto self = args[0];
    auto key = args[1];
    auto val = args[2];
    if(!self->is_hashmap()){
        std::stringstream stream;
        stream << "SyntaxError: `.update()` method is not supported.\n";
        stream << "expected a HashMap object, but got object of type '";
        stream << self->type().m_data << "'";
        throw std::runtime_error(stream.str());
    }
    auto dict = std::get<Dict>(self->m_value);
    auto xs = dict.update(key, val);
    auto keyval = *xs;
    return std::make_shared<Object>(keyval);
}

// -*- hasattr(obj, "str") -> bool
Self Object::hasattr(Args args){
    if(check_argcount(args, 2)){
        std::stringstream stream;
        stream << "ValueError: incorrent number of arguments. ";
        stream << "`hasattr()` expects 2 arguments.";
        throw std::runtime_error(stream.str());
    }
    auto self = args[0];
    auto key = Symbol(static_cast<Str>(*args[1]));
    bool flag{false};
    if(self->is_bool()){
        if(Lynx::boolMethods.find(key) != Lynx::boolMethods.end()){
            flag = true;
        }
    }else if(self->is_integer()){
        if(Lynx::integerMethods.find(key) != Lynx::integerMethods.end()){
            flag = true;
        }
    }else if(self->is_float()){
        if(Lynx::floatMethods.find(key) != Lynx::floatMethods.end()){
            flag = true;
        }
    }else if(self->is_complex()){
        if(Lynx::complexMethods.find(key) != Lynx::complexMethods.end()){
            flag = true;
        }
    }else if(self->is_string()){
        if(Lynx::stringMethods.find(key) != Lynx::stringMethods.end()){
            flag = true;
        }
    }else if(self->is_tuple()){
        if(Lynx::tupleMethods.find(key) != Lynx::tupleMethods.end()){
            flag = true;
        }
    }else if(self->is_list()){
        if(Lynx::listMethods.find(key) != Lynx::listMethods.end()){
            flag = true;
        }
    }else if(self->is_hashset()){
        if(Lynx::hashsetMethods.find(key) != Lynx::hashsetMethods.end()){
            flag = true;
        }
    }else if(self->is_hashmap()){
        if(Lynx::hashmapMethods.find(key) != Lynx::hashmapMethods.end()){
            flag = true;
        }
    }else if(self->is_structure()){
        auto attr = key.m_data;
        auto mystruct = static_cast<Structure>(*self);

        if(attr.substr(0, 2)==Str("::")){
            //! cls_attribute :=  "::name"
            auto name = attr.substr(2);
            auto attributes = mystruct.attributes();
            if(attributes.find(name) != attributes.end()){ flag = true; }
        }else if(attr[0]==':'){
            //! cls_obj_attribute := ":name"
            auto name = attr.substr(1);
            auto attributes = mystruct.object_attribues();
            if(attributes.find(name) != attributes.end()){ flag = true; }
        }else if(attr[0]=='#'){
            //! cls_property := "#name"
            auto name = attr.substr(1);
            auto properties = mystruct.properties();
            if(properties.find(name) != properties.end()){ flag = true; }
        }else{
            //! cls_method := "name"
            auto methods = mystruct.methods();
            if(methods.find(attr) != methods.end()){ flag = true; }
        }
    }
    return std::make_shared<Object>(flag);
}

// -------------------------------
// -*- Result specific methods -*-
// -------------------------------
// -*- result.ok()
Self Object::ok(Args args){
    if(check_argcount(args, 1)){
        std::stringstream stream;
        stream << "ValueError: `.ok()` : incorrect number of arguments.\n";
        stream << "Expect 0 argument.";
        std::runtime_error(stream.str());
    }
    auto self = args[0];
    if(!self->is_result()){
        std::stringstream stream;
        stream << "SyntaxError: `.ok()` : method is not supported.\n";
        std::runtime_error(stream.str());
    }
    auto result = std::get<Result>(self->m_value);
    if(!result.is_ok()){
        std::stringstream stream;
        stream << "ValueError: `.ok()` : result value wraps an errors.\n";
        std::runtime_error(stream.str());
    }
    auto val = result.ok();
    return std::make_shared<Object>(*val);
}

// -*- result.ok_or(obj)
Self Object::ok_or(Args args){
    if(check_argcount(args, 2)){
        std::stringstream stream;
        stream << "ValueError: `.ok_or()` : incorrect number of arguments.\n";
        stream << "Expect 1 argument.";
        std::runtime_error(stream.str());
    }
    auto self = args[0];
    if(!self->is_result()){
        std::stringstream stream;
        stream << "SyntaxError: `.ok_or()` : method is not supported.\n";
        std::runtime_error(stream.str());
    }
    auto obj = *args[1];
    auto result = std::get<Result>(self->m_value);
    Self val = nullptr;
    if(!result.is_ok()){
        val = std::make_shared<Object>(obj);
    }else{
        val = result.ok();
    }
    return std::make_shared<Object>(*val);
}

// -*-
/*
result.expect(msg)
result.expect(ErrorSymbol)
result.expect(ErrorSymbol, msg)
*/
Self Object::expect(Args args){
    if(args.size()==0 || args.size() > 3){
        std::stringstream stream;
        stream << "ValueError: `.expect()` : incorrect number of arguments.\n";
        stream << "Expect at least 1 argument and at most 2.";
        std::runtime_error(stream.str());
    }
    auto self = args[0];
    if(!self->is_result()){
        std::stringstream stream;
        stream << "SyntaxError: `.expect()` : method is not supported.\n";
        std::runtime_error(stream.str());
    }
    Object obj{};
    if(args.size()==2){
        auto msg = args[1];
        if(msg->is_symbol()){
            auto sym = std::get<Symbol>(msg->m_value);
            Error err(sym, "`.expect()`: an error occured");
            obj = Object(Result(err));
        }else if(msg->is_string()){
            auto sym = std::get<Str>(msg->m_value);
            Error err(Error::Kind::Error, sym);
            obj = Object(Result(err));
        }else{
            std::stringstream stream;
            stream << "TypeError: `.expect()`: invalid argument type";
            std::runtime_error(stream.str());
        }
    }else if(args.size()==3){
        auto sym = args[1];
        auto msg = args[2];
        if((!sym->is_symbol()) && (!msg->is_string())){
            std::stringstream stream;
            stream << "SyntaxError: `.expect()`: invalid arguments type";
            std::runtime_error(stream.str());
        }
        auto _sym = std::get<Symbol>(sym->m_value);
        auto _msg = std::get<Str>(msg->m_value);
        Error err(_sym, _msg);
        obj = Object(Result(err));
    }
    return std::make_shared<Object>(obj);
}

// -*-
std::ostream& operator<<(std::ostream& os, const Object& obj){
    Args args{}, argv{};
    args.push_back(std::make_shared<Object>(obj));
    args.push_back(std::make_shared<Object>(Object("__string__")));

    argv.push_back(std::make_shared<Object>(obj));
    argv.push_back(std::make_shared<Object>(Object("__repr__")));

    if(static_cast<bool>(*Object().hasattr(args))){
        auto method = Str{"__string__"};
        auto mystr = Object()(method, args);
        auto str = static_cast<Str>(*mystr);
        os << str;
    }else if(static_cast<bool>(*Object().hasattr(argv))){
        auto method = Str{"__repr__"};
        auto mystr = Object()(method, args);
        auto str = static_cast<Str>(*mystr);
        os << str;
    }else{
        std::stringstream stream;
        stream << std::hex << &obj;
        os << stream.str();
    }
    return os;

}

/*
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
// typecat-operators & constructors
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