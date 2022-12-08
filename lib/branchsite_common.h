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

    /**
     * @brief matches simple if-else branch with a return statement
     * somewhere in the then clause and only return and no condition
     * in else clause.
     * Eg:
     *  if(a || b) {
     *      return 5;
     *  }
     *  else return -1;
     *
     * ifStmt(unless(isExpansionInSystemHeader()), unless(hasAncestor(ifStmt())), hasThen(compoundStmt(hasAnySubstatement(returnStmt()))), hasElse(returnStmt()))
     *
     */
    inline auto mk_simple_if_else_return_matcher(std::string const & if_else_bind_name) {
        return ifStmt(unless(isExpansionInSystemHeader()),
               unless(hasAncestor(ifStmt())),
               hasThen(compoundStmt(hasAnySubstatement(returnStmt()))),
               hasElse(returnStmt())).bind(if_else_bind_name);
    }

} //namespace corct

//end of file