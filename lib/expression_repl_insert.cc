// signature_insert.cc
// Oct 06, 2016
// (c) Copyright 2016 LANSLLC, all rights reserved

#include "clang/AST/Type.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Core/Replacement.h"
#include "clang/Tooling/Tooling.h"
#include "dump_things.h"
#include "make_replacement.h"
#include "types.h"
#include "utilities.h"

#include "expression_repl_insert.h"

#include <iostream>
#include <sstream>

namespace corct
{

  using clang::CallExpr;
  using clang::FunctionDecl;
  using clang::ParmVarDecl;
  using clang::SourceManager;
  using clang::tooling::Replacement;

  /**
   * @brief generate replacement for simple if-else-expression
   * assumes that it will only be called by simple is-else-expression matcher.
   *
   * @param f_decl
   * @param new_param_text
   * @param sm
   * @return Replacement
   */
  Replacement
  gen_new_expression(clang::Stmt const *condstmt,
                     clang::IfStmt const *ifstmt,
                     clang::ReturnStmt const *if_return_stmt,
                     clang::ReturnStmt const *else_return_stmt,
                     clang::SourceManager const &sm)
  {
    using namespace clang;

    // stringstream for easy construction of branchless expression
    std::stringstream rep_str;

    std::string cond = get_source_text(condstmt->getSourceRange(), sm).str();
    /*instead of getting return value expression shenanigans just get the
     * source range and do + len("return") i.e. 6 for now
     */
    std::string if_return_val = get_source_text(if_return_stmt->getRetValue()->getSourceRange(), sm).str();
    std::string else_return_val = get_source_text(else_return_stmt->getRetValue()->getSourceRange(), sm).str();

    // construct branchless expression
    rep_str << "(" << cond << ")*" << "(" << if_return_val << ")"
      << " + " << "!(" << cond << ")*" << "(" << else_return_val << ");";

    std::cout << rep_str.str() << std::endl;
    dumpSourceRange(ifstmt->getSourceRange(), &sm);

    //add offset of one for simple else return (without braces)
    return replace_source_range_with_offset(sm, ifstmt->getSourceRange(), 1, rep_str.str());

  } // gen_new_expression

  /**
   * @brief generate replacement for complex if-else-expression
   * assumes that it will only be called by simple is-else-expression matcher.
   *
   * @param condstmt
   * @param then_compound_stmt
   * @param ifstmt
   * @param if_return_stmt
   * @param else_return_stmt
   * @param sm
   * @return clang::tooling::Replacement
   */
  clang::tooling::Replacement
  gen_new_expression(clang::Stmt const *condstmt,
                     clang::Stmt const *then_compound_stmt,
                     clang::IfStmt const *ifstmt,
                     clang::ReturnStmt const *if_return_stmt,
                     clang::ReturnStmt const *else_return_stmt,
                     clang::SourceManager const &sm)
  {
    std::cout << "then expression contains side effects!\n";
    return Replacement();
  }

} // namespace corct

// End of file
