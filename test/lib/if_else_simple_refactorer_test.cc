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
bool run_case_iesr(string_t const &code, Tester &t, replacements_t const &reps_exp)
{
    ASTUPtr ast;
    ASTContext *pctx;
    TranslationUnitDecl *decl;
    std::tie(ast, pctx, decl) = prep_code(code);
    finder_t finder;
    corct::if_else_simple_refactorer::matchers_t branch_matchers =
        t.branch_matchers();

    for (auto &m : branch_matchers)
    {
        finder.addMatcher(m, &t);
    }

    finder.matchAST(*pctx);

    replacements_t const &reps(t.get_replacements(fname));
    bool const size_ok(reps_exp.size() == reps.size());

    EXPECT_EQ(reps_exp.size(), reps.size());

    bool const reps_match = reps_exp == reps;

    replacements_t::const_iterator ri = reps.begin(), ei = reps_exp.begin();

    for (; ri != reps.end() && ei != reps_exp.end(); ++ri, ++ei)
    {
        EXPECT_EQ(ei->getReplacementText(), ri->getReplacementText());
    }

    if (!reps_match)
    {
        std::cout << "reps disagree:\n";
        replacements_t::const_iterator ri = reps.begin(), ei = reps_exp.begin();
        for (; ri != reps.end() && ei != reps_exp.end(); ++ri, ++ei)
        {
            std::cout << "expected: " << ei->toString()
                      << ", actual: " << ri->toString() << "\n";
        }
    }

    EXPECT_EQ(reps_exp, reps);

    return size_ok && reps_match;
}

TEST(if_else_refactor, instantiate)
{
    replacements_map_t reps;
    vec_str ftargs = {};
    bool const dry_run(false);
    IESR give_me_a_name(reps, dry_run);
    EXPECT_TRUE(true);
} // instantiate

TEST(if_else_refactor, simple_if_simple_else)
{
    string_t const code =
        "int simple_if_else_simple_return (int a1, int b1) {if(a1 || b1)return a1;else return b1;}";

    string_t const refactored_expr =
        "(a1 || b1)*(a1) + !(a1 || b1)*(b1);";

    replacements_map_t reps;
    vec_str ftargs = {};
    bool const dry_run(false);
    IESR iesr(reps, dry_run);
    replacements_t exp_repls;

    if (exp_repls.add({fname, 51u, 37u, refactored_expr}))
    {
        HERE("add replacement failed")
    }

    run_case_iesr(code, iesr, exp_repls);
}

TEST(if_else_refactor, if_simple_else)
{
    string_t const code =
        "int if_simple_else (int a2, int b2) {if(a2 || b2){return a2;}else return b2;}";

    string_t const refactored_expr =
        "(a2 || b2)*(a2) + !(a2 || b2)*(b2);";

    replacements_map_t reps;
    vec_str ftargs = {};
    bool const dry_run(false);
    IESR iesr(reps, dry_run);
    replacements_t exp_repls;

    if (exp_repls.add({fname, 37u, 39u, refactored_expr}))
    {
        HERE("add replacement failed")
    }

    run_case_iesr(code, iesr, exp_repls);
}

TEST(if_else_refactor, simple_if_else)
{
    string_t const code =
        "int simple_if_else (int a3, int b3) {if(a3 || b3)return a3;else{return b3;}}";

    string_t const refactored_expr =
        "(a3 || b3)*(a3) + !(a3 || b3)*(b3);";

    replacements_map_t reps;
    vec_str ftargs = {};
    bool const dry_run(false);
    IESR iesr(reps, dry_run);
    replacements_t exp_repls;

    if (exp_repls.add({fname, 37u, 38u, refactored_expr}))
    {
        HERE("add replacement failed")
    }

    run_case_iesr(code, iesr, exp_repls);
}

TEST(if_else_refactor, if_else_only_return)
{
    string_t const code =
        "int if_else_only_return (int a4, int b4) {if(a4 || b4){return a4;}else{return b4;}}";

    string_t const refactored_expr =
        "(a4 || b4)*(a4) + !(a4 || b4)*(b4);";

    replacements_map_t reps;
    vec_str ftargs = {};
    bool const dry_run(false);
    IESR iesr(reps, dry_run);
    replacements_t exp_repls;

    if (exp_repls.add({fname, 42u, 40u, refactored_expr}))
    {
        HERE("add replacement failed")
    }

    run_case_iesr(code, iesr, exp_repls);
}