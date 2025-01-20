#include "lynx.hpp"
#include<iomanip>

// -*----------------------------------------------------------------*-
// -*- begin::namespace::lynx                                       -*-
// -*----------------------------------------------------------------*-
namespace lynx{
// -
// --------------
// -*- Object -*-
// --------------
Object::Object(): m_type{Lynx::type("Nil")}{}

// -*-
Object::Object(Type* type): m_type{type}{}

// -*-
Object::Object(const Object& self): m_type{self.m_type}{}

// -*-
Object& Object::operator=(const Object& self){
    if(this != &self){
        this->m_type = self.m_type;
    }
    return *this;
}

// -*-
Object::Object(Object&& self): m_type{self.m_type}{}

// -*-
Object& Object::operator=(Object&& self){
    if(this != &self){
        this->m_type = self.m_type;
    }
    return *this;
}

// ---------------
// -*- Boolean -*-
// ---------------
Boolean::Boolean()
: Object(Lynx::type("Bool")), m_value{false}{}

Boolean::Boolean(bool val)
: Object(Lynx::type("Bool")), m_value{val}{}

Boolean::Boolean(const Boolean& self)
: Object(self.m_type), m_value{self.m_value}{}

Boolean& Boolean::operator=(const Boolean& self){
    if(this != &self){
        this->m_type = self.m_type;
        this->m_value = self.m_value;
    }
    return *this;
}

Boolean::Boolean(Boolean&& self)
: Object(std::move(self.m_type)), m_value{std::move(self.m_value)}{}

Boolean& Boolean::operator=(Boolean&& self){
    if(this != &self){
        this->m_type = std::move(self.m_type);
        this->m_value = std::move(self.m_value);
    }
    return *this;
}

Str Boolean::str(void) const{
    return this->m_value ? Str{"true"} : Str{"false"};
}

Str Boolean::repr(void) const{
    std::stringstream stream;
    stream << std::quoted(this->str());
    return stream.str();
}

Boolean::operator bool(){
    return this->m_value;
}

Boolean::operator i64(){
    return this->m_value ? i64(1) : i64(0);
}

Boolean::operator f64(){
    return this->m_value ? f64{1.0} : f64{0.0};
}

Boolean::operator Str(){
    return this->str();
}

// -*-----------*-
// -*- Integer -*-
// -*-----------*-
Integer::Integer()
: Object(Lynx::type("Integer")), m_num(0){}

Integer::Integer(i64 num)
: Object(Lynx::type("Integer")), m_num{num}{}

Integer::Integer(const Integer& self)
: Object(self.m_type), m_num{self.m_num}{}

Integer::Integer(Integer&& self)
: Object(std::move(self.m_type)), m_num{std::move(self.m_num)}{}

Integer& Integer::operator=(const Integer& self){
    if(this != &self){
        this->m_type = self.m_type;
        this->m_num = self.m_num;
    }
    return *this;
}

Integer& Integer::operator=(Integer&& self){
    if(this != &self){
        this->m_type = std::move(self.m_type);
        this->m_num = std::move(self.m_num);
    }
    return *this;
}

Str Integer::str(void) const{
    std::stringstream stream;
    stream << this->m_num;
    return stream.str();
}

Str Integer::repr(void) const{
    std::stringstream stream;
    stream << std::quoted(this->str());
    return stream.str();
}

Integer::operator bool(){
    return this->m_num==0? false : true;
}

Integer::operator i64(){
    return this->m_num;
}

Integer::operator f64(){
    return static_cast<f64>(this->m_num);
}

Integer::operator Str(){
    return this->str();
}

// -*---------*-
// -*- Float -*-
// -*---------*-
Float::Float()
: Object(Lynx::type("Float")), m_num{0.0}{}

Float::Float(f64 num)
: Object(Lynx::type("Float")), m_num{num}{}

Float::Float(const Float& self)
: Object(self.m_type), m_num{self.m_num}{}

Float::Float(Float&& self)
: Object(std::move(self.m_type)), m_num{std::move(self.m_num)}{}

Float& Float::operator=(const Float& self){
    if(this != &self){
        this->m_type = self.m_type;
        this->m_num = self.m_num;
    }

    return *this;
}

Float& Float::operator=(Float&& self){
    if(this != &self){
        this->m_type = std::move(self.m_type);
        this->m_num = std::move(self.m_num);
    }

    return *this;
}

Str Float::str(void) const{
    std::stringstream stream;
    stream << this->m_num;
    return stream.str();
}

Str Float::repr(void) const{
    std::stringstream stream;
    stream << std::quoted(this->str());
    return stream.str();
}

Float::operator bool(){
    return this->m_num==0.0 ? false : true;
}

Float::operator i64(){
    return static_cast<i64>(this->m_num);
}

Float::operator f64(){
    return this->m_num;
}

Float::operator Str(){
    return this->str();
}

// -*----------*-
// -*- Symbol -*-
// -*----------*-
Symbol::Symbol(const Str& str)
: Object(Lynx::type("Symbol"))
, m_str{str}{}

Symbol::Symbol(const Symbol& sym)
: Object(sym.m_type), m_str{sym.m_str}{}

Symbol::Symbol(Symbol&& sym)
: Object(sym.m_type), m_str{sym.m_str}{}

Symbol& Symbol::operator=(const Symbol& sym){
    if(this != &sym){
        this->m_type = sym.m_type;
        this->m_str = sym.m_str;
    }
    return *this;
}

Symbol& Symbol::operator=(Symbol&& sym){
    if(this != &sym){
        this->m_type = std::move(sym.m_type);
        this->m_str = std::move(sym.m_str);
    }

    return *this;
}

Str Symbol::str(void) const{
    return this->m_str;
}

Str Symbol::repr(void) const{
    return this->m_str;
}

Symbol::operator bool(){ return true; }


// -*----------*-
// -*- String -*-
// -*----------*-
String::String()
: Object(Lynx::type("String")), m_str{""}{}

String::String(const Str& str)
: Object(Lynx::type("String")), m_str{str}{}

String::String(const String& self)
: Object(self.m_type), m_str{self.m_str}{}

String::String(String&& self)
: Object(std::move(self.m_type)), m_str{std::move(self.m_str)}{}

String& String::operator=(const String& self){
    if(this != &self){
        this->m_type = self.m_type;
        this->m_str = self.m_str;
    }
    return *this;
}

String& String::operator=(String&& self){
    if(this != &self){
        this->m_type = std::move(self.m_type);
        this->m_str = std::move(self.m_str);
    }

    return *this;
}

Str String::str(void) const {
    return this->m_str;
}

Str String::repr(void) const{
    std::stringstream stream;
    stream << std::quoted(this->m_str);
    return stream.str();
}

String::operator bool(){
    return true;
}

// -*---------*-
// -*- Array -*-
// -*---------*-
Array::Array()
: Object(Lynx::type("Array")), m_data{}{}

Array::Array(Vec<Self> vec)
: Object(Lynx::type("Array")), m_data{vec}{}

Array::Array(const Array& self)
: Object(self.m_type), m_data{self.m_data}{}

Array::Array(Array&& self)
: Object(std::move(self.m_type)), m_data{std::move(self.m_data)}{}

Array& Array::operator=(const Array& self){
    if(this != &self){
        this->m_type = self.m_type;
        this->m_data = self.m_data;
    }
    return *this;
}

Array& Array::operator=(Array&& self){
    if(this != &self){
        this->m_type = std::move(self.m_type);
        this->m_data = std::move(self.m_data);
    }
    return *this;
}

Str Array::str(void) const{
    std::stringstream stream;
    if(this->m_data.size() == 0){
        return "[]";
    }
    stream << "[";
    for(std::size_t i=0; i < this->m_data.size(); i++){
        stream << this->m_data[i]->str();
        if(i < this->m_data.size()-1){
            stream << ", ";
        }
    }
    stream << "]";
    return stream.str();
}

Str Array::repr(void) const{
    std::stringstream stream;
    stream << std::quoted(this->str());
    return stream.str();
}

Array::operator bool(){
    return true;
}

Vec<Self> Array::data(void) const{
    return this->m_data;
}

// -*--------*-
// -*- Dict -*-
// -*--------*-
bool Dict::Comparator::operator()(const Self& lhs, const Self& rhs) const{
    auto _lname = lhs->type()->name();
    auto _rname = rhs->type()->name();
    if(!(_lname == "Integer" || _lname == "String" || _lname == "Bool")){
        std::stringstream stream;
        stream << lhs->str() << " of type '" << _lname << "' is not hashable";
        throw TypeError(stream.str());
    }
    if(!(_rname == "Integer" || _rname == "String" || _rname == "Bool")){
        std::stringstream stream;
        stream << rhs->str() << " of type '" << _rname << "' is not hashable";
        throw TypeError(stream.str());
    }
    return (lhs->str() < rhs->str());
}

Dict::Dict()
: Object(Lynx::type("Dict")), m_data{}{}


Dict::Dict(std::map<Str, Self>& data)
: Object(Lynx::type("Dict")){
    for(auto entry: data){
        this->m_data[std::make_shared<Object>(String(entry.first))] = entry.second;
    }
}

Dict::Dict(const Dict& self)
: Object(self.m_type), m_data{self.m_data}{}

Dict::Dict(Dict&& self)
: Object(std::move(self.m_type)), m_data{std::move(self.m_data)}{}

Dict& Dict::operator=(const Dict& self){
    if(this != &self){
        this->m_type = self.m_type;
        this->m_data = self.m_data;
    }
    return *this;
}

Dict& Dict::operator=(Dict&& self){
    if(this != &self){
        this->m_type = self.m_type;
        this->m_data = self.m_data;
    }

    return *this;
}

Str Dict::str(void) const{
    std::stringstream stream;
    if(this->m_data.size() == 0){
        return "{}";
    }
    stream << "{";
    std::size_t len = 0;
    for(const auto& entry: this->m_data){
        stream << "[" << entry.first->str() << ", ";
        stream << entry.second->str() << "]";
        if(++len < this->m_data.size()-1){
            stream << ", ";
        }
    }
    stream << "]";
    return stream.str();
}

Str Dict::repr(void) const{
    std::stringstream stream;
    stream << std::quoted(this->str());
    return stream.str();
}

Dict::operator bool(){
    return true;
}

Dict::Map Dict::data(void) const{
    return this->m_data;
}

// -*--------*-
// -*- File -*-
// -*--------*-
File::File(): Object(Lynx::type("File"))
, m_filename{".untitled"}
, m_mode{"rw+"}{
    this->m_stream = std::move(
        std::fstream(this->m_filename,
        this->get_openmode(this->m_mode))
    );
}

File::File(const Str& filename)
: Object(Lynx::type("File"))
, m_filename{filename}
, m_mode{"r"} {
    this->m_stream = std::move(
        std::fstream(this->m_filename, this->get_openmode(this->m_mode))
    );
}

File::File(const Str& filename, const Str& mode)
: Object(Lynx::type("File"))
, m_filename{filename}
, m_mode{mode} {
    if(!this->check_openmode(mode)){
        throw ValueError("invalid file '" + mode + "'");
    }
    this->m_stream = std::move(
        std::fstream(this->m_filename, this->get_openmode(this->m_mode))
    );
}

File::File(File&& self)
: Object(std::move(self.m_type))
, m_filename{std::move(self.m_filename)}
, m_mode{std::move(self.m_mode)}
, m_stream{std::move(self.m_stream)}{}

// -*-
File& File::operator=(File&& self){
    if(this != &self){
        this->m_type = std::move(self.m_type);
        this->m_filename = std::move(self.m_filename);
        this->m_mode = std::move(self.m_mode);
        this->m_stream = std::move(self.m_stream);
    }

    return *this;
}

// read the whole file to string
Str File::str(void) const{
    std::stringstream stream;
    stream << "File::<" << this->m_filename << ">";
    stream << " @ " << std::hex << (&this->m_stream);
    return stream.str();
}

Str File::repr(void) const{
    std::stringstream stream;
    stream << std::quoted(this->str());
    return stream.str();
}

File::operator bool(){
    return true;
}

// -*-----------*-
// -*- Builtin -*-
// -*-----------*-
Self _fn_nil(Vec<Self> argv){
    LYNX_UNUSED(argv);
    return std::make_shared<Object>();
}
// -
Builtin::Builtin()
: Object(Lynx::type("Builtin"))
, m_name{}
, m_arity{}
, m_fn{nullptr}
, m_doc{} {}

Builtin::Builtin(const Str& name, int arity, BuiltinFn fn, Str doc)
: Object(Lynx::type("Builtin"))
, m_name{name}
, m_arity{arity}
, m_fn{fn}
, m_doc{doc}
{}

Builtin::Builtin(Builtin&& self)
: Object(std::move(self.m_type))
, m_name{std::move(self.m_name)}
, m_arity{std::move(self.m_arity)}
, m_fn{std::move(self.m_fn)}
, m_doc{std::move(self.m_doc)}
{}

Builtin& Builtin::operator=(Builtin&& self){
    if(this != &self){
        this->m_name = std::move(self.m_name);
        this->m_arity = std::move(self.m_arity);
        this->m_fn = std::move(self.m_fn);
        this->m_doc = std::move(self.m_doc);
    }
    return *this;
}

Str Builtin::str(void) const{
    std::stringstream stream;
    stream << "Builtin::" << this->name();
    stream << " @ " << std::hex << this->m_fn;
    return stream.str();
}

Str Builtin::repr(void) const{
    std::stringstream stream;
    stream << std::quoted(this->str());
    return stream.str();
}

Builtin::operator bool(){ return true; }

// -*------------*-
// -*- Function -*-
// -*------------*-
Function::Function()
: Object(Lynx::type("Function"))
, m_name{}
, m_params{}
, m_body{}
, m_scope{}
, m_doc{}
{}

Function::Function(const Str& name, Vec<Self> params, Vec<Self> body, Scope scope, Str doc)
: Object(Lynx::type("Function"))
, m_name{name}
, m_params{params}
, m_body{body}
, m_scope{scope}
, m_doc{doc}{}

Function::Function(Function&& func)
: Object(std::move(func.m_type))
, m_name{std::move(func.m_name)}
, m_params{std::move(func.m_params)}
, m_body{std::move(func.m_body)}
, m_scope{std::move(func.m_scope)}
, m_doc{std::move(func.m_doc)}{}

Function& Function::operator=(Function&& func){
    if(this != &func){
        this->m_name = std::move(func.m_name);
        this->m_params = std::move(func.m_params);
        this->m_body = std::move(func.m_body);
        this->m_scope = std::move(func.m_scope);
        this->m_doc = std::move(func.m_doc);
    }

    return *this;
}

Str Function::str(void) const{
    std::stringstream stream;
    stream << "Function::" << this->m_name;
    stream << " @ " << std::hex << &this->m_body;
    return stream.str();
}

Str Function::repr(void) const{
    std::stringstream stream;
    stream << std::quoted(this->str());
    return stream.str();
}

Function::operator bool(){
    return true;
}


// -*----------*-
// -*- Lambda -*-
// -*----------*-
Lambda::Lambda()
: Object(Lynx::type("Lambda"))
, m_params{}
, m_body{}
, m_scope{}{}

Lambda::Lambda(Vec<Self> params, Vec<Self> body, Scope scope)
: Object(Lynx::type("Lambda"))
, m_params{params}
, m_body{body}
, m_scope{scope}
{}

Lambda::Lambda(Lambda&& lambda)
: Object(std::move(lambda.m_type))
, m_params{std::move(lambda.m_params)}
, m_body{std::move(lambda.m_body)}
, m_scope{std::move(lambda.m_scope)}
{}

Lambda& Lambda::operator=(Lambda&& lambda){
    if(this != &lambda){
        this->m_params = std::move(lambda.m_params);
        this->m_body = std::move(lambda.m_body);
        this->m_scope = std::move(lambda.m_scope);
    }
    return *this;
}

Str Lambda::str(void) const{
    std::stringstream stream;
    stream << "Lambda @ " << std::hex << &this->m_body;
    return stream.str();
}

Str Lambda::repr(void) const{
    std::stringstream stream;
    stream << std::quoted(this->str());
    return stream.str();
}

// -*--------*-
// -*- Type -*-
// -*--------*-
Self Type::operator()(const Str& method, Self& self, Vec<Self> argv){
    auto iter = this->m_methods.find(method);
    if(iter == this->m_methods.end()){
        std::stringstream stream;
        stream << "Type '" << self->type()->name() << "' does not support ";
        stream << "method " << method;
        throw Error(stream.str());
    }
    auto fun = iter->second;
    return fun(self, argv);
}

// -*-------*-
// -*- Env -*-
// -*-------*-
Env::Env()
: m_bindings{}
, m_parent{nullptr}
{}

Env::Env(const Env& env)
: m_bindings{env.m_bindings}
, m_parent{env.m_parent}{}

Env::Env(Env&& env)
: m_bindings{std::move(env.m_bindings)}
, m_parent{std::move(env.m_parent)}{}

Env& Env::operator=(const Env& env){
    if(this != &env){
        this->m_bindings = env.m_bindings;
        this->m_parent = env.m_parent;
    }
    return *this;
}

Env& Env::operator=(Env&& env){
    if(this != &env){
        this->m_bindings = std::move(env.m_bindings);
        this->m_parent = std::move(env.m_parent);
    }
    return *this;
}

Env::Env(Scope& parent)
: m_bindings{}
, m_parent{parent}{}

Self Env::get(const Str& key) const{
    auto entry = this->m_bindings.find(key);
    if(entry != this->m_bindings.end()){
        return entry->second;
    }else if(this->m_parent != nullptr){
        return this->m_parent->get(key);
    }

    std::stringstream stream;
    stream << key << " not bound";
    throw ValueError(stream.str());
}

void Env::put(const Str& key, const Self& val){
    this->m_bindings[key] = val;
}



// -*----------------------------------------------------------------*-
}//-*- end::namespace::lynx                                         -*-
// -*----------------------------------------------------------------*-

// -*-----------------------------*-
// -*-   M A I N   D R I V E R   -*-
// -*-----------------------------*-
int main(int argc, char** argv){
    //! @todo
    return 0;
}