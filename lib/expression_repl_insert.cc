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

namespace corct {

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
gen_new_expression(clang::Stmt * condstmt,
                  clang::IfStmt * ifstmt,
                  clang::ReturnStmt * if_return_stmt,
                  clang::ReturnStmt * else_return_stmt,
                  clang::SourceManager const & sm)
{
  using namespace clang;

  //construct expression
  std::stringstream s;

  std::string cond = get_source_text(condstmt->getSourceRange(), sm).str();
  /*instead of getting return value expression shenanigans just get the
  * source range and do + len("return") i.e. 6 for now
  */
  std::string if_return_val = get_source_text(if_return_stmt->getRetValue()->getSourceRange(), sm).str();

  s << cond << " " << if_return_val << "\n";
  std::cout << s.str();

  return Replacement();
}  // gen_new_expression

}  // namespace corct

// End of file
