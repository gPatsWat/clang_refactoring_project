// callsite_lister.cc
// Feb 02, 2018
// (c) Copyright 2018 LANSLLC, all rights reserved

#include "branchsite_lister.h"

namespace corct {

void
print_branch_details(clang::Stmt const * stmt,
                   clang::IfStmt const * ifstmt,
                   clang::SourceManager & sm,
                   std::ostream & o)
{
  using namespace std;

  corct::string_t tabs = "\t";

  o << "ifStmt:\n"
    << tabs << "condition:"
    << tabs << get_source_text(stmt->getSourceRange(), sm).str() << "\n"
    #ifdef PRINT_SOURCELOC
    << tabs << "callsite source range: "
    << tabs << fullSourceRangeAsString(ifstmt->getSourceRange(), &sm)
    #endif
    << "\n";

  clang::IfStmt const * if_iter = ifstmt;

  while(if_iter) {
    if(auto elseptr = clang::dyn_cast_or_null<clang::IfStmt const>(if_iter->getElse())) {
      o << "elseStmt:\n"
        << tabs << "condition:"
        << tabs << get_source_text(elseptr->getCond()->getSourceRange(), sm).str() << "\n"
        #ifdef PRINT_SOURCELOC
        << tabs << "callsite source range: "
        << tabs << fullSourceRangeAsString(ifstmt->getSourceRange(), &sm)
        #endif
        << "\n";
        if_iter = elseptr;
    }
    else if(auto stmtptr = clang::dyn_cast_or_null<clang::Stmt const>(if_iter->getElse())){
      o << "last nonelse statement:\n"
        << tabs << get_source_text(stmtptr->getSourceRange(), sm).str() << "\n"
        #ifdef PRINT_SOURCELOC
        << tabs << "callsite source range: "
        << tabs << fullSourceRangeAsString(ifstmt->getSourceRange(), &sm)
        #endif
        << "\n";
        if_iter = clang::dyn_cast_or_null<clang::IfStmt const>(stmtptr);
    }
    else {
      o << "panic: unknown statement type or branch end!!";
      break;
    }
  }

}  // print_if_condition_details

void
print_branch_details(clang::Expr const * stmt,
                   clang::IfStmt const * ifstmt,
                   clang::SourceManager & sm,
                   std::ostream & o)
{
  using namespace std;
  corct::string_t tabs = "\t";

  o << "ifStmt:\n"
    << tabs << get_source_text(stmt->getSourceRange(), sm).str() << "\n"
    << tabs << "callsite source range: "
    << tabs << fullSourceRangeAsString(ifstmt->getSourceRange(), &sm)
    << "\n";
}  // print_if_condition_details

}  // namespace corct

// End of file
