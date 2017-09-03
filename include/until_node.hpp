#pragma once
#ifndef BE_BLT_UNTIL_NODE_HPP_
#define BE_BLT_UNTIL_NODE_HPP_

#include "node.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct UntilNode : Node {
   std::unique_ptr<Node> block;
   std::unique_ptr<Node> stop_condition;

   UntilNode(std::unique_ptr<Node> block, std::unique_ptr<Node> stop_condition)
      : block(std::move(block)), stop_condition(std::move(stop_condition)) { }

   virtual void operator()(std::ostream& os) const override {
      os << nl << "repeat " << indent;
      if (block) {
         (*block)(os);
      }
      os << unindent << nl << "until " << indent;
      (*stop_condition)(os);
      os << unindent;
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
      debug_cir("Until ", block, stop_condition, os, ctx);
   }

};

} // be::blt

#endif
