#pragma once
#ifndef BE_BLT_UNTIL_NODE_HPP_
#define BE_BLT_UNTIL_NODE_HPP_

#include "node.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct UntilNode {
   Node block;
   Node stop_condition;

   void operator()(std::ostream& os) const {
      os << nl << "repeat " << indent;
      if (!is_empty(block)) {
         block(os);
      }
      os << unindent << nl << "until " << indent;
      stop_condition(os);
      os << unindent;
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
      debug_cir("Until ", block, stop_condition, os, ctx);
   }

};

} // be::blt

#endif
