#pragma once
#ifndef BE_BLT_WITH_EACH_USING_NODE_HPP_
#define BE_BLT_WITH_EACH_USING_NODE_HPP_

#include "node.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct WithEachUsingNode : Node {
   std::unique_ptr<Node> expr;
   std::unique_ptr<Node> using_expr;
   std::unique_ptr<Node> block;

   WithEachUsingNode(std::unique_ptr<Node> expr, std::unique_ptr<Node> using_expr, std::unique_ptr<Node> block)
      : expr(std::move(expr)), using_expr(std::move(using_expr)), block(std::move(block)) { }

   virtual void operator()(std::ostream& os) const override {
      os << nl << "__ctx__:push( " << indent;
      (*expr)(os);
      os << unindent << " )";
      os << nl << "for __k__ in ( " << indent << indent;
      (*using_expr)(os);
      os << unindent << " )(__ctx__:get()) do";
      os << nl << "__ctx__:push(__ctx__:ref(__k__)) ";
      if (block) {
         (*block)(os);
      }
      os << nl << "__ctx__:pop()";
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
      debug_licr(expr, using_expr, "WithEachUsing", block, os, ctx);
   }

};

} // be::blt

#endif
