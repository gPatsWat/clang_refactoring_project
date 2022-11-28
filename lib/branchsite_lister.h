// branchsite_lister.h

#pragma once

#include "branchsite_common.h"
#include "dump_things.h"
#include "types.h"
#include "utilities.h"

namespace corct
{
    void print_branch_details(clang::Stmt const * stmt,
                              clang::IfStmt const * ifstmt,
                              clang::SourceManager &sm,
                              std::ostream &o);

    void print_branch_details(clang::Expr const * stmt,
                              clang::IfStmt const * ifstmt,
                              clang::SourceManager &sm,
                              std::ostream &o);

    struct branchsite_lister : public callback_t
    {
        using matcher_t = clang::ast_matchers::StatementMatcher;
        using matchers_t = std::vector<matcher_t>;

        matchers_t matchers()
        {
            using namespace clang::ast_matchers;
            matchers_t ms;
            ms.push_back(mk_if_matcher());
            return ms;
        }

        void run(result_t const &result) override
        {
            using namespace clang;
            auto stmt =
                result.Nodes.getNodeAs<Stmt>("condStmt");

            auto ifstmt =
                result.Nodes.getNodeAs<IfStmt>("ifStmt");

            SourceManager & sm(result.Context->getSourceManager());

            if (stmt && ifstmt)
            {
                m_num_if_statements++;
                print_branch_details(stmt, ifstmt, sm, std::cout);
            }
            else
            {
                check_ptr(stmt, "condStmt");
                check_ptr(ifstmt, "ifStmt");
            }
            return;
            // // run
        }

        // explicit branchsite_lister(vec_str const &targets) : m_targets(targets) {}

        uint32_t m_num_if_statements = 0;

    private:
        vec_str m_targets;
    }; // branchsite lister
} // namespace corct