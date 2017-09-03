#pragma once
#ifndef BE_BLT_EXPR_LIST_NODE_HPP_
#define BE_BLT_EXPR_LIST_NODE_HPP_

#include "node.hpp"
#include <vector>

namespace be::blt {

struct ExprListNode : Node {
   std::vector<std::unique_ptr<Node>> exprs;

   virtual void operator()(std::ostream& os) const override {
      bool first = true;
      for (auto& expr : exprs) {
         if (first) {
            first = false;
         } else {
            os << ", ";
         }
         (*expr)(os);
      }
   }

   virtual bool is_literal() const override {
      return false;
   }

   virtual bool is_static_constant() const override {
      for (auto& node : exprs) {
         if (!node->is_static_constant()) {
            return false;
         }
      }
      return true;
   }

   virtual bool is_nonnil_constant() const override {
      for (auto& node : exprs) {
         if (!node->is_nonnil_constant()) {
            return false;
         }
      }
      return true;
   }

   virtual bool is_nullipotent() const override {
      for (auto& node : exprs) {
         if (!node->is_nullipotent()) {
            return false;
         }
      }
      return true;
   }

   virtual void debug(std::ostream& os, NodeDebugContext& ctx) const override {
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
