#include "lynx.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::lynx                                       -*-
// -*----------------------------------------------------------------*-
namespace lynx{
// -

/*

struct BaseAst: public AbstractAST{
    public:
        explicit BaseAst(AstKind kind, Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        explicit BaseAst(AstKind kind, Ast& root, Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BaseAst);
        LYNX_DECLARE_MOVE(BaseAst);
        virtual ~BaseAst() = default;
        LYNX_VIRTUAL_OVERLOAD_AST_COMMONS();
        const Vec<Ast>& nodes(void) const{ return this->m_nodes; }
    
    protected:
        Vec<Ast> m_nodes;
    };
    
struct TernaryExprAst: public AbstractAST{
    public:
        explicit TernaryExprAst(AstKind kind, Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(TernaryExprAst);
        LYNX_DECLARE_MOVE(TernaryExprAst);
        virtual ~TernaryExprAst() = default;
        LYNX_VIRTUAL_OVERLOAD_AST_COMMONS();
        const Ast& first_node(void) const{ return this->m_fst; }
        const Ast& second_node(void) const{ return this->m_snd; }
        const Ast& third_node(void) const{ return this->m_trd; }
    
    protected:
        Ast m_fst;  // First Node
        Ast m_snd;  // Second Node
        Ast m_trd;  // Third Node
    };
    
struct BinaryExprAst: public AbstractAST{
    public:
        explicit BinaryExprAst(AstKind kind, Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BinaryExprAst);
        LYNX_DECLARE_MOVE(BinaryExprAst);
        virtual ~BinaryExprAst() = default;
        LYNX_VIRTUAL_OVERLOAD_AST_COMMONS();
        const Ast& lhs(void) const{ return this->m_lhs; }
        const Ast& rhs(void) const{ return this->m_rhs; }
    
    protected:
        Ast m_lhs;
        Ast m_rhs;
    };
    
struct UnaryExprAst: public AbstractAST{
    public:
        explicit UnaryExprAst(AstKind kind, Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(UnaryExprAst);
        LYNX_DECLARE_MOVE(UnaryExprAst);
        virtual ~UnaryExprAst();
        LYNX_VIRTUAL_OVERLOAD_AST_COMMONS();
        const Ast& rhs(void) const{ return this->m_rhs; }
    
    protected:
        Ast m_rhs;
    };
    
struct NullaryExprAst: public AbstractAST{
    public:
        explicit NullaryExprAst(AstKind kind, Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(NullaryExprAst);
        LYNX_DECLARE_MOVE(NullaryExprAst);
        virtual ~NullaryExprAst() = default;
        LYNX_VIRTUAL_OVERLOAD_AST_COMMONS();
        const Ast& value(void) const{ return this->m_value; }
    
    protected:
        Ast m_value;
    };
    
struct IdentifierAst final: public NullaryExprAst{
        explicit IdentifierAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_MOVE(IdentifierAst);
        LYNX_DECLARE_COPY(IdentifierAst);
        ~IdentifierAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct NilLiteralAst final: public NullaryExprAst{
        explicit NilLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(NilLiteralAst);
        LYNX_DECLARE_MOVE(NilLiteralAst);
        ~NilLiteralAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct BoolLiteralAst final: public NullaryExprAst{
        explicit BoolLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BoolLiteralAst);
        LYNX_DECLARE_MOVE(BoolLiteralAst);
        ~BoolLiteralAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct IntegerLiteralAst final: public NullaryExprAst{
        explicit IntegerLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(IntegerLiteralAst);
        LYNX_DECLARE_MOVE(IntegerLiteralAst);
        ~IntegerLiteralAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct FloatLiteralAst final: public NullaryExprAst{
        explicit FloatLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(FloatLiteralAst);
        LYNX_DECLARE_MOVE(FloatLiteralAst);
        ~FloatLiteralAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };

struct ComplexExprAst final: public UnaryExprAst{
    explicit ComplexExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
    LYNX_DECLARE_COPY(ComplexExprAst);
    LYNX_DECLARE_MOVE(ComplexExprAst);
    ~ComplexExprAst() = default;
    LYNX_OVERLOAD_AST_COMMONS();
};
    
    
struct SymbolLiteralAst final: public NullaryExprAst{
        explicit SymbolLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(SymbolLiteralAst);
        LYNX_DECLARE_MOVE(SymbolLiteralAst);
        ~SymbolLiteralAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct StringLiteralAst final: public NullaryExprAst{
        explicit StringLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(StringLiteralAst);
        LYNX_DECLARE_MOVE(StringLiteralAst);
        ~StringLiteralAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct TupleLiteralAst final: public NullaryExprAst{
        explicit TupleLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(TupleLiteralAst);
        LYNX_DECLARE_MOVE(TupleLiteralAst);
        ~TupleLiteralAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct ListLiteralAst final: public NullaryExprAst{
        explicit ListLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(ListLiteralAst);
        LYNX_DECLARE_MOVE(ListLiteralAst);
        ~ListLiteralAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct SetLiteralAst final: public NullaryExprAst{
        explicit SetLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(SetLiteralAst);
        LYNX_DECLARE_MOVE(SetLiteralAst);
        ~SetLiteralAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct DictLiteralAst final: public NullaryExprAst{
        explicit DictLiteralAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(DictLiteralAst);
        LYNX_DECLARE_MOVE(DictLiteralAst);
        ~DictLiteralAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct ParameterListExprAst final: public UnaryExprAst{
        explicit ParameterListExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(ParameterListExprAst);
        LYNX_DECLARE_MOVE(ParameterListExprAst);
        ~ParameterListExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    
        const HashSet<Parameter> params(void) const;
        const Vec<Parameter> params_vec(void) const;
    
    private:
        HashSet<Parameter> m_params;
        Vec<Parameter> m_paramsvec;
    };
    
struct StructDefExprAst final: public BinaryExprAst{
        explicit StructDefExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(StructDefExprAst);
        LYNX_DECLARE_MOVE(StructDefExprAst);
        ~StructDefExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct LambdaDefAst final: public BinaryExprAst{
        explicit LambdaDefAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(LambdaDefAst);
        LYNX_DECLARE_MOVE(LambdaDefAst);
        ~LambdaDefAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct FunDefExprAst final: public BaseAst{
        explicit FunDefExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(FunDefExprAst);
        LYNX_DECLARE_MOVE(FunDefExprAst);
        ~FunDefExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct MacroDefExprAst final: public BaseAst {
        explicit MacroDefExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(MacroDefExprAst);
        LYNX_DECLARE_MOVE(MacroDefExprAst);
        ~MacroDefExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
        // -
        Ast expand(void);
        HashSet<Parameter> parameters(void) const;
    
    private:
        HashSet<Parameter> m_params;
    };
    
struct GetExprAst final: public BinaryExprAst {
        explicit GetExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(GetExprAst);
        LYNX_DECLARE_MOVE(GetExprAst);
        ~GetExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct SetExprAst final: public TernaryExprAst {
        explicit SetExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(SetExprAst);
        LYNX_DECLARE_MOVE(SetExprAst);
        ~SetExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct FCallExprAst final: public BinaryExprAst {
        explicit FCallExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(FCallExprAst);
        LYNX_DECLARE_MOVE(FCallExprAst);
        ~FCallExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct MCallExprAst final: public TernaryExprAst {
        explicit MCallExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(MCallExprAst);
        LYNX_DECLARE_MOVE(MCallExprAst);
        ~MCallExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct MinusExprAst final: public UnaryExprAst {
        explicit MinusExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(MinusExprAst);
        LYNX_DECLARE_MOVE(MinusExprAst);
        ~MinusExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct PlusExprAst final: public UnaryExprAst {
        explicit PlusExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(PlusExprAst);
        LYNX_DECLARE_MOVE(PlusExprAst);
        ~PlusExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct AddExprAst final: public BinaryExprAst {
        explicit AddExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(AddExprAst);
        LYNX_DECLARE_MOVE(AddExprAst);
        ~AddExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct SubExprAst final: public BinaryExprAst {
        explicit SubExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(SubExprAst);
        LYNX_DECLARE_MOVE(SubExprAst);
        ~SubExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct DivExprAst final: public BinaryExprAst {
        explicit DivExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(DivExprAst);
        LYNX_DECLARE_MOVE(DivExprAst);
        ~DivExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct ModExprAst final: public BinaryExprAst {
        explicit ModExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(ModExprAst);
        LYNX_DECLARE_MOVE(ModExprAst);
        ~ModExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct PowExprAst final: public BinaryExprAst {
        explicit PowExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(PowExprAst);
        LYNX_DECLARE_MOVE(PowExprAst);
        ~PowExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct AddAssignExprAst final: public BinaryExprAst {
        explicit AddAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(AddAssignExprAst);
        LYNX_DECLARE_MOVE(AddAssignExprAst);
        ~AddAssignExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct SubAssignExprAst final: public BinaryExprAst {
        explicit SubAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(SubAssignExprAst);
        LYNX_DECLARE_MOVE(SubAssignExprAst);
        ~SubAssignExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct DivAssignExprAst final: public BinaryExprAst {
        explicit DivAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(DivAssignExprAst);
        LYNX_DECLARE_MOVE(DivAssignExprAst);
        ~DivAssignExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct ModAssignExprAst final: public BinaryExprAst {
        explicit ModAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(ModAssignExprAst);
        LYNX_DECLARE_MOVE(ModAssignExprAst);
        ~ModAssignExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct PowAssignExprAst final: public BinaryExprAst {
        explicit PowAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(PowAssignExprAst);
        LYNX_DECLARE_MOVE(PowAssignExprAst);
        ~PowAssignExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
    
struct LogicalOrExprAst final: public BinaryExprAst {
        explicit LogicalOrExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(LogicalOrExprAst);
        LYNX_DECLARE_MOVE(LogicalOrExprAst);
        ~LogicalOrExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct LogicalAndExprAst final: public BinaryExprAst {
        explicit LogicalAndExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(LogicalAndExprAst);
        LYNX_DECLARE_MOVE(LogicalAndExprAst);
        ~LogicalAndExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct LogicalNotExprAst final: public UnaryExprAst {
        explicit LogicalNotExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(LogicalNotExprAst);
        LYNX_DECLARE_MOVE(LogicalNotExprAst);
        ~LogicalNotExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct BitNotExprAst final: public UnaryExprAst {
        explicit BitNotExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BitNotExprAst);
        LYNX_DECLARE_MOVE(BitNotExprAst);
        ~BitNotExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct BitOrExprAst final: public BinaryExprAst {
        explicit BitOrExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BitOrExprAst);
        LYNX_DECLARE_MOVE(BitOrExprAst);
        ~BitOrExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct BitAndExprAst final: public BinaryExprAst {
        explicit BitAndExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BitAndExprAst);
        LYNX_DECLARE_MOVE(BitAndExprAst);
        ~BitAndExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct BitXorExprAst final: public BinaryExprAst {
        explicit BitXorExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BitXorExprAst);
        LYNX_DECLARE_MOVE(BitXorExprAst);
        ~BitXorExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct BitShlExprAst final: public BinaryExprAst {
        explicit BitShlExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BitShlExprAst);
        LYNX_DECLARE_MOVE(BitShlExprAst);
        ~BitShlExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct BitShrExprAst final: public BinaryExprAst {
        explicit BitShrExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BitShrExprAst);
        LYNX_DECLARE_MOVE(BitShrExprAst);
        ~BitShrExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct BitOrAssignExprAst final: public BinaryExprAst {
        explicit BitOrAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BitOrAssignExprAst);
        LYNX_DECLARE_MOVE(BitOrAssignExprAst);
        ~BitOrAssignExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct BitAndAssignExprAst final: public BinaryExprAst {
        explicit BitAndAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BitAndAssignExprAst);
        LYNX_DECLARE_MOVE(BitAndAssignExprAst);
        ~BitAndAssignExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct BitXorAssignExprAst final: public BinaryExprAst {
        explicit BitXorAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BitXorAssignExprAst);
        LYNX_DECLARE_MOVE(BitXorAssignExprAst);
        ~BitXorAssignExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct BitShlAssignExprAst final: public BinaryExprAst {
        explicit BitShlAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BitShlAssignExprAst);
        LYNX_DECLARE_MOVE(BitShlAssignExprAst);
        ~BitShlAssignExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct BitShrAssignExprAst final: public BinaryExprAst {
        explicit BitShrAssignExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BitShrAssignExprAst);
        LYNX_DECLARE_MOVE(BitShrAssignExprAst);
        ~BitShrAssignExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct CreateStructObjExprAst final: public BinaryExprAst {
        explicit CreateStructObjExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(CreateStructObjExprAst);
        LYNX_DECLARE_MOVE(CreateStructObjExprAst);
        ~CreateStructObjExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct AtInitExprAst final: public BinaryExprAst {
        explicit AtInitExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(AtInitExprAst);
        LYNX_DECLARE_MOVE(AtInitExprAst);
        ~AtInitExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct AtMainExprAst final: public BinaryExprAst {
        explicit AtMainExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(AtMainExprAst);
        LYNX_DECLARE_MOVE(AtMainExprAst);
        ~AtMainExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct AtTestingExprAst final: public BinaryExprAst {
        explicit AtTestingExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(AtTestingExprAst);
        LYNX_DECLARE_MOVE(AtTestingExprAst);
        ~AtTestingExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct AtTestExprAst final: public BinaryExprAst {
        explicit AtTestExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(AtTestExprAst);
        LYNX_DECLARE_MOVE(AtTestExprAst);
        ~AtTestExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct AtSetupExprAst final: public UnaryExprAst {
        explicit AtSetupExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(AtSetupExprAst);
        LYNX_DECLARE_MOVE(AtSetupExprAst);
        ~AtSetupExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct AtTeardownExprAst final: public UnaryExprAst {
        explicit AtTeardownExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(AtTeardownExprAst);
        LYNX_DECLARE_MOVE(AtTeardownExprAst);
        ~AtTeardownExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct AtDefineExprAst final: public TernaryExprAst {
        explicit AtDefineExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(AtDefineExprAst);
        LYNX_DECLARE_MOVE(AtDefineExprAst);
        ~AtDefineExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct AtOperatorExprAst final: public TernaryExprAst {
        explicit AtOperatorExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(AtOperatorExprAst);
        LYNX_DECLARE_MOVE(AtOperatorExprAst);
        ~AtOperatorExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct AtDocExprAst final: public UnaryExprAst {
        explicit AtDocExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(AtDocExprAst);
        LYNX_DECLARE_MOVE(AtDocExprAst);
        ~AtDocExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct AtWhenExprAst final: public TernaryExprAst {
        explicit AtWhenExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(AtWhenExprAst);
        LYNX_DECLARE_MOVE(AtWhenExprAst);
        ~AtWhenExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct BlockStmtAst final: public UnaryExprAst {
        explicit BlockStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BlockStmtAst);
        LYNX_DECLARE_MOVE(BlockStmtAst);
        ~BlockStmtAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct LetStmtAst final: public BinaryExprAst {
        explicit LetStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(LetStmtAst);
        LYNX_DECLARE_MOVE(LetStmtAst);
        ~LetStmtAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct VarStmtAst final: public BinaryExprAst {
        explicit VarStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(VarStmtAst);
        LYNX_DECLARE_MOVE(VarStmtAst);
        ~VarStmtAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
    
struct IfStmtAst final: public BaseAst {
        explicit IfStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(IfStmtAst);
        LYNX_DECLARE_MOVE(IfStmtAst);
        ~IfStmtAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct ForStmtAst final: public BinaryExprAst {
        explicit ForStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(ForStmtAst);
        LYNX_DECLARE_MOVE(ForStmtAst);
        ~ForStmtAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct WhileStmtAst final: public BinaryExprAst {
        explicit WhileStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(WhileStmtAst);
        LYNX_DECLARE_MOVE(WhileStmtAst);
        ~WhileStmtAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
    
struct BreakStmtAst final: public NullaryExprAst {
        explicit BreakStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(BreakStmtAst);
        LYNX_DECLARE_MOVE(BreakStmtAst);
        ~BreakStmtAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct ContinueStmtAst final: public NullaryExprAst {
        explicit ContinueStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(ContinueStmtAst);
        LYNX_DECLARE_MOVE(ContinueStmtAst);
        ~ContinueStmtAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct ReturnStmtAst final: public UnaryExprAst {
        explicit ReturnStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(ReturnStmtAst);
        LYNX_DECLARE_MOVE(ReturnStmtAst);
        ~ReturnStmtAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct ImportStmtAst final: public BaseAst {
        explicit ImportStmtAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(ImportStmtAst);
        LYNX_DECLARE_MOVE(ImportStmtAst);
        ~ImportStmtAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct DestructuringExprAst final: public BinaryExprAst {
        explicit DestructuringExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(DestructuringExprAst);
        LYNX_DECLARE_MOVE(DestructuringExprAst);
        ~DestructuringExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct ResultExprAst final: public UnaryExprAst {
        explicit ResultExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(ResultExprAst);
        LYNX_DECLARE_MOVE(ResultExprAst);
        ~ResultExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct LessExprAst final: public BinaryExprAst {
        explicit LessExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(LessExprAst);
        LYNX_DECLARE_MOVE(LessExprAst);
        ~LessExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct LessEqExprAst final: public BinaryExprAst {
        explicit LessEqExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(LessEqExprAst);
        LYNX_DECLARE_MOVE(LessEqExprAst);
        ~LessEqExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct GreaterExprAst final: public BinaryExprAst {
        explicit GreaterExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(GreaterExprAst);
        LYNX_DECLARE_MOVE(GreaterExprAst);
        ~GreaterExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct GreaterEqExprAst final: public BinaryExprAst {
        explicit GreaterEqExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(GreaterEqExprAst);
        LYNX_DECLARE_MOVE(GreaterEqExprAst);
        ~GreaterEqExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct EqualExprAst final: public BinaryExprAst {
        explicit EqualExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(EqualExprAst);
        LYNX_DECLARE_MOVE(EqualExprAst);
        ~EqualExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    
struct NotEqualExprAst final: public BinaryExprAst {
        explicit NotEqualExprAst(Vec<Token>::iterator begin, Vec<Token>::iterator end) noexcept;
        LYNX_DECLARE_COPY(NotEqualExprAst);
        LYNX_DECLARE_MOVE(NotEqualExprAst);
        ~NotEqualExprAst() = default;
        LYNX_OVERLOAD_AST_COMMONS();
    };
    

*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::lynx                                         -*-
// -*----------------------------------------------------------------*-