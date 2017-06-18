#pragma once
#ifndef BE_BLT_WRAPPED_EXPR_NODE_HPP_
#define BE_BLT_WRAPPED_EXPR_NODE_HPP_

#include "node.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct WrappedExprNode {
   Node expr;

   void operator()(std::ostream& os) const {
      os << nl << "__ctx__:write( " << indent;
      expr(os);
      os << " ) " << unindent;
   }

   bool is_literal() const {
      return false;
   }

   bool is_static_constant() const {
      return false;
   }

   bool is_nonnil_constant() const {
      return false;
   }

   bool is_nullipotent() const {
      return false;
   }

   void debug(std::ostream& os, NodeDebugContext& ctx) const {
      debug_cr("WriteExpr", expr, os, ctx);
   }

};

} // be::blt

#endif
