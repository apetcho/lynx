#include "lynx.hpp"


#define LYNX_KEYWORDS()             \
    LYNX_DEF(Fun, "fun")            \
    LYNX_DEF(Lambda, "lambda")      \
    LYNX_DEF(Let, "let")            \
    LYNX_DEF(If, "if")              \
    LYNX_DEF(Else, "else")          \
    LYNX_DEF(Return, "return")      \
    LYNX_DEF(While, "while")        \
    LYNX_DEF(For, "for")            \
    LYNX_DEF(Break, "break")        \
    LYNX_DEF(Continue, "continue")  \
    LYNX_DEF(Import, "import")


#define LYNX_TOKENS()                   \
    LYNX_DEF(Invalid, "INVALID")        \
    LYNX_DEF(Eof, "EOF")                \
    LYNX_DEF(Ident, "IDENTIFIER")       \
    LYNX_DEF(True, "true")              \
    LYNX_DEF(False, "false")            \
    LYNX_DEF(Int, "INTEGER")            \
    LYNX_DEF(Float, "FLOAT")            \
    LYNX_DEF(String, "STRING")          \
    LYNX_DEF(Assign, "=")               \
    LYNX_DEF(Plus, "+")                 \
    LYNX_DEF(Minus, "-")                \
    LYNX_DEF(Mul, "*")                  \
    LYNX_DEF(Div, "/")                  \
    LYNX_DEF(Mod, "%")                  \
    LYNX_DEF(PlusAssign, "+=")          \
    LYNX_DEF(MinusAssign, "-=")         \
    LYNX_DEF(MulAssign, "*=")           \
    LYNX_DEF(DivAssign, "/=")           \
    LYNX_DEF(ModAssign, "%=")           \
    LYNX_DEF(Not, "not")                \
    LYNX_DEF(And, "and")                \
    LYNX_DEF(Or, "or")                  \
    LYNX_DEF(Less, "<")                 \
    LYNX_DEF(Greater, ">")              \
    LYNX_DEF(LessEqual, "<=")           \
    LYNX_DEF(GreaterEqual, ">=")        \
    LYNX_DEF(Equal, "==")               \
    LYNX_DEF(NotEqual, "!=")            \
    LYNX_DEF(QMark, "?")                \
    LYNX_DEF(Dot, ".")                  \
    LYNX_DEF(Comma, ",")                \
    LYNX_DEF(DoubleColon, "::")         \
    LYNX_DEF(Colon, ":")                \
    LYNX_DEF(Semicolon, ";")            \
    LYNX_DEF(LParen, "(")               \
    LYNX_DEF(RParen, ")")               \
    LYNX_DEF(LBrace, "{")               \
    LYNX_DEF(RBrace, "}")               \
    LYNX_DEF(LBracket, "[")             \
    LYNX_DEF(RBracket, "]")             \
    

#define LYNX_AST_KINDS()            \
    LYNX_DEF(Nil, "NIL")            \
    LYNX_DEF(Bool, "BOOL")          \
    LYNX_DEF(Num, "NUMBER")         \
    LYNX_DEF(Ident, "IDENTIFIER")   \
    LYNX_DEF(String, "STRING")      \
    LYNX_DEF(Array, "ARRAY")        \
    LYNX_DEF(Group, "GROUP")        \
    LYNX_DEF(Block, "BLOCK")        \
    LYNX_DEF(If, "IF")              \
    LYNX_DEF(While, "WHILE")        \
    LYNX_DEF(For, "FOR")            \
    LYNX_DEF(FunDef, "FUN_DEF")     \
    LYNX_DEF(LambdaDef, "LAMBDA_DEF") \
    LYNX_DEF(FCall, "FCALL")        \
    LYNX_DEF(LCall, "LCALL")        \
    LYNX_DEF(MCall, "MCALL")        \
    LYNX_DEF(Index, "EXPR")         \
    LYNX_DEF(Expr, "EXPR")          \
    LYNX_DEF(Return, "RETURN")      \
    LYNX_DEF(Continue, "CONTINUE")  \
    LYNX_DEF(Break, "BREAK")        \
    LYNX_DEF(Let, "LET")            \
    LYNX_DEF(Unary, "UNARY_OP")     \
    LYNX_DEF(Binaray, "BINARY_OP")  \
    LYNX_DEF(Ternary, "TERNARY_OP") \
    LYNX_DEF(Comment, "COMMENT")


#define LYNX_DECLARE_EVAL()         \
    Self eval(Scope& scope) override

// -*----------------------------------------------------------------*-
// -*- begin::namespace::lynx                                       -*-
// -*----------------------------------------------------------------*-
namespace lynx{
// -
enum class TokenKind{
#define LYNX_DEF(kind, _) kind,
    LYNX_TOKENS()
    LYNX_KEYWORDS()
#undef LYNX_DEF
};

// -*-
struct Token{
    TokenKind kind;
    Str::iterator beg;
    Str::iterator end;
    i32 row;
    i32 col;
    Token(TokenKind kind, Str::iterator start, Str::iterator last, i32 r, i32 c)
    : kind{kind}, beg{start}, end{last}, row{r}, col{c}{}

    Str str(void){ return Str{beg, end};}
};

// -
enum class AstKind{
#define LYNX_DEF(kind, _) kind,
    LYNX_AST_KINDS()
#undef LYNX_DEF
};

class AbstractAst{
public:
    AbstractAst(AstKind kind): m_kind{kind}{}
    virtual ~AbstractAst() = default;
    AstKind kind(void) const{ return this->m_kind; }
    virtual Self eval(Scope& scope) = 0;

protected:
    AstKind m_kind;
};

class BlockStmt;
class FunDefExpr;
class LambdaDefExpr;
using Ast = Shared<AbstractAst>;
using Block = Shared<BlockStmt>;
using FunDef = Shared<FunDefExpr>;
using LambdaDef = Shared<LambdaDefExpr>;

// -*- nil
class NilLiteral final: public AbstractAst{
public:
    NilLiteral(): AbstractAst(AstKind::Nil){}
    ~NilLiteral() = default;
    LYNX_DECLARE_EVAL();
};

// -*- false, true
class BoolLiteral final: public AbstractAst {
public:
    BoolLiteral(const Token& token)
    : AbstractAst{AstKind::Bool}, m_token{token}{}
    ~BoolLiteral() = default;
    LYNX_DECLARE_EVAL();

private:
    Token m_token;
};

// -*- 1, 3.14
class NumberLiteral final: public AbstractAst{
private:
    Token m_token;

public:
    NumberLiteral(const Token& token)
    : AbstractAst(AstKind::Num), m_token{token}{}
    ~NumberLiteral() = default;
    LYNX_DECLARE_EVAL();
};

// -*- sym
class Identifier final: public AbstractAst{
private:
    Token m_token;

public:
    Identifier(const Token& token)
    : AbstractAst(AstKind::Ident), m_token{token}{}
    ~Identifier() = default;
    LYNX_DECLARE_EVAL();
};

// -*- "text"
class StringLiteral final: public AbstractAst{
private:
    Token m_token;

public:
    StringLiteral(const Token& token)
    : AbstractAst(AstKind::String), m_token{token}{}
    ~StringLiteral() = default;
    LYNX_DECLARE_EVAL();
};

// -*- op expr
class UnaryExpr final: public AbstractAst{
private:
    Str m_op;
    Ast m_rhs;

public:
    UnaryExpr(const Str& op, Ast rhs)
    : AbstractAst(AstKind::Unary), m_op{op}, m_rhs{rhs}
    {}
    ~UnaryExpr() = default;
    LYNX_DECLARE_EVAL();
};

// -*- expr op expr
class BinaryExpr final: public AbstractAst{
private:
    Ast m_lhs;
    Str m_op;
    Ast m_rhs;

public:
    BinaryExpr(Ast lhs, const Str& op, Ast rhs)
    : AbstractAst(AstKind::Binaray)
    , m_lhs{lhs}, m_op{op}, m_rhs{rhs}{}
    ~BinaryExpr() = default;
    LYNX_DECLARE_EVAL();
};

// -*- test ? then : alt
class TernaryExpr final: public AbstractAst{
public:
    Ast m_cond;
    Ast m_then;
    Ast m_alt;

public:
    TernaryExpr(Ast cond, Ast then, Ast alt)
    : AbstractAst(AstKind::Ternary)
    , m_cond{cond}, m_then{then}, m_alt{alt}{}
    ~TernaryExpr() = default;
    LYNX_DECLARE_EVAL();
};

// -*- let x, y = 1, 2
class GroupExpr final: public AbstractAst{
private:
    Vec<Ast> m_exprs;

public:
    GroupExpr(Vec<Ast> exprs)
    : AbstractAst(AstKind::Group), m_exprs{exprs}{}
    ~GroupExpr() = default;
    LYNX_DECLARE_EVAL();
};

// -*- if(test){<stmts>}else{<stmts>}
class IfExpr final: public AbstractAst{
private:
    Ast m_test;
    Block m_then;
    Block m_alt;

public:
    IfExpr(Ast test, Block then, Block alt)
    : AbstractAst(AstKind::If)
    , m_test{std::move(test)}
    , m_then{std::move(then)}
    , m_alt{std::move(alt)}{}
    ~IfExpr() = default;
    LYNX_DECLARE_EVAL();
};

// -*- while(test){<stmts>}
class WhileExpr final: public AbstractAst{
private:
    Ast m_test;
    Block m_block;

public:
    WhileExpr(Ast test, Block block)
    : AbstractAst(AstKind::While)
    , m_test{test}
    , m_block{block}
    {}
    ~WhileExpr() = default;
    LYNX_DECLARE_EVAL();
};

// -*- for(x: iterable){<stmts>}
class ForExpr final: public AbstractAst{
private:
    Identifier m_id;
    Ast m_iterable;
    Block m_block;

public:
    ForExpr(Identifier id, Ast iterable, Block block)
    : AbstractAst(AstKind::For)
    , m_id{id}, m_iterable{iterable}, m_block{block}{}
    ~ForExpr() = default;
    LYNX_DECLARE_EVAL();
};

// -*- fun name(<param>){<stmts>}
class FunDefExpr final: public AbstractAst{
private:
    Identifier m_name;
    Vec<Identifier> m_params;
    Block m_block;
    Scope m_scope;

public:
    FunDefExpr(Identifier name, Vec<Identifier> params, Block block, Scope scope)
    : AbstractAst(AstKind::FunDef)
    , m_name{name}
    , m_params{params}
    , m_block{block}
    , m_scope{scope}
    {}
    ~FunDefExpr() = default;
    LYNX_DECLARE_EVAL();
};

// -*- lambda(<params>){<stmts>}
class LambdaDefExpr final: public AbstractAst{
private:
    Identifier m_name;
    Vec<Identifier> m_params;
    Block m_block;
    Scope m_scope;

public:
    LambdaDefExpr(Identifier name, Vec<Identifier> params, Block block, Scope scope)
    : AbstractAst(AstKind::FunDef)
    , m_name{name}
    , m_params{params}
    , m_block{block}
    , m_scope{scope}
    {}
    ~LambdaDefExpr() = default;
    LYNX_DECLARE_EVAL();
};

// -*- fname(argv)
class FunCallExpr final: public AbstractAst {
private:
    FunDef m_fun;
    Vec<Ast> m_argv;

public:
    FunCallExpr(FunDef fun, Vec<Ast> argv)
    : AbstractAst(AstKind::FCall)
    , m_fun{fun}, m_argv{argv}{}
    ~FunCallExpr() = default;
    LYNX_DECLARE_EVAL();
};

// -*- lambdaVal(argv)
class LambdaCallExpr final: public AbstractAst{
private:
    LambdaDef m_lambda;
    Vec<Ast> m_argv;

public:
    LambdaCallExpr(LambdaDef lambda, Vec<Ast> argv)
    : AbstractAst(AstKind::LCall)
    , m_lambda{lambda}, m_argv{argv}{}
    ~LambdaCallExpr() = default;
    LYNX_DECLARE_EVAL();
};

// -*- self.method(argv)
class MethodCallExpr final: public AbstractAst{
private:
    Self m_self;
    Identifier m_method;
    Vec<Ast> m_argv;

public:
    MethodCallExpr(Self self, Identifier method, Vec<Ast> argv)
    : AbstractAst(AstKind::MCall)
    , m_self{self}, m_method{method}, m_argv{argv}{}
    ~MethodCallExpr() = default;
    LYNX_DECLARE_EVAL();
};

// -*- expr[idx]
class IndexExpr final: public AbstractAst{
private:
    Self m_self;
    Ast m_expr;

public:
    IndexExpr(const Self& self, Ast expr)
    : AbstractAst(AstKind::Index)
    , m_self{self}, m_expr{expr}{}
    ~IndexExpr() = default;
    LYNX_DECLARE_EVAL();
};

// -*- let var = expr
class LetStmt final: public AbstractAst{
private:
    Identifier m_var;
    Ast m_expr;

public:
    LetStmt(Identifier var, Ast expr)
    : AbstractAst(AstKind::Let)
    , m_var{var}, m_expr{expr}{}
    ~LetStmt() = default;
    LYNX_DECLARE_EVAL();
};

// -*- <expr>
class ExprStmt final: public AbstractAst {
private:
    Ast m_expr;

public:
    ExprStmt(Ast expr): AbstractAst(AstKind::Expr), m_expr{expr}{}
    ~ExprStmt() = default;
    LYNX_DECLARE_EVAL();
};

// -*- return <expr>
class ReturnStmt final: public AbstractAst{
private:
    Ast m_expr;

public:
    ReturnStmt(Ast expr)
    : AbstractAst(AstKind::Return)
    , m_expr{expr}{}
    ~ReturnStmt() = default;
    LYNX_DECLARE_EVAL();
};

// -*- {<expr>}
class BlockStmt final: public AbstractAst{
private:
    Scope m_scope;
    Vec<Ast> m_stmts;

public:
    BlockStmt(Scope scope, Vec<Ast> stmts)
    : AbstractAst(AstKind::Block)
    , m_scope{scope}, m_stmts{stmts}{}
    ~BlockStmt() = default;
    LYNX_DECLARE_EVAL();
};

// -*- break
class BreakStmt final: public AbstractAst{
public:
    BreakStmt(): AbstractAst(AstKind::Break){}
    ~BreakStmt() = default;
    LYNX_DECLARE_EVAL();
};

// -*- continue
class ContinueStmt final: public AbstractAst{
public:
    ContinueStmt(): AbstractAst(AstKind::Continue){}
    ~ContinueStmt() = default;
    LYNX_DECLARE_EVAL();
};

// -*- (* text *)
class Comment final: public AbstractAst{
private:
    Str m_comment;

public:
    Comment(const Str& comment)
    : AbstractAst(AstKind::Comment), m_comment{comment}{}
    ~Comment() = default;
    LYNX_DECLARE_EVAL();
};


// -*-----------*-
// -*- Scanner -*-
// -*-----------*-
// -
class Scanner final{
private:
    Str::iterator m_begin;
    Str::iterator m_end;
    i32 m_curPos;
    i32 m_nxtPos;
    char m_char;

public:
    Scanner() = default;
    Scanner(Str::const_iterator& begin, Str::const_iterator& end);
    ~Scanner() = default;
    Vec<Token> tokenizer(void);

private:
    char peek(int i=0);
    void advance(void);
    Token next_token(void);
    Token read_ident(void);
    Token read_boolean(void);
    Token read_number(void);
    Token read_string(void);
    void read_comment(void); // (* text *)
    void skip_whitespace(void);
};


// -*----------*-
// -*- Parser -*-
// -*----------*-
enum class Precedence{
    Lowest,
    Equal, 
    LessGreater,
    Sum,
    Product,
    Unary,
    Call,
    Index,
};
// -
class Parser{
private:
    Scanner m_scanner;
    Scope m_scope;
    Precedence m_curPrecedence;
    Precedence m_nxnPrecedence;
    Vec<Token> m_tokens;
    int m_idx;

public:
    Parser(const Str& script, Scope scope);
    Parser(std::istream&& stream, Scope scope);
    ~Parser();
    Self parse(void);

private:
    Precedence current_precedence(void);
    Precedence next_precedence(void);
    bool match(const Token& token, TokenKind kind);
    bool expect(TokenKind kind);
    Self parse_stmt(void);
    Self parse_let_stmt(void);
    Self parse_return_stmt(void);
    Self parse_break_stmt(void);
    Self parse_continue_stmt(void);
    Self parse_expr_stmt(void);
    Self parse_block_stmt(void);
    Self parse_expr(void);
    Self parse_ident_expr(void);
    Self parse_unary_expr(void);
    Self parse_binary_expr(void);
    Self parse_ternary_expr(void);
    Self parse_nil_literal(void);
    Self parse_boolean_literal(void);
    Self parse_number_literal(void);
    Self parse_string_literal(void);
    Self parse_fundef_expr(void);
    Self parse_lambda_expr(void);
    Self parse_array_literal(void);
    Self parse_dict_literal(void);
    Self parse_group_expr(void);
    Self parse_if_expr(void);
    Self parse_while_expr(void);
    Self parse_for_expr(void);
    Self parse_call_expr(Vec<Self> argv);
    Self parse_index_expr(Ast expr);
};


// -*--------------------*-
// -*- Lynx Interpreter -*-
// -*--------------------*-
/*
void Lynx::run(const Str& source);
void Lynx::repl(const std::fstream&& stream);
Str Lynx::version(void);
Str Lynx::license(void);
Str Lynx::authors(void);
*/


// -*----------------------------------------------------------------*-
}//-*- end::namespace::lynx                                         -*-
// -*----------------------------------------------------------------*-