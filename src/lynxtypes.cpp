#include "lynx.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::lynx                                       -*-
// -*----------------------------------------------------------------*-
namespace lynx{
// -
/*
// OPERATORS
[01] __negate__: -x
[02] __add__: x + y
[03] __sub__: x - y
[04] __div__: x / y
[05] __mul__: x * y
[06] __mod__: x % y
[07] __add_assign__: x += y
[08] __sub_assign__: x -= y
[09] __div_assign__: x /= y
[10] __mul_assign__: x *= y
[11] __mod_assign__: x %= y
[12] __or__: x or y
[13] __and__: x and y
[14] __not__: not x
[15] __equal__: x == y
[16] __not_equal__: x != y
[17] __get__: self[x]
[18] __put__: self[x] = y

typedef Self (*Callback)(Self&, Vec<Self>)
*/
namespace type{
// -
// -*-
Self nil_str(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

// -*-
Self nil_repr(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

// -*-
Self nil_bool(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

// -*-
Self nil_equal(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

// -*-
Self nil_not_equal(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

HashMap<Str, Callback> _nilMethods = {
    {"__str__", nil_str},
    {"__repr__", nil_repr},
    {"__equal__", nil_equal},
    {"__bool__", nil_bool},
    {"__not_equal__", nil_not_equal},
};

class Nil final: public Type{
public:
    Nil(): Type{"Nile", std::move(_nilMethods)}{}
    ~Nil() = default;
};

// -*-----------*-
// -*- Boolean -*-
// -*-----------*-
Self bool_str(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self bool_repr(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self bool_int(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self bool_float(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self bool_not(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self bool_or(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self bool_and(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self bool_equal(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self bool_not_equal(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

HashMap<Str, Callback> _boolMethods = {
    {"__str__", bool_str},
    {"__repr__", bool_repr},
    {"__int__", bool_int},
    {"__float__", bool_float},
    {"__not__", bool_not},
    {"__or__", bool_or},
    {"__and__", bool_and},
    {"__equal__", bool_equal},
    {"__not_equal__", bool_not_equal},
};


class Bool final: public Type{
public:
    Bool(): Type{"Bool", std::move(_boolMethods)}{}
    ~Bool() = default;
};

// -*-----------*-
// -*- Integer -*-
// -*-----------*-
Self int_str(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_repr(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_bool(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_float(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_equal(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_not_equal(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_negate(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_add(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_sub(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_mul(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_div(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_mod(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_add_assign(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_sub_assign(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_mul_assign(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_div_assign(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self int_mod_assign(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

// -*-
HashMap<Str, Callback> _intMethods = {
    {"__str__", int_str},
    {"__repr__", int_repr},
    {"__bool__", int_bool},
    {"__float__", int_float},
    {"__equal__", int_equal},
    {"__not_equal__", int_not_equal},
    {"__negate__", int_negate},
    {"__add__", int_add},
    {"__sub__", int_sub},
    {"__mul__", int_mul},
    {"__div__", int_div},
    {"__mod__", int_mod},
    {"__add_assign__", int_add_assign},
    {"__sub_assign__", int_sub_assign},
    {"__mul_assign__", int_mul_assign},
    {"__div_assign__", int_div_assign},
    {"__mod_assign__", int_mod_assign},
};

class Integer final: public Type{
public:
    Integer(): Type{"Integer", std::move(_intMethods)}{}
    ~Integer() = default;
};

// -*---------*-
// -*- Float -*-
// -*---------*-
Self float_str(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_repr(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_bool(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_int(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_equal(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_not_equal(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_negate(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_add(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_sub(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_mul(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_div(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_mod(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_add_assign(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_sub_assign(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_mul_assign(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_div_assign(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self float_mod_assign(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

// -*-
HashMap<Str, Callback> _floatMethods = {
    {"__str__", float_str},
    {"__repr__", float_repr},
    {"__bool__", float_bool},
    {"__float__", float_int},
    {"__equal__", float_equal},
    {"__not_equal__", float_not_equal},
    {"__negate__", float_negate},
    {"__add__", float_add},
    {"__sub__", float_sub},
    {"__mul__", float_mul},
    {"__div__", float_div},
    {"__mod__", float_mod},
    {"__add_assign__", float_add_assign},
    {"__sub_assign__", float_sub_assign},
    {"__mul_assign__", float_mul_assign},
    {"__div_assign__", float_div_assign},
    {"__mod_assign__", float_mod_assign},
};

// -*-
class Float final: public Type{
public:
    Float(): Type{"Float", std::move(_floatMethods)}{}
    ~Float() = default;
};

// -*----------*-
// -*- String -*-
// -*----------*-
Self string_str(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_repr(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_bool(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_int(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_float(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_equal(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_not_equal(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_add(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_add_assign(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_lower(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_upper(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_join(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_replace(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_substr(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_capitalize(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_title(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_ltrim(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_rtrim(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_trim(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_replace_all(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_split(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_startswith(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_endswith(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_find(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_find_all(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_get(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_set(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_begin(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_end(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self string_len(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

HashMap<Str, Callback> _stringMethods = {
    {"__str__", string_str},
    {"__repr__", string_repr},
    {"__bool__", string_bool},
    {"__int__", string_int},
    {"__float__", string_float},
    {"__equal__", string_equal},
    {"__not_equal__", string_not_equal},
    {"__add__", string_add},
    {"__add_assign__", string_add_assign},
    {"__get__", string_get},
    {"__put__", string_set},
    {"lower", string_lower},
    {"upper", string_upper},
    {"join", string_join},
    {"replace", string_replace},
    {"replace_all", string_replace_all},
    {"substr", string_substr},
    {"capitalize", string_capitalize},
    {"title", string_title},
    {"ltrim", string_ltrim},
    {"rtrim", string_rtrim},
    {"trim", string_trim},
    {"split", string_split},
    {"startswith", string_startswith},
    {"endswith", string_endswith},
    {"find", string_find},
    {"find_all", string_find_all},
    {"begin", string_begin},
    {"end", string_end},
    {"len", string_len},
};

class String final: public Type{
public:
    String(): Type{"", std::move(_stringMethods)}{}
    ~String() = default;
};


// -*---------*-
// -*- Array -*-
// -*---------*-
Self array_str(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self array_repr(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self array_bool(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self array_add(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self array_add_assign(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self array_find(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self array_find_all(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self array_get(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self array_put(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self array_begin(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self array_end(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self array_len(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

HashMap<Str, Callback> _arrayMethods = {
    {"__str__", array_str},
    {"__repr__", array_repr},
    {"__bool__", array_bool},
    {"__add__", array_add},
    {"__add_assign__", array_add_assign},
    {"__get__", array_get},
    {"__put__", array_put},
    {"__begin__", array_put},
    {"__end__", array_put},
    {"len", array_len},
    {"find", array_find},
    {"find_all", array_find_all},
};

class Array final: public Type{
public:
    Array(): Type{"Array", std::move(_arrayMethods)}{}
    ~Array() = default;
};

// -*--------*-
// -*- Dict -*-
// -*--------*-
Self dict_str(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self dict_repr(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self dict_bool(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self dict_get(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self dict_put(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self dict_update(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self dict_len(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self dict_begin(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self dict_end(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self dict_keys(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self dict_values(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self dict_items(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self dict_contains(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self dict_add(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

Self dict_add_assign(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

HashMap<Str, Callback> _dictMethods = {
    {"__str__", dict_str},
    {"__repr__", dict_repr},
    {"__bool__", dict_bool},
    {"__get__", dict_get},
    {"__put__", dict_put},
    {"__begin__", dict_begin},
    {"__end__", dict_end},
    {"__add__", dict_add},
    {"__add_assign__", dict_add_assign},
    {"len", dict_len},
    {"keys", dict_keys},
    {"values", dict_values},
    {"items", dict_items},
    {"contains", dict_contains},
};

class Dict final: public Type{
public:
    Dict(): Type{"", std::move(_dictMethods)}{}
    ~Dict() = default;
};

// -*--------*-
// -*- File -*-
// -*--------*-
Self file_bool(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self file_read(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self file_readn(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self file_readln(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self file_readlines(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self file_write(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self file_writen(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self file_writeln(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self file_writelines(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self file_len(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self file_get_position(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self file_set_position(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self file_rewind(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self file_close(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self file_closed(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}

HashMap<Str, Callback> _fileMethods = {
    {"__bool__", file_bool},
    {"read", file_read},
    {"readn", file_readn},
    {"readln", file_readln},
    {"readlines", file_readlines},
    {"write", file_write},
    {"writen", file_writen},
    {"writeln", file_writeln},
    {"writelines", file_writelines},
    {"size", file_len},
    {"get_position", file_get_position},
    {"set_position", file_set_position},
    {"rewind", file_rewind},
    {"close", file_close},
    {"closed", file_closed},
};

class File final: public Type{
public:
    File(): Type{"", std::move(_fileMethods)}{}
    ~File() = default;
};

// -*-----------------------------*-
// -*- Builtin, Function, Lambda -*-
// -*-----------------------------*-
Self callable_str(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self callable_repr(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


Self callable_bool(Self& self, Vec<Self> argv){
    //! @todo
    return nullptr;
}


HashMap<Str, Callback> _builtinMethods = {
    {"__str__", callable_str},
    {"__repr__", callable_repr},
    {"__bool__", callable_bool},
};

HashMap<Str, Callback> _functionMethods = {
    {"__str__", callable_str},
    {"__repr__", callable_repr},
    {"__bool__", callable_bool},
};

HashMap<Str, Callback> _lambdaMethods = {
    {"__str__", callable_str},
    {"__repr__", callable_repr},
    {"__bool__", callable_bool},
};


class Builtin final: Type{
public:
    Builtin(): Type{"", std::move(_builtinMethods)}{}
    ~Builtin() = default;
};

class Function final: Type{
public:
    Function(): Type{"Function", std::move(_functionMethods)}{}
    ~Function() = default;
};

class Lambda final: Type{
public:
    Lambda(): Type{"Function", std::move(_lambdaMethods)}{}
    ~Lambda() = default;
};

// -
}

// std::map<Str, std::unique_ptr<Type>> initialize_types(void);

// -*----------------------------------------------------------------*-
}//-*- end::namespace::lynx                                         -*-
// -*----------------------------------------------------------------*-