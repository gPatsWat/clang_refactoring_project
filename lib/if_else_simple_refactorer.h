// function_signature_expander.h
// Oct 06, 2016
// (c) Copyright 2016 LANSLLC, all rights reserved

#ifndef IF_ELSE_SIMPLE_REFACTORER_H
#define IF_ELSE_SIMPLE_REFACTORER_H

#include "branchsite_common.h"
#include "branchsite_lister.h"
#include "branchless_repl_gen.h"
#include "expression_repl_insert.h"
#include "types.h"

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Core/Replacement.h"
#include "clang/Tooling/Tooling.h"

#include <iostream>

namespace corct
{

  struct expand_signature_traits
  {
    using matcher_t = clang::ast_matchers::StatementMatcher;
  };

  /** replaces the if branch by branchless expression.
   *  The local variable set to the condition (if enabled)
   *  is inserted before the
   *  if-else clause.
   */
  class if_else_simple_refactorer
      : public branchless_replacement_generator<expand_signature_traits>
  {
  public:
    using Base = branchless_replacement_generator<expand_signature_traits>;

    bool check_simple_return(clang::CompoundStmt *cstmt)
    {
      auto first = cstmt->body_begin();
      auto end = cstmt->body_end();
      if (first == end - 1)
      {
        if (auto retstmt = clang::dyn_cast_or_null<clang::ReturnStmt>(*first))
        {
          return true;
        }
      }
      return false;
    }

    bool check_only_return(clang::Stmt *stmt)
    {

      if (auto retstmt = clang::dyn_cast_or_null<clang::ReturnStmt>(stmt))
      {
        return true;
      }

      return false;
    }

    static const string_t if_bind_name;

    void run(result_t const &result) override
    {
      replacement_t rep;

      bool simple_if = false;
      bool simple_else = false;
      bool two_branches = true;

      clang::SourceManager &src_manager(
          const_cast<clang::SourceManager &>(result.Context->getSourceManager()));

      clang::IfStmt *ifstmt;
      // try binding ifstmt node against all matcher bind names

      if ((ifstmt = const_cast<clang::IfStmt *>(
               result.Nodes.getNodeAs<clang::IfStmt>("if_else_bind_name"))))
      {
        // do nothing
        std::cout << "\033[1;31m compound if-else\033[0m\n" << std::endl;
      }
      else if ((ifstmt = const_cast<clang::IfStmt *>(
                    result.Nodes.getNodeAs<clang::IfStmt>("if_simple_else_bind_name"))))
      {
        std::cout << "\033[1;31m if-simple-else\033[0m\n" << std::endl;
        simple_else = true;
      }
      else if ((ifstmt = const_cast<clang::IfStmt *>(
                    result.Nodes.getNodeAs<clang::IfStmt>("simple_if_else_bind_name"))))
      {
        std::cout << "\033[1;31m simple-if-else\033[0m\n" << std::endl;

        simple_if = true;
      }
      else if ((ifstmt = const_cast<clang::IfStmt *>(
                    result.Nodes.getNodeAs<clang::IfStmt>("simple_if_simple_else_bind_name"))))
      {
        std::cout << "\033[1;31m simple-if-simple-else\033[0m\n" << std::endl;
        simple_if = simple_else = true;
      }

      if (ifstmt)
      {

        auto if_then_stmt = clang::dyn_cast_or_null<clang::CompoundStmt>(ifstmt->getThen());
        clang::ReturnStmt *if_return_stmt = nullptr;
        clang::ReturnStmt *else_return_stmt = nullptr;

        // if_then is a compound stmt
        if (if_then_stmt)
        {
#ifdef PRINT_SOURCELOC
          auto stmt =
              result.Nodes.getNodeAs<clang::Stmt>("condStmt");
          print_branch_details(stmt, ifstmt, src_manager, std::cout);
#endif
          // check whether if_then has only returnstmt inside it
          if (check_simple_return(if_then_stmt))
          {
            std::cout << "compound if_then has only return, safe to convert" << std::endl;
            if_return_stmt = clang::dyn_cast_or_null<clang::ReturnStmt>(*(if_then_stmt->body_begin()));
          }
          else
          {
            std::cout << "compound if_then is not safe to convert" << std::endl;
            return;
          }
        }
        else
        {
          if (check_only_return(ifstmt->getThen()))
          {
            std::cout << "simple if_then is simple return" << std::endl;
            if_return_stmt = clang::dyn_cast_or_null<clang::ReturnStmt>(ifstmt->getThen());
          }
          else
          {
            std::cout << "simple if_then is not a return stmt, not safe to convert" << std::endl;
            return;
          }
        }

        auto else_then_stmt = clang::dyn_cast_or_null<clang::CompoundStmt>(ifstmt->getElse());
        // else_then is a compound stmt
        if (else_then_stmt)
        {
#ifdef PRINT_SOURCELOC
          auto stmt =
              result.Nodes.getNodeAs<clang::Stmt>("condStmt");
          print_branch_details(stmt, ifstmt, src_manager, std::cout);
#endif
          // check whether compound else_then has only returnstmt inside it
          if (check_simple_return(else_then_stmt))
          {
            std::cout << "compound else_then has only return, safe to convert" << std::endl;
            else_return_stmt = clang::dyn_cast_or_null<clang::ReturnStmt>(*(else_then_stmt->body_begin()));
          }
          else
          {
            std::cout << "compound else_then is not safe to convert" << std::endl;
            return;
          }
        }
        else
        {
          auto else_branch = clang::dyn_cast_or_null<clang::IfStmt>(ifstmt->getElse());
          if (else_branch)
          {
            std::cout << "multiple condition refactoring currently not supported" << std::endl;
            return;
          }
          else
          {
            if (check_only_return(ifstmt->getElse()))
            {
              std::cout << "simple else_then has only return, safe to convert" << std::endl;
              else_return_stmt = clang::dyn_cast_or_null<clang::ReturnStmt>(ifstmt->getElse());
            }
            else
            {
              std::cout << "compound else_then is not safe to convert" << std::endl;
              return;
            }
          }
        }

        clang::Stmt *condstmt = const_cast<clang::Stmt *>(
            result.Nodes.getNodeAs<clang::Stmt>("condStmt"));

        if (simple_if && simple_else && two_branches)
        {
          // give offset 1 as expression is simple_if and simple_else to include the semicolon
          rep = gen_new_expression_with_offset(condstmt, ifstmt, if_return_stmt, else_return_stmt, 1u, src_manager);
          std::cout << "\033[1;31m generated replacement for simple-if-simple-else\033[0m\n" << std::endl;
          if (!dry_run_)
          {
            // use file name to select correct Replacements
            auto &reps = find_repls(ifstmt, src_manager, rep_map_);
            if (auto e = reps.add(rep))
            {
              // llvm::outs() << e;
              std::cout << "rep fp: " << rep.getFilePath().str() << "\nreps file path: " << reps.begin()->getFilePath().str() << std::endl;
              HERE("add replacement failed");
            }
          }
        }
        else if (simple_if && two_branches)
        {
          rep = gen_new_expression(condstmt, ifstmt, if_return_stmt, else_return_stmt, src_manager);
          std::cout << "\033[1;31m generated replacement for simple-if-else\033[0m\n" << std::endl;
          if (!dry_run_)
          {
            // use file name to select correct Replacements
            auto &reps = find_repls(ifstmt, src_manager, rep_map_);
            if (auto e = reps.add(rep))
            {
              // llvm::outs() << e;
              std::cout << "rep fp: " << rep.getFilePath().str() << "\nreps file path: " << reps.begin()->getFilePath().str() << std::endl;
              HERE("add replacement failed");
            }
          }
        }
        else if (simple_else && two_branches)
        {
          rep = gen_new_expression_with_offset(condstmt, ifstmt, if_return_stmt, else_return_stmt, 1u, src_manager);
          std::cout << "\033[1;31m generated replacement for if-simple-else\033[0m\n" << std::endl;
          if (!dry_run_)
          {
            // use file name to select correct Replacements
            auto &reps = find_repls(ifstmt, src_manager, rep_map_);
            if (auto e = reps.add(rep))
            {
              // llvm::outs() << e;
              std::cout << "rep fp: " << rep.getFilePath().str() << "\nreps file path: " << reps.begin()->getFilePath().str() << std::endl;
              HERE("add replacement failed");
            }
          }
        }
        else if(two_branches)
        {
          rep = gen_new_expression(condstmt, ifstmt, if_return_stmt, else_return_stmt, src_manager);
          std::cout << "\033[1;31m generated replacement for if-else\033[0m\n" << std::endl;
          if (!dry_run_)
          {
            // use file name to select correct Replacements
            auto &reps = find_repls(ifstmt, src_manager, rep_map_);
            if (auto e = reps.add(rep))
            {
              // llvm::outs() << e;
              std::cout << "rep fp: " << rep.getFilePath().str() << "\nreps file path: " << reps.begin()->getFilePath().str() << std::endl;
              HERE("add replacement failed");
            }
          }
        }
      }
      else
      {
        check_ptr(ifstmt, "ifstmt");
      }

      return;
    } // run

    matchers_t mk_branch_matcher() const override
    {
      return {mk_compound_if_matcher(), mk_if_simple_else_matcher(), mk_simple_if_else_matcher(), mk_simple_if_simple_else_matcher()};
    }

    /** \brief Ctor
      \param reps: pointer to clang::Replacements object, as in
      tool.getReplacements() \param new_param: new parameter text, e.g.
      "std::vector<block_t> & blocks" \param targ_fns: vector of target functions
      \param dry_run: unsure about all this?
       */
    if_else_simple_refactorer(replacements_map_t &rep_map,
                              bool const dry_run)
        : Base(rep_map, dry_run)
    {
    } // ctor
  };  // if_else_simple_refactorer

  const string_t if_else_simple_refactorer::if_bind_name = "if_else_bind_name";
} // namespace corct

#endif // include guard

// End of file
