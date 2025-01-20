#include "lynx.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::lynx                                       -*-
// -*----------------------------------------------------------------*-
namespace lynx{
// -
/*
// OPERATORS
[01] negate: -x
[02] add: x + y
[03] sub: x - y
[04] div: x / y
[05] mul: x * y
[06] mod: x % y
[07] add_assign: x += y
[08] sub_assign: x -= y
[09] div_assign: x /= y
[10] mul_assign: x *= y
[11] mod_assign: x %= y
[12] or: x or y
[13] and: x and y
[14] not: not x
[15] equal: x == y
[16] not_equal: x != y
*/
namespace type{
// -
/*
nil_str
nil_repr
nil_bool
nil_equal
nil_not_equal
*/
class Nil;

/*
bool_str
bool_repr
bool_int
bool_float
bool_not
bool_or
bool_and
bool_equal
bool_not_equal
*/
class Bool;

/*
int_str
int_repr
int_bool
int_int
int_float
int_equal
int_not_equal
int_negate
int_add
int_add_assign
int_sub
int_sub_assign
int_div
int_div_assign
int_mul
int_mul_assign
int_mod
int_mod_assign
*/
class Integer;

/*
float_str
float_repr
float_bool
float_int
float_float
float_negate
float_equal
float_not_equal
float_add
float_add_assign
float_sub
float_sub_assign
float_div
float_div_assign
float_mul
float_mul_assign
float_mod
float_mod_assign
*/
class Float;

/*
string_str
string_repr
string_bool
string_int
string_float
string_equal
string_not_equal
string_add
string_add_assign
string_lower
string_upper
string_join
string_replace
string_substr
string_capitalize()
string_title()
string_ltrim
string_rtrim
string_trim
string_replace_all()
string_split
string_startswith
string_endswith
*/
class String;

/*
array_str
array_repr
array_bool
array_int
array_float
array_add
array_add_assign
array_get
array_put
array_len
array_begin
array_end
*/
class Array;

/*
dict_str
dict_repr
dict_bool
dict_int
dict_float
dict_get
dict_put
dict_update
dict_len
dict_begin
dict_end
dict_keys
dict_values
dict_items
*/
class Dict;

/*
file_bool
file_read
file_readln
file_readlines
file_write
file_writeln
file_writelines
file_len
file_get_position
file_set_position
file_rewind
file_close
file_closed
*/
class File;

/*
callable_str
callable_repr
callable_bool
*/
class Callable;
// -
}

// std::map<Str, std::unique_ptr<Type>> initialize_types(void);

// -*----------------------------------------------------------------*-
}//-*- end::namespace::lynx                                         -*-
// -*----------------------------------------------------------------*-