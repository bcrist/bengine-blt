#pragma once
#ifndef BE_BLT_EXPR_LIST_NODE_HPP_
#define BE_BLT_EXPR_LIST_NODE_HPP_

#include "node.hpp"
#include <vector>

namespace be::blt {

struct ExprListNode {
   std::vector<Node> exprs;

   void operator()(std::ostream& os) const {
      bool first = true;
      for (auto& expr : exprs) {
         if (first) {
            first = false;
         } else {
            os << ", ";
         }
         expr(os);
      }
   }

   bool is_literal() const {
      return false;
   }

   bool is_static_constant() const {
      for (auto& node : exprs) {
         if (!node.is_static_constant()) {
            return false;
         }
      }
      return true;
   }

   bool is_nonnil_constant() const {
      for (auto& node : exprs) {
         if (!node.is_nonnil_constant()) {
            return false;
         }
      }
      return true;
   }

   bool is_nullipotent() const {
      for (auto& node : exprs) {
         if (!node.is_nullipotent()) {
            return false;
         }
      }
      return true;
   }

   void debug(std::ostream& os, NodeDebugContext& ctx) const {
      debug_c("ExprList", os, ctx.c_prefix, ctx.last_line_empty);
      
      for (auto it = exprs.begin(), end = exprs.end(); it != end; ++it) {
         if (it + 1 != end) {
            debug_i(*it, os, ctx.r_prefix, ctx.last_line_empty);
         } else {
            debug_r(*it, os, ctx.r_prefix, ctx.last_line_empty);
         }
      }
   }

};

} // be::blt

#endif
