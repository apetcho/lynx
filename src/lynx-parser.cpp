#include "lynx.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::lynx                                       -*-
// -*----------------------------------------------------------------*-
namespace lynx{
// -

/*
class Parser final{
public:
    Parser() = default;
    LYNX_DECLARE_MOVE(Parser);
    LYNX_DELETE_COPY(Parser);
    ~Parser();
    Ast parse(void) const;

private:
    using Stream = std::variant<std::istream*, std::string>;
    bool m_fromStr;
    Stream m_stream;
    Vec<Token> m_tokens;
    Token m_current;

private:
    bool match(TokenKind kind, int index=0);
    void expect(TokenKind kind, int index=0);
    Token token(void);
    void advance(void);
    void skip(void);
    // -*-
    Ast parse_parameters(void);
    Ast parse_destructuring_expr(void);

    Ast parse_nil_literal(void);
    Ast parse_identifier(void);
    Ast parse_bool_literal(void);
    Ast parse_number_literal(void);
    Ast parse_symbol(void);
    Ast parse_string_literal(void);
    Ast parse_tuple_literal(void);
    Ast parse_list_literal(void);
    Ast parse_set_literal(void);
    Ast parse_dict_literal(void);
    Ast parse_result(void);
    Ast parse_fun_expr(void);
    Ast parse_lambda_expr(void);
    Ast parse_macro_expr(void);
    Ast parse_struct_expr(void);
    Ast parse_method_expr(void);
    // -
    Ast parse_get_key_expr(void);
    Ast parse_get_index_expr(void);
    Ast parse_slice_expr(void);
    // -
    Ast parse_ternary_expr(void);
    Ast parse_method_call_expr(void);
    Ast parse_when_expr(void);
    Ast parse_set_index_expr(void);
    Ast parse_set_key_expr(void);
    Ast parse_set_struct_attr_expr(void);
    Ast parse_set_struct_prop_expr(void);
    Ast parse_set_obj_attr_expr(void);
    Ast parse_set_slice_expr(void);
    Ast parse_struct_add_expr(void);
    Ast parse_struct_sub_expr(void);
    Ast parse_struct_div_expr(void);
    Ast parse_struct_mul_expr(void);
    Ast parse_struct_mod_expr(void);
    Ast parse_struct_pow_expr(void);
    Ast parse_struct_bit_or_expr(void);
    Ast parse_struct_bit_xor_expr(void);
    Ast parse_struct_bit_and_expr(void);
    Ast parse_struct_bit_shl_expr(void);
    Ast parse_struct_bit_shr_expr(void);
    Ast parse_struct_bit_or_assign_expr(void);
    Ast parse_struct_bit_xor_assign_expr(void);
    Ast parse_struct_bit_and_assign_expr(void);
    Ast parse_struct_bit_shl_assign_expr(void);
    Ast parse_struct_bit_shr_assign_expr(void);
    Ast parse_struct_add_assign_expr(void);
    Ast parse_struct_sub_assign_expr(void);
    Ast parse_struct_div_assign_expr(void);
    Ast parse_struct_mul_assign_expr(void);
    Ast parse_struct_mod_assign_expr(void);
    Ast parse_struct_pow_assign_expr(void);
    // -
    Ast parse_binary_expr(void);
    Ast parse_funcall_expr(void);
    Ast parse_logical_or_expr(void);
    Ast parse_logical_and_expr(void);
    Ast parse_in_expr(void);
    Ast parse_get_struct_attr_expr(void);
    Ast parse_get_struct_prop_expr(void);
    Ast parse_get_obj_attr_expr(void);
    Ast parse_add_expr(void);
    Ast parse_sub_expr(void);
    Ast parse_div_expr(void);
    Ast parse_mul_expr(void);
    Ast parse_mod_expr(void);
    Ast parse_pow_expr(void);
    Ast parse_add_assign_expr(void);
    Ast parse_sub_assign_expr(void);
    Ast parse_div_assogn_expr(void);
    Ast parse_mul_assign_expr(void);
    Ast parse_mod_assign_expr(void);
    Ast parse_pow_assign_expr(void);
    
    // -
    Ast parse_unary_expr(void);
    Ast parse_bit_not_expr(void);
    Ast parse_logical_not_expr(void);

    // -
    Ast parse_let_stmt(void);
    Ast parse_var_stmt(void);
    Ast parse_for_stmt(void);
    Ast parse_while_stmt(void);
    Ast parse_if_stmt(void);
    Ast parse_break_stmt(void);
    Ast parse_continue_stmt(void);
    Ast parse_return_stmt(void);
    Ast parse_block_stmt(void);
    Ast parse_stmt_list(void);
    Ast parse_expr_stmt(void);
    Ast parse_stmt(void);

    Ast parse_at_doc_expr(void);
    Ast parse_at_define_expr(void);
    Ast parse_at_testing_expr(void);
    Ast parse_at_test_expr(void);
    Ast parse_at_expr(void);
    Ast parse_at_setup_expr(void);
    Ast parse_at_teardown_expr(void);
    Ast parse_at_main_expr(void);
};
*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::lynx                                         -*-
// -*----------------------------------------------------------------*-