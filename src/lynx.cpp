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
: Object(self.m_type), m_value{std::move(self.m_value)}{}



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