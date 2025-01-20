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