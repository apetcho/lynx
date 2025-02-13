#include "lynx.hpp"

// -*----------------------------------------------------------------*-
// -*- begin::namespace::lynx                                       -*-
// -*----------------------------------------------------------------*-
namespace lynx{
// -
/*
class Lynx final{
public:
    class Lynx::Frame:: final{
        Lynx::Frame::Frame() noexcept;
        Lynx::Frame::Frame(Lynx::Frame* parent);
        Lynx::Frame::Frame(Lynx::Frame&& frame);
        Lynx::Frame& Lynx::Frame::operator=(Lynx::Frame&& frame);
        Lynx::Frame::~Frame();
        Lynx::Frame& Lynx::Frame::operator=(const Frame& frame);
        Self& Lynx::Frame::operator[](const Str& key);
        const Self& Lynx::Frame::operator[](const Str& key) const;
        Self Lynx::Frame::update(const Str& key, Self value);
        bool Lynx::Frame::contains(const Str& key) const;
    };

    // -*-
    class Module final{
        Lynx::Module::Module() noexcept;
        Lynx::Module::Module(const Str& name, const fs::path& path) noexcept;
        Lynx::Module::Module(Lynx::Module&& lynxModule);
        Lynx::Module& Lynx::Module::operator=(Lynx::Module&& lynxModule);
        Lynx::Module::~Module();
        Lynx::Frame& Lynx::Module::frame(void);
        const Lynx::Frame& Lynx::Module::frame(void) const;
    };

    // -*-
    class TestCase {
        Lynx::TestCase::TestCase(const Str& name, Self testAst, bool skip=false);
        Lynx::TestCase::TestCase(Lynx::TestCase&& testcase);
        Lynx::TestCase& Lynx::TestCase::operator=(Lynx::TestCase&& testcase);
        Lynx::TestCase::~TestCase();
        Lynx::TestCase& Lynx::TestCase::operator=(const TestCase& testcase);
        Self Lynx::TestCase::operator()();
        Str Lynx::TestCase::name(void) const;
    };

    // -*-
    class UnitTest {
        Lynx::UnitTest::UnitTest() noexcept;
        Lynx::UnitTest::UnitTest(const Str& moduleName) noexcept;
        Lynx::UnitTest::UnitTest(const Str& unitTestName, const Str& moduleName) noexcept;
        LYNX_DECLARE_MOVE(UnitTest);
        Lynx::UnitTest::UnitTest(Lynx::UnitTest&& unittest) noexcept;
        Lynx::UnitTest& Lynx::UnitTest::operator=(Lynx::UnitTest&& unittest) noexcept;
        Lynx::UnitTest::~UnitTest();

        // -*-
        void Lynx::UnitTest::run(void);
        void Lynx::UnitTest::add_testcase(const TestCase& testcase);
        Str Lynx::UnitTest::name(void) const;
        Str Lynx::UnitTest::module(void) const;
        // -*-
        Self Lynx::UnitTest::setup(Args args);
        Self Lynx::UnitTest::teardown(Args args);
        Self Lynx::UnitTest::assertTrue(Args args);
        Self Lynx::UnitTest::assertFalse(Args args);
        Self Lynx::UnitTest::assertEqual(Args args);
        Self Lynx::UnitTest::assertNotEqual(Args args);
        Self Lynx::UnitTest::assertLess(Args args);
        Self Lynx::UnitTest::assertLessEqual(Args args);
        Self Lynx::UnitTest::assertGreater(Args args);
        Self Lynx::UnitTest::assertGreaterEqual(Args args);
        Self Lynx::UnitTest::assertTypeof(Args args);
        Self Lynx::UnitTest::assertError(Args args);
        Self Lynx::UnitTest::assertErrorType(Args args);
    };
    

    struct Lynx::Version {
        Lynx::Version::Version();
        Lynx::Version::Version(u32 maj, u32 min, u32 patch);
        Lynx::Version& Lynx::Version::operator=(const Version& version);
        Lynx::Version::~Version() = default;
        Lynx::Version::operator Str();
    };
    
    Lynx::CallBackDict Lynx::boolMethods;
    Lynx::CallBackDict Lynx::integerMethods;
    Lynx::CallBackDict Lynx::floatMethods;
    Lynx::CallBackDict Lynx::complexMethods;
    Lynx::CallBackDict Lynx::symbolMethods;
    Lynx::CallBackDict Lynx::stringMethods;
    Lynx::CallBackDict Lynx::tupleMethods;
    Lynx::CallBackDict Lynx::listMethods;
    Lynx::CallBackDict Lynx::hashsetMethods;
    Lynx::CallBackDict Lynx::hashmapMethods;
    Lynx::CallBackDict Lynx::resultMethods;
    Lynx::CallBackDict Lynx::structMethods;

    HashMap<Str, CFun> Lynx::builtinFunctions;
    HashMap<Str, Self> Lynx::builtinConstants;
    HashMap<Str, Symbol> Lynx::builtinSymbols;

    Lynx::Lynx() noexcept;
    Lynx::Lynx(Lynx&& interp) noexcept;
    Lynx& Lynx::operator=(Lynx& interp) noexcept;
    Lynx::~Lynx();
    Str Lynx::banner(void) const;
    void Lynx::run(const Str& filename, Vec<Str> argv);
    void Lynx::repl(Vec<Str> argv);

    Self Lynx::operator()(IdentifierAst);
    Self Lynx::operator()(NilLiteralAst);
    Self Lynx::operator()(BoolLiteralAst);
    Self Lynx::operator()(IntegerLiteralAst);
    Self Lynx::operator()(FloatLiteralAst);
    Self Lynx::operator()(SymbolLiteralAst);
    Self Lynx::operator()(StringLiteralAst);
    Self Lynx::operator()(TupleLiteralAst);
    Self Lynx::operator()(ListLiteralAst);
    Self Lynx::operator()(SetLiteralAst);
    Self Lynx::operator()(DictLiteralAst);
    Self Lynx::operator()(ParameterListExprAst);
    Self Lynx::operator()(StructDefExprAst);
    Self Lynx::operator()(LambdaDefExprAst);
    Self Lynx::operator()(FunDefExprAst);
    Self Lynx::operator()(MacroDefExprAst);
    Self Lynx::operator()(GetExprAst);
    Self Lynx::operator()(SetExprAst);
    Self Lynx::operator()(FCallExprAst);
    Self Lynx::operator()(MCallExprAst);
    Self Lynx::operator()(MinusExprAst);
    Self Lynx::operator()(PlusExprAst);
    Self Lynx::operator()(AddExprAst);
    Self Lynx::operator()(SubExprAst);
    Self Lynx::operator()(DivExprAst);
    Self Lynx::operator()(ModExprAst);
    Self Lynx::operator()(PowExprAst);
    Self Lynx::operator()(AddAssignExprAst);
    Self Lynx::operator()(SubAssignExprAst);
    Self Lynx::operator()(DivAssignExprAst);
    Self Lynx::operator()(ModAssignExprAst);
    Self Lynx::operator()(PowAssignExprAst);
    Self Lynx::operator()(LogicalOrExprAst);
    Self Lynx::operator()(LogicalAndExprAst);
    Self Lynx::operator()(LogicalNotExprAst);
    Self Lynx::operator()(BitNotExprAst);
    Self Lynx::operator()(BitOrExprAst);
    Self Lynx::operator()(BitAndExprAst);
    Self Lynx::operator()(BitXorExprAst);
    Self Lynx::operator()(BitShlExprAst);
    Self Lynx::operator()(BitShrExprAst);
    Self Lynx::operator()(BitOrAssignExprAst);
    Self Lynx::operator()(BitAndAssignExprAst);
    Self Lynx::operator()(BitXorAssignExprAst);
    Self Lynx::operator()(BitShlAssignExprAst);
    Self Lynx::operator()(BitShrAssignExprAst);
    Self Lynx::operator()(CreateStructObjExprAst);
    Self Lynx::operator()(AtInitExprAst);
    Self Lynx::operator()(AtMainExprAst);
    Self Lynx::operator()(AtTestingExprAst);
    Self Lynx::operator()(AtTestExprAst);
    Self Lynx::operator()(AtSetupExprAst);
    Self Lynx::operator()(AtTeardownExprAst);
    Self Lynx::operator()(AtDefineExprAst);
    Self Lynx::operator()(AtOperatorExprAst);
    Self Lynx::operator()(AtDocExprAst);
    Self Lynx::operator()(WhenExprAst);
    Self Lynx::operator()(BlockStmtAst);
    Self Lynx::operator()(LetStmtAst);
    Self Lynx::operator()(VarStmtAst);
    Self Lynx::operator()(IfStmtAst);
    Self Lynx::operator()(ForStmtAst);
    Self Lynx::operator()(WhileStmtAst);
    Self Lynx::operator()(BreakStmtAst);
    Self Lynx::operator()(ContinueStmtAst);
    Self Lynx::operator()(ReturnStmtAst);
    Self Lynx::operator()(ImportStmtAst);
    Self Lynx::operator()(DestructuringExprAst);
    Self Lynx::operator()(ResultExprAst);
    Self Lynx::operator()(LessExprAst);
    Self Lynx::operator()(LessEqExprAst);
    Self Lynx::operator()(GreaterExprAst);
    Self Lynx::operator()(GreaterEqExprAst);
    Self Lynx::operator()(EqualExprAst);
    Self Lynx::operator()(NotEqualExprAst);
    Self Lynx::operator()(MatchExprAst);
    Self Lynx::operator()(WithExprAst);
    Self Lynx::operator()(AtArgvExprAst);
    Self Lynx::operator()(AtVersionExprAst);

    void Lynx::initialize(void);
};
*/

// -*----------------------------------------------------------------*-
}//-*- end::namespace::lynx                                         -*-
// -*----------------------------------------------------------------*-