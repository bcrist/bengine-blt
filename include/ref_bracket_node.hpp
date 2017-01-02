#pragma once
#ifndef BE_BLT_REF_BRACKET_NODE_HPP_
#define BE_BLT_REF_BRACKET_NODE_HPP_

#include "node.hpp"
#include <be/core/console_indent.hpp>

namespace be {
namespace blt {

struct RefBracketNode {
   Node parent;
   Node expr;

   void operator()(std::ostream& os) const {
      parent(os);
      os << '[' << indent;
      expr(os);
      os << ']' << unindent;
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
      return true;
   }

   void debug(std::ostream& os, NodeDebugContext& ctx) const {
      debug_lcr(parent, "RefBracket", expr, os, ctx);
   }

};

} // be::blt
} // be

#endif
