#pragma once
#ifndef BE_BLT_WHILE_NODE_HPP_
#define BE_BLT_WHILE_NODE_HPP_

#include "node.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct WhileNode : Node {
   std::unique_ptr<Node> expr;
   std::unique_ptr<Node> block;

   WhileNode(std::unique_ptr<Node> expr, std::unique_ptr<Node> block)
      : expr(std::move(expr)), block(std::move(block)) { }

   virtual void operator()(std::ostream& os) const override {
      os << nl << "while " << indent << indent;
      (*expr)(os);
      os << " do " << unindent;
      if (block) {
         (*block)(os);
      }
      os << nl << "::__continue__::" << unindent << nl << "end ";
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
      debug_cir("While", expr, block, os, ctx);
   }

};

} // be::blt

#endif
