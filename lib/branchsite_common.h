#pragma once

#include "clang/ASTMatchers/ASTMatchers.h"

namespace corct {
    using namespace clang::ast_matchers;

    inline auto mk_if_matcher() {
        return ifStmt(unless(isExpansionInSystemHeader()),
                hasCondition(expr().bind("condStmt"))).bind("ifStmt");
    }
} //namespace corct

//end of file