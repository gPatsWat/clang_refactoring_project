// function_signature_expander.h
// Oct 06, 2016
// (c) Copyright 2016 LANSLLC, all rights reserved

#ifndef IF_ELSE_SIMPLE_REFACTORER_H
#define IF_ELSE_SIMPLE_REFACTORER_H

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Core/Replacement.h"
#include "clang/Tooling/Tooling.h"
#include "branchsite_common.h"
#include "branchless_repl_gen.h"
#include "types.h"
#include <iostream>

namespace corct {

struct expand_signature_traits {
  using matcher_t = clang::ast_matchers::StatementMatcher;
};

/** replaces the if branch by branchless expression.
 *  The local variable set to the condition (if enabled)
 *  is inserted before the
 *  if-else clause.
*/
class if_else_simple_refactorer
    : public branch_replacement_generator<expand_signature_traits> {
public:
  using Base = branch_replacement_generator<expand_signature_traits>;
  static const string_t if_bind_name;

  void run(result_t const & result) override
  {
    clang::IfStmt * ifstmt = const_cast<clang::IfStmt *>(
        result.Nodes.getNodeAs<clang::IfStmt>(if_bind_name));
    clang::SourceManager & src_manager(
        const_cast<clang::SourceManager &>(result.Context->getSourceManager()));
    if(ifstmt) {
        replacement_t rep;
    //   replacement_t rep = gen_new_signature(ifstmt, new_str_, src_manager);
      if(!dry_run_) {
        // use file name to select correct Replacements
        auto & reps = find_repls(ifstmt, src_manager, rep_map_);
        if(reps.add(rep)) { HERE("add replacement failed"); }
      }
    }
    else {
      check_ptr(ifstmt, "ifstmt");
    }
    return;
  }  // run

  matcher_t mk_branch_matcher(str_t_cr if_else_bind_name) const override
  {
    return mk_simple_if_else_return_matcher(if_else_bind_name);
  }

  /** \brief Ctor
    \param reps: pointer to clang::Replacements object, as in
    tool.getReplacements() \param new_param: new parameter text, e.g.
    "std::vector<block_t> & blocks" \param targ_fns: vector of target functions
    \param dry_run: unsure about all this?
     */
  if_else_simple_refactorer(replacements_map_t & rep_map,
                              vec_str const & targ_fns,
                              string_t const & new_param,
                              bool const dry_run)
      : Base(rep_map, targ_fns, new_param, dry_run)
  {
  }  // ctor
};   // if_else_simple_refactorer

const string_t if_else_simple_refactorer::if_bind_name = "simple_if_else_return";
}  // namespace corct

#endif  // include guard

// End of file
