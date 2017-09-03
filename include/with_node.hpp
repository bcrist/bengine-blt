#pragma once
#ifndef BE_BLT_WITH_NODE_HPP_
#define BE_BLT_WITH_NODE_HPP_

#include "node.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct WithNode : Node{
   std::unique_ptr<Node> expr;
   std::unique_ptr<Node> block;

   WithNode(std::unique_ptr<Node> expr, std::unique_ptr<Node> block)
      : expr(std::move(expr)), block(std::move(block)) { }

   virtual void operator()(std::ostream& os) const override {
      os << nl << "__ctx__:push( " << indent;
      (*expr)(os);
      os << unindent << " ) ";
      if (block) {
         (*block)(os);
      }
      os << nl << "__ctx__:pop() ";
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
      debug_lcr(expr, "With", block, os, ctx);
   }

};

} // be::blt

#endif
