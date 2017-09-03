#pragma once
#ifndef BE_BLT_PAREN_EXPR_NODE_HPP_
#define BE_BLT_PAREN_EXPR_NODE_HPP_

#include "node.hpp"
#include "token_type.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct ParenExprNode : Node {
   std::unique_ptr<Node> expr;

   ParenExprNode(std::unique_ptr<Node> expr) : expr(std::move(expr)) { }

   virtual void operator()(std::ostream& os) const override {
      os << '(';
      (*expr)(os);
      os << ')';
   }

   virtual bool is_literal() const override {
      return expr->is_literal();
   }

   virtual bool is_static_constant() const override {
      return expr->is_static_constant();
   }

   virtual bool is_nonnil_constant() const override {
      return expr->is_nonnil_constant();
   }

   virtual bool is_nullipotent() const override {
      return expr->is_nullipotent();
   }

   virtual void debug(std::ostream& os, NodeDebugContext& ctx) const override {
      debug_cr("ParenExpr", expr, os, ctx);
   }

};

} // be::blt

#endif
