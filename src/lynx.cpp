#include "lynx.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::lynx                                       -*-
// -*----------------------------------------------------------------*-
namespace lynx{
// -
// --------------
// -*- Object -*-
// --------------
Object::Object(): m_type{Lynx::type("nil")}{}

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