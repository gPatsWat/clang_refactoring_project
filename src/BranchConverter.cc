// LoopConvert.cc
// Aug 17, 2016
// (c) Copyright 2016 LANSLLC, all rights reserved

/* Like GlobalDetect, only now we're trying to replace some stuff. */

#include "dump_things.h"
#include "if_else_simple_refactorer.h"
#include "make_replacement.h"
#include "utilities.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/Type.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Core/Replacement.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include <iostream>
#include <vector>

using namespace clang::tooling;
using namespace llvm;

using corct::vec_str;

const char * addl_help =
    "Replace global variable references with locals, or thread new local "
    "variables through call chains";

static llvm::cl::OptionCategory CompilationOpts("Global Replace options:");

static llvm::cl::opt<bool> expand_func(
    "newvar",
    llvm::cl::desc("insert new local variable capturing each condition"),
    llvm::cl::cat(CompilationOpts),
    llvm::cl::init(false));

static cl::opt<std::string> target_func_string(
    "tf",
    cl::desc("target function(s) to modify, separated by commas if nec. E.g. "
             "-tf=\"foo,bar,baz\""),
    cl::value_desc("target-function-string"),
    cl::cat(CompilationOpts));

static cl::opt<std::string> new_func_param_string(
    "np",
    cl::desc("param to add to the global-using functions' signature (with "
             "-xpnd), e.g. -np=\"SimConfig const & sim_config\""),
    cl::value_desc("new-param-string"),
    cl::cat(CompilationOpts));

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp(
    "Replace branches with a branchless expression.\n"
    "Additionally, can add a new local variable capturing condition in if clause\n");

static cl::opt<bool> dry_run("d",
                             cl::desc("dry run"),
                             cl::cat(CompilationOpts),
                             cl::init(false));

void
announce_dry(bool const dry_run_)
{
  if(dry_run_) { std::cout << "This is a dry run\n"; }
  else {
    std::cout << "This is not a dry run\n";
  }
  return;
}  // announce_dry

void
list_compilations(CommonOptionsParser & opt_prs)
{
  auto & comps(opt_prs.getCompilations());
  std::cout << "# of compile commands: " << comps.getAllCompileCommands().size()
            << "\n";
  std::cout << "Sources from compilation:\n";
  for(auto & source : opt_prs.getSourcePathList()) {
    std::cout << source << "\n";
  }
  return;
}

int
main(int argc, const char ** argv)
{
  using corct::replacements_map_t;
  using corct::split;
  CommonOptionsParser opt_prs(argc, argv, CompilationOpts, addl_help);

  RefactoringTool tool(opt_prs.getCompilations(), opt_prs.getSourcePathList());

  announce_dry(dry_run);
  list_compilations(opt_prs);

  replacements_map_t & rep_map = tool.getReplacements();


  // sort out target functions
  vec_str targ_fns(split("", ','));

  corct::if_else_simple_refactorer if_refactorer(rep_map, dry_run);

//   corct::expand_callsite s_expander(rep_map, targ_fns, new_func_arg_string,
//                                     dry_run);

  clang::ast_matchers::MatchFinder finder;

  corct::if_else_simple_refactorer::matchers_t branch_matchers =
      if_refactorer.branch_matchers();

  for(auto  & m : branch_matchers) { finder.addMatcher(m, &if_refactorer); }

  tool.runAndSave(newFrontendActionFactory(&finder).get());

  llvm::outs() << "Replacements collected: \n";
  for(auto & p : tool.getReplacements()) {
    llvm::outs() << "file: " << p.first << ":\n";
    for(auto & r : p.second) { llvm::outs() << r.toString() << "\n"; }
  }
  return 0;
}  // main

// End of file
