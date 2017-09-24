#pragma once
#ifndef BE_BLT_CALL_NODE_HPP_
#define BE_BLT_CALL_NODE_HPP_

#include "token_data_node.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct CallNode : Node {
   std::unique_ptr<Node> ref;
   std::unique_ptr<Node> expr_list;
   SV method;

   CallNode(std::unique_ptr<Node> ref, std::unique_ptr<Node> expr_list)
      : ref(std::move(ref)), expr_list(std::move(expr_list)) { }

   CallNode(std::unique_ptr<Node> ref, std::unique_ptr<Node> expr_list, SV method)
      : ref(std::move(ref)), expr_list(std::move(expr_list)), method(method) { }

   virtual void operator()(std::ostream& os) const override {
      (*ref)(os);
      if (!method.empty()) {
         os << ":" << method;
      }
      os << '(' << indent;
      if (expr_list) {
         (*expr_list)(os);
      }
      os << ')' << unindent;
   }

   virtual bool is_literal() const override {
      return false;
   }

   virtual bool is_static_constant() const override {
      return false;
   }

   virtual bool is_nonnil_constant() const override {
      return false;
   }

   virtual bool is_nullipotent() const override {
      return false;
   }

   virtual void debug(std::ostream& os, NodeDebugContext& ctx) const override {
      debug_l(ref, os, ctx.l_prefix, ctx.last_line_empty);

      if (!method.empty()) {
         debug_i(S(":") + S(method.begin(), method.end()), os, ctx.l_prefix, ctx.last_line_empty);
      }

      debug_c("Call", os, ctx.c_prefix, ctx.last_line_empty);
      debug_r(expr_list, os, ctx.r_prefix, ctx.last_line_empty);
   }

};

} // be::blt

#endif
