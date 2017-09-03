#pragma once
#ifndef BE_BLT_WRAPPED_EXPR_NODE_HPP_
#define BE_BLT_WRAPPED_EXPR_NODE_HPP_

#include "node.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct WrappedExprNode : Node {
   std::unique_ptr<Node> expr;

   WrappedExprNode(std::unique_ptr<Node> expr)
      : expr(std::move(expr)) { }

   virtual void operator()(std::ostream& os) const override {
      os << nl << "__ctx__:write( " << indent;
      (*expr)(os);
      os << " ) " << unindent;
   }

   virtual bool is_literal() const override {
      return false;
   }

   virtual bool is_static_constant() const override {
      return false;
   }

   virtual bool is_nonnil_constant() const override {
      return false;
   }

   virtual bool is_nullipotent() const override {
      return false;
   }

   virtual void debug(std::ostream& os, NodeDebugContext& ctx) const override {
      debug_cr("WriteExpr", expr, os, ctx);
   }

};

} // be::blt

#endif
