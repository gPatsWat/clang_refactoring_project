// replacement_generator.h
// Oct 11, 2016
// (c) Copyright 2016 LANSLLC, all rights reserved

#ifndef BRANCHLESS_REPL_GEN_H
#define BRANCHLESS_REPL_GEN_H

#include "types.h"
#include "utilities.h"

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/Core/Replacement.h"
#include "clang/Tooling/Tooling.h"

namespace corct {
/** \brief Base class for simple if-else refactorizations.

TODO: edit this
It supports generating a set of matchers for named targets (one matcher
per target). It maintains state like a pointer to a Clang Replacements
data structure. It maintains a string that indicates some kind of new
text, such as a new parameter text or new argument text. Finally, it has a dry
run indicator.

*/
template <typename traits_t>
class branchless_replacement_generator
    : public clang::ast_matchers::MatchFinder::MatchCallback {
public:
  // types and constants
  using matcher_t = typename traits_t::matcher_t;
  using matchers_t = std::vector<matcher_t>;

  // interface
  /** \brief Callback when a matcher finds a match. */
  virtual void run(const result_t & result) override = 0;

  /**\brief Generate an AST matcher for target branch. */
  virtual matcher_t mk_branch_matcher() const = 0;

  /** \brief Get a set of branch matchers to which derived class will respond.
   */
  matchers_t branch_matchers() const
  {
    matchers_t ms;
    for(auto const & t : br_targets_) { ms.push_back(mk_branch_matcher()); }
    return ms;
  }

  branchless_replacement_generator(replacements_map_t & rep_map,
                                 vec_str const & br_targets,
                                 str_t_cr new_str,
                                 bool const dry_run)
      : rep_map_(rep_map),
        br_targets_(br_targets),
        new_str_(new_str),
        dry_run_(dry_run)
  {
  }

  virtual ~branchless_replacement_generator() {}

  replacements_t const & get_replacements(string_t const & bname) const
  {
    return rep_map_[bname];
  }

  replacements_map_t const & get_replacements_map() const { return rep_map_; }

  // state
protected:
  replacements_map_t & rep_map_;
  vec_str const br_targets_;    //!< branch targets
  str_t_cr new_str_;
  bool const dry_run_;
};  // replacement_generator

}  // namespace corct

#endif  // include guard
// End of file
