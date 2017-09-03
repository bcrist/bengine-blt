#pragma once
#ifndef BE_BLT_WITH_EACH_NODE_HPP_
#define BE_BLT_WITH_EACH_NODE_HPP_

#include "node.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct WithEachNode : Node {
   std::unique_ptr<Node> expr;
   std::unique_ptr<Node> block;

   WithEachNode(std::unique_ptr<Node> expr, std::unique_ptr<Node> block)
      : expr(std::move(expr)), block(std::move(block)) { }

   virtual void operator()(std::ostream& os) const override {
      os << nl << "__ctx__:push( " << indent;
      (*expr)(os);
      os << unindent << " )";
      os << nl << "if __blt__.is_sequence(__ctx__:get()) then" << indent;
      os << nl << "for __k__ = 1, #(__ctx__:get()) do" << indent;
      os << nl << "__ctx__:push(__ctx__:ref(__k__)) ";
      if (block) {
         (*block)(os);
      }
      os << nl << "__ctx__:pop()";
      os << unindent << nl << "end";
      os << unindent << nl << "else" << indent;
      os << nl << "for __k__ in pairs(__ctx__:get()) do" << indent;
      os << nl << "__ctx__:push(__ctx__:ref(__k__)) ";
      if (block) {
         (*block)(os);
      }
      os << nl << "__ctx__:pop()";
      os << unindent << nl << "end";
      os << unindent << nl << "end";
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
      debug_lcr(expr, "WithEach", block, os, ctx);
   }

};

} // be::blt

#endif
