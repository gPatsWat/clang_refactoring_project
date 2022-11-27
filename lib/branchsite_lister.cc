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
    << tabs << get_source_text(stmt->getSourceRange(), sm) << "\n"
    << tabs << "callsite source range: "
    << tabs << fullSourceRangeAsString(ifstmt->getSourceRange(), &sm)
    << "\n";
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
    << tabs << get_source_text(stmt->getSourceRange(), sm) << "\n"
    << tabs << "callsite source range: "
    << tabs << fullSourceRangeAsString(ifstmt->getSourceRange(), &sm)
    << "\n";
}  // print_if_condition_details

}  // namespace corct

// End of file
