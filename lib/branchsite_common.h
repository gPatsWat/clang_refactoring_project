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
        hasCondition(stmt().bind("condStmt")),
        hasThen(stmt().bind("if_then_stmt")),
        hasElse(stmt().bind("else_stmt"))).bind("if_else_bind_name");
    }

    /**
     * @brief matches if-simple-else branch
     * Eg:
     *  if(a || b) {
     *      //can have side effects
     *      return 5;
     *  }
     *  else return -1;
     *
     */
    inline auto mk_if_simple_else_matcher(std::string const & if_else_bind_name) {
        // return ifStmt(unless(isExpansionInSystemHeader()),
        //        unless(hasAncestor(ifStmt())),
        //        hasCondition(stmt().bind("condStmt")),
        //        hasThen(compoundStmt(hasAnySubstatement(returnStmt().bind("if_return_stmt")))),
        //        hasElse(returnStmt().bind("else_return_stmt"))).bind(if_else_bind_name);
        return ifStmt(unless(isExpansionInSystemHeader()),
               unless(hasAncestor(ifStmt())),
               hasCondition(stmt().bind("condStmt")),
               hasThen(compoundStmt(has(returnStmt().bind("if_return_stmt"))).bind("then_compound_stmt")),
               hasElse(returnStmt().bind("else_return_stmt"))).bind(if_else_bind_name);
    }

    /**
     * @brief matches simple-if-else branch
     * Eg:
     *  if(a || b)
     *      return 5;
     *  else {
     *      can have side effects
     *      return -1;
     *  }
     *
     */
    inline auto mk_simple_if_else_matcher(std::string const & if_else_bind_name) {
        return ifStmt(unless(isExpansionInSystemHeader()),
               unless(hasAncestor(ifStmt())),
               hasCondition(stmt().bind("condStmt")),
               hasThen(compoundStmt(has(returnStmt().bind("if_return_stmt"))).bind("then_compound_stmt")),
               hasElse(returnStmt().bind("else_return_stmt"))).bind(if_else_bind_name);
    }

} //namespace corct

//end of file