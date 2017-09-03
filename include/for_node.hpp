#pragma once
#ifndef BE_BLT_FOR_NODE_HPP_
#define BE_BLT_FOR_NODE_HPP_

#include "node.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct ForNode : Node {
   std::unique_ptr<Node> init;
   std::unique_ptr<Node> condition;
   std::unique_ptr<Node> update;
   std::unique_ptr<Node> block;

   ForNode(std::unique_ptr<Node> init, std::unique_ptr<Node> condition, std::unique_ptr<Node> update, std::unique_ptr<Node> block)
      : init(std::move(init)), condition(std::move(condition)), update(std::move(condition)), block(std::move(block)) { }

   virtual void operator()(std::ostream& os) const override {
      if (init) {
         os << nl << "do " << indent;
         (*init)(os);
         write_loop(os);
         os << unindent << nl << "end ";
      } else {
         write_loop(os);
      }
   }

   void write_loop(std::ostream& os) const {
      os << nl << "while " << indent << indent;
      if (condition) {
         (*condition)(os);
      } else {
         os << "true";
      }
      os << " do " << unindent;
      write_block(os);
      os << unindent << nl << "end ";
   }

   void write_block(std::ostream& os) const {
      if (block) {
         (*block)(os);
      }
      os << nl << "::__continue__:: ";
      if (update) {
         (*update)(os);
      }
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
      debug_l(init, os, ctx.l_prefix, ctx.last_line_empty);
      debug_c("For", os, ctx.c_prefix, ctx.last_line_empty);
      debug_i(condition, os, ctx.r_prefix, ctx.last_line_empty);
      debug_i(block, os, ctx.r_prefix, ctx.last_line_empty);
      debug_r(update, os, ctx.r_prefix, ctx.last_line_empty);
   }

};

} // be::blt

#endif
