#pragma once

#include "clang/ASTMatchers/ASTMatchers.h"

namespace corct {
    using namespace clang::ast_matchers;

    /**
     * @brief matches any if condition.
     * Note: the actual logic of whether it is a lone if or accompanied with
     * else conditions is done by parsing the AST rather than creating custom
     * matchers (which is a lot harder and less flexible).
     *
     * @return auto
     */
    inline auto mk_if_matcher() {
        return ifStmt(unless(isExpansionInSystemHeader()),
                unless(hasAncestor(ifStmt())),
                hasCondition(stmt().bind("condStmt"))).bind("ifStmt");
    }

} //namespace corct

//end of file