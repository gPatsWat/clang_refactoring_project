// signature_insert.h
// Oct 06, 2016
// (c) Copyright 2016 LANSLLC, all rights reserved

#ifndef EXPRESSION_REPL_INSERT_H
#define EXPRESSION_REPL_INSERT_H

#include "clang/ASTMatchers/ASTMatchers.h"
#include "types.h"

// forward decls
namespace clang {
class FunctionDecl;
class SourceManager;
class CallExpr;

namespace tooling {

class Replacement;

}  // namespace tooling

}  // namespace clang

namespace corct {

clang::tooling::Replacement
gen_new_expression_with_offset(clang::Stmt const *condstmt,
                     clang::IfStmt const *ifstmt,
                     clang::ReturnStmt const *if_return_stmt,
                     clang::ReturnStmt const *else_return_stmt,
                     unsigned int offset,
                     clang::SourceManager const &sm);

clang::tooling::Replacement
gen_new_expression(clang::Stmt const * condstmt,
                  clang::IfStmt const * ifstmt,
                  clang::ReturnStmt const * if_return_stmt,
                  clang::ReturnStmt const * else_return_stmt,
                  clang::SourceManager const & sm);

clang::tooling::Replacement
gen_new_expression(clang::Stmt const * condstmt,
                  clang::Stmt const * then_compound_stmt,
                  clang::IfStmt const * ifstmt,
                  clang::ReturnStmt const * if_return_stmt,
                  clang::ReturnStmt const * else_return_stmt,
                  clang::SourceManager const & sm);
}  // namespace corct

#endif  // include guard

// End of file
