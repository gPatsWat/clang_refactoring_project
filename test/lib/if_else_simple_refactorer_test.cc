#include "if_else_simple_refactorer.h"
#include "gtest/gtest.h"
#include "prep_code.h"

using namespace corct;
using namespace clang;
using namespace clang::ast_matchers;

using IESR = if_else_simple_refactorer;

string_t const new_param = "";
string_t const fname = "input.cc";

template <typename Tester>
bool
run_case_iesr(string_t const& code, Tester &t, replacements_t const & reps_exp)
{
    ASTUPtr ast;
    ASTContext * pctx;
    TranslationUnitDecl * decl;
    std::tie(ast, pctx, decl) = prep_code(code);
    finder_t finder;
    finder.addMatcher(t.mk_branch_matcher(), &t);

    finder.matchAST(*pctx);

    replacements_t const & reps(t.get_replacements(fname));
    bool const size_ok(reps_exp.size() == reps.size());

    EXPECT_EQ(reps_exp.size(), reps.size());
    return size_ok;
}

TEST(if_else_refactor, instantiate)
{
    replacements_map_t reps;
    vec_str ftargs = {};
    bool const dry_run(false);
    IESR(reps, ftargs, new_param, dry_run);
    EXPECT_TRUE(true);
} //instantiate

TEST(if_else_refactor, simple_if_simple_else)
{
    string_t const code =
    "int simple_if_else_simple_return(int a3, int b3) \
    { \
        if (a3 || b3) return a3; \
     else \
     return b3; \
    } \
    ";

    replacements_map_t reps;
    vec_str ftargs = {};
    bool const dry_run(false);
    IESR iesr(reps, ftargs, new_param, dry_run);
    replacements_t exp_repls;

    run_case_iesr(code, iesr, exp_repls);
}