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
gen_new_expression(clang::Stmt * condstmt,
                  clang::IfStmt * ifstmt,
                  clang::ReturnStmt * if_return_stmt,
                  clang::ReturnStmt * else_return_stmt,
                  clang::SourceManager const & sm);

// clang::tooling::Replacement
// gen_new_call(clang::CallExpr * call_expr,
//              clang::FunctionDecl * func_decl,
//              std::string const & new_arg_text,
//              clang::SourceManager const & sm,
//              bool const verbose = false);

// clang::tooling::Replacement
// gen_signature_repl(clang::FunctionDecl * f_decl,
//                    std::string const & old_name,
//                    std::string const & type_name,
//                    std::string const & new_param,
//                    clang::SourceManager const & sm,
//                    bool const verbose = false);

}  // namespace corct

#endif  // include guard

// End of file
