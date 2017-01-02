#pragma once
#ifndef BE_BLT_WHILE_NODE_HPP_
#define BE_BLT_WHILE_NODE_HPP_

#include "node.hpp"
#include <be/core/console_indent.hpp>

namespace be {
namespace blt {

struct WhileNode {
   Node expr;
   Node block;

   void operator()(std::ostream& os) const {
      os << nl << "while " << indent << indent;
      expr(os);
      os << " do " << unindent;
      if (!is_empty(block)) {
         block(os);
      }
      os << nl << "::__continue__::" << unindent << nl << "end ";
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
      debug_cir("While", expr, block, os, ctx);
   }

};

} // be::blt
} // be

#endif
