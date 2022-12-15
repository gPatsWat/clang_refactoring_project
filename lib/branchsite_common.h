#pragma once

#include "clang/ASTMatchers/ASTMatchers.h"

namespace corct {
    using namespace clang::ast_matchers;

     /**
     * @brief matches any if condition whose parents are not conditions.
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
     * @brief matches any if condition with only returs.
     *
     * @return auto
     */
    inline auto mk_compound_if_matcher() {
        return ifStmt(unless(isExpansionInSystemHeader()),
        unless(hasAncestor(ifStmt())),
        hasCondition(stmt().bind("condStmt")),
        hasThen(compoundStmt(has(returnStmt().bind("if_return_stmt"))).bind("then_compound_stmt")),
        hasElse(compoundStmt(has(returnStmt().bind("else_return_stmt"))).bind("else_compound_stmt"))).bind("if_else_bind_name");
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
    inline auto mk_if_simple_else_matcher() {
        // return ifStmt(unless(isExpansionInSystemHeader()),
        //        unless(hasAncestor(ifStmt())),
        //        hasCondition(stmt().bind("condStmt")),
        //        hasThen(compoundStmt(hasAnySubstatement(returnStmt().bind("if_return_stmt")))),
        //        hasElse(returnStmt().bind("else_return_stmt"))).bind(if_else_bind_name);
        return ifStmt(unless(isExpansionInSystemHeader()),
               unless(hasAncestor(ifStmt())),
               hasCondition(stmt().bind("condStmt")),
               hasThen(compoundStmt(has(returnStmt().bind("if_return_stmt"))).bind("then_compound_stmt")),
               hasElse(returnStmt().bind("else_return_stmt"))).bind("if_simple_else_bind_name");
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
    inline auto mk_simple_if_else_matcher() {
        return ifStmt(unless(isExpansionInSystemHeader()),
               unless(hasAncestor(ifStmt())),
               hasCondition(stmt().bind("condStmt")),
               hasThen(returnStmt().bind("if_return_stmt")),
               hasElse(compoundStmt(has(returnStmt().bind("else_return_stmt"))).bind("else_compound_stmt"))).bind("simple_if_else_bind_name");
    }

    /**
     * @brief matches simple-if-simple-else branch
     * Eg:
     *  if(a || b)
     *      return 5;
     *  else
     *      return -1;
     */
    inline auto mk_simple_if_simple_else_matcher() {
        return ifStmt(unless(isExpansionInSystemHeader()),
               unless(hasAncestor(ifStmt())),
               hasCondition(stmt().bind("condStmt")),
               hasThen(returnStmt().bind("if_return_stmt")),
               hasElse(returnStmt().bind("else_return_stmt"))).bind("simple_if_simple_else_bind_name");
    }

} //namespace corct

//end of file