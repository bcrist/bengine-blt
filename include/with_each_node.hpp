#pragma once
#ifndef BE_BLT_WITH_EACH_NODE_HPP_
#define BE_BLT_WITH_EACH_NODE_HPP_

#include "node.hpp"
#include <be/core/console_indent.hpp>

namespace be {
namespace blt {

struct WithEachNode {
   Node expr;
   Node block;

   void operator()(std::ostream& os) const {
      os << nl << "__ctx__:push( " << indent;
      expr(os);
      os << unindent << " )";
      os << nl << "if __blt__.is_sequence(__ctx__:get()) then" << indent;
      os << nl << "for __k__ = 1, #(__ctx__:get()) do" << indent;
      os << nl << "__ctx__:push(__ctx__:ref(__k__)) ";
      if (!is_empty(block)) {
         block(os);
      }
      os << nl << "__ctx__:pop()";
      os << unindent << nl << "end";
      os << unindent << nl << "else" << indent;
      os << nl << "for __k__ in pairs(__ctx__:get()) do" << indent;
      os << nl << "__ctx__:push(__ctx__:ref(__k__)) ";
      if (!is_empty(block)) {
         block(os);
      }
      os << nl << "__ctx__:pop()";
      os << unindent << nl << "end";
      os << unindent << nl << "end";
      os << nl << "__ctx__:pop() ";
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
      debug_lcr(expr, "WithEach", block, os, ctx);
   }

};

} // be::blt
} // be

#endif
