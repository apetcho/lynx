#include "lynx.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::lynx                                       -*-
// -*----------------------------------------------------------------*-
namespace lynx{
// -
/**
 * @brief Lynx string-formatter
 * 
 * The formater is used by the builtin function `format`
 * 
 */
struct Formatter{
    explicit Formatter(const Str& str) noexcept;
    LYNX_DECLARE_MOVE(Formatter);
    ~Formatter();
    Str operator()(const Lynx& interp);

    // -*-
    enum class Kind{
        LBrace,     // '{'
        RBrace,     // '}'
        Expr,       // expr
        Colon,      // ':'
        Dot,        // '.',
        FSpec,      // FormatSpec: 'i', 'b', 'o', 'x' , 'f', 'e', 'E', 's', 'a', 'r'
        Ident,      // identifier
        Any,        // any-other-character-string
    };
    /*
    FormatSpecs:
        - Integer: i, [0][w]i, b, x
        - Float: f, e, E, [0][w][.][p](f|e|E)
        - String: s
        - Generic: a
        - Parseable-string: r 
    */
    struct Token{
        Formatter::Kind kind;
        Str lexme;
    };
};
// -
/*

Self fn_input(Args args);
Self fn_println(Args args);
Self fn_print(Args args);
Self fn_eprintln(Args args);
Self fn_eprint(Args args);
Self fn_panic(Args args);
Self fn_format(Args args);
Self fn_chr(Args args);
Self fn_ord(Args args);
Self fn_hasattr(Args args);
Self fn_all(Args args);
Self fn_any(Args args);
Self fn_filter(Args args);
Self fn_reduce(Args args);
Self fn_map(Args args);
Self fn_zip(Args args);
Self fn_foldl(Args args);
Self fn_foldr(Args args);
Self fn_sum(Args args);
Self fn_product(Args args);
Self fn_range(Args args);
Self fn_isCallable(Args args);
Self fn_isMacro(Args args);
Self fn_isInstance(Args args);
Self fn_typeof(Args args);
Self fn_addressof(Args args);
Self fn_bool(Args args);
Self fn_integer(Args args);
Self fn_float(Args args);
Self fn_complex(Args args);
Self fn_polar(Args args);
Self fn_string(Args args);
Self fn_tuple(Args args);
Self fn_list(Args args);
Self fn_hashset(Args args);
Self fn_hashmap(Args args);
Self fn_iterator(Args args);
Self fn_ok(Args args);
Self fn_err(Args args);
Self fn_eval(Args args);

<Module::random>
Self fn_random(Args args);
random::Random(seed)
random::Random(seed, generator)
random::Random(seed, distribution)
random::Random(seed, generator, distribution)
random::Random(seed, distribution, generator)
    generators:
        MersenneTwister
        Ranlux24
        Ranlux48
        Knuth
    distributions:
        Uniform
        Bernoulli
        Binomial
        Geometric
        Poisson
        Exponential
        Gamma
        Weibull
        Normal
        LogNormal
        ChiSquare
        Cauchy
        FisherF
        StudentT
myrandom.nextInteger()
myrandom.nextFloat()
random::shuffle(List)
random::sample()
random::sample(seed, generator, distribution)
random::sample(seed, distribution, generator)
random::reset(seed)
random::reset(seed, generator, distribution)
random::reset(seed, distribution, generator)

<Module::math>
Self fn_abs(Args args);
Self fn_min(Args args);
Self fn_max(Args args);
Self fn_ceil(Args args);
Self fn_floor(Args args);
Self fn_round(Args args);
Self fn_trunc(Args args);
Self fn_sin(Args args);
Self fn_cos(Args args);
Self fn_tan(Args args);
Self fn_asin(Args args);
Self fn_acos(Args args);
Self fn_atan(Args args);
Self fn_atan2(Args args);
Self fn_sinh(Args args);
Self fn_cosh(Args args);
Self fn_tanh(Args args);
Self fn_asinh(Args args);
Self fn_acosh(Args args);
Self fn_atanh(Args args);
Self fn_exp(Args args);
Self fn_ln(Args args);
Self fn_log(Args args);
Self fn_pow(Args args);

std::map<Str, BuiltinFn>
Lynx::initialize_builtins(void){
}
*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::lynx                                         -*-
// -*----------------------------------------------------------------*-