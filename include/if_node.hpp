#pragma once
#ifndef BE_BLT_IF_NODE_HPP_
#define BE_BLT_IF_NODE_HPP_

#include "node.hpp"
#include <be/core/console_indent.hpp>
#include <vector>

namespace be {
namespace blt {

struct IfNode {
   std::vector<std::pair<Node, Node>> clauses;
   Node else_block;

   void operator()(std::ostream& os) const {
      if (!clauses.empty()) {
         const char* kw = "if ";
         for (auto& clause : clauses) {
            os << nl << kw << indent << indent;
            clause.first(os);
            os << " then " << unindent;
            if (!is_empty(clause.second)) {
               clause.second(os);
            }
            os << unindent;
            kw = "elseif ";
         }
         if (!is_empty(else_block)) {
            os << nl << "else " << indent;
            else_block(os);
            os << unindent;
         }
         os << nl << "end ";
      } else if (!is_empty(else_block)) {
         os << nl << "do " << indent;
         else_block(os);
         os << unindent << nl << "end ";
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
      debug_c("If", os, ctx.c_prefix, ctx.last_line_empty);
      debug_if(os, ctx);
   }

   void debug_if(std::ostream& os, NodeDebugContext& ctx) const {
      {
         NodeDebugContext ctx2 { ctx.r_prefix + " |  ", ctx.r_prefix + " +--", ctx.r_prefix + " |  ", ctx.last_line_empty };
         debug_clauses(os, ctx2);
      }
      {
         NodeDebugContext ctx2 { ctx.r_prefix + " |  ", ctx.r_prefix + " \\--", ctx.r_prefix + "    ", ctx.last_line_empty };
         debug_else(os, ctx2);
      }
   }

   void debug_clauses(std::ostream& os, NodeDebugContext& ctx) const {
      for (auto& clause : clauses) {
         debug_lcr(clause.first, "Clause", clause.second, os, ctx);
      }
   }

   void debug_else(std::ostream& os, NodeDebugContext& ctx) const {
      debug_cr("Else", else_block, os, ctx);
   }

};

} // be::blt
} // be

#endif
