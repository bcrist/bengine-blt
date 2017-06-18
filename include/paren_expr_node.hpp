#pragma once
#ifndef BE_BLT_PAREN_EXPR_NODE_HPP_
#define BE_BLT_PAREN_EXPR_NODE_HPP_

#include "node.hpp"
#include "token_type.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct ParenExprNode {
   Node expr;

   void operator()(std::ostream& os) const {
      os << '(';
      expr(os);
      os << ')';
   }

   bool is_literal() const {
      return expr.is_literal();
   }

   bool is_static_constant() const {
      return expr.is_static_constant();
   }

   bool is_nonnil_constant() const {
      return expr.is_nonnil_constant();
   }

   bool is_nullipotent() const {
      return expr.is_nullipotent();
   }

   void debug(std::ostream& os, NodeDebugContext& ctx) const {
      debug_cr("ParenExpr", expr, os, ctx);
   }

};

} // be::blt

#endif
