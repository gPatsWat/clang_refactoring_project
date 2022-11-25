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
  corct::string_t tabs = "\t";
  o << "ifStmt";
}  // print_short_function_decl_details

}  // namespace corct

// End of file
