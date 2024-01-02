//===--- InvalidArgumentInStrerrorCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "InvalidArgumentInStrerrorCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Preprocessor.h"
#include <optional>
#include <iostream>

using namespace clang::ast_matchers;

namespace clang::tidy::bugprone {

void InvalidArgumentInStrerrorCheck::registerMatchers(MatchFinder *Finder) {
  auto ExpressionMatcher = unaryOperator(hasOperatorName("-"), hasUnaryOperand(declRefExpr(to(varDecl(hasName("errno"))))));
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName("::strerror", "::strerror_r", "::strerror_l", "::strerrorname_np", "::strerrordesc_np"))),
               hasArgument(0, anyOf(ExpressionMatcher.bind("neg_strerror"),
                                    expr().bind("exprs")))),
      this);
}

static Preprocessor *PP;

void InvalidArgumentInStrerrorCheck::check(const MatchFinder::MatchResult &Result) {
  Expr::EvalResult EvalResult;
  const Expr *MatchedExpr = Result.Nodes.getNodeAs<Expr>("exprs");
  if (MatchedExpr && MatchedExpr->EvaluateAsInt(EvalResult, *Result.Context)) {
    std::cout << "getKind: " << EvalResult.Val.getKind() << '\n';
    std::cout << "isInt: " << EvalResult.Val.isInt() << '\n';
  }
  const UnaryOperator *MatchedExpr2 = Result.Nodes.getNodeAs<UnaryOperator>("neg_strerror");
  if (MatchedExpr2 && MatchedExpr2->EvaluateAsInt(EvalResult, *Result.Context)) {
    std::cout << "neg_strerror matched\n";
  }
}

void InvalidArgumentInStrerrorCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *Pp, Preprocessor *ModuleExpanderPP) {
  PP = Pp;
}

} // namespace clang::tidy::bugprone
