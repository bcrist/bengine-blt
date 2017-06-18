#pragma once
#ifndef BE_BLT_FOR_NODE_HPP_
#define BE_BLT_FOR_NODE_HPP_

#include "node.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct ForNode {
   Node init;
   Node condition;
   Node update;
   Node block;

   void operator()(std::ostream& os) const {
      if (!is_empty(init)) {
         os << nl << "do " << indent;
         init(os);
         write_loop(os);
         os << unindent << nl << "end ";
      } else {
         write_loop(os);
      }
   }

   void write_loop(std::ostream& os) const {
      os << nl << "while " << indent << indent;
      if (!is_empty(condition)) {
         condition(os);
      } else {
         os << "true";
      }
      os << " do " << unindent;
      write_block(os);
      os << unindent << nl << "end ";
   }

   void write_block(std::ostream& os) const {
      if (!is_empty(block)) {
         block(os);
      }
      os << nl << "::__continue__:: ";
      if (!is_empty(update)) {
         update(os);
      }
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
      debug_l(init, os, ctx.l_prefix, ctx.last_line_empty);
      debug_c("For", os, ctx.c_prefix, ctx.last_line_empty);
      debug_i(condition, os, ctx.r_prefix, ctx.last_line_empty);
      debug_i(block, os, ctx.r_prefix, ctx.last_line_empty);
      debug_r(update, os, ctx.r_prefix, ctx.last_line_empty);
   }

};

} // be::blt

#endif
