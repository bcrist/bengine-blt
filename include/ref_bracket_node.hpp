#pragma once
#ifndef BE_BLT_REF_BRACKET_NODE_HPP_
#define BE_BLT_REF_BRACKET_NODE_HPP_

#include "node.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct RefBracketNode : Node {
   std::unique_ptr<Node> parent;
   std::unique_ptr<Node> expr;

   RefBracketNode(std::unique_ptr<Node> parent, std::unique_ptr<Node> expr)
      : parent(std::move(parent)), expr(std::move(expr)) { }

   virtual void operator()(std::ostream& os) const override {
      (*parent)(os);
      os << '[' << indent;
      (*expr)(os);
      os << ']' << unindent;
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
      return true;
   }

   virtual void debug(std::ostream& os, NodeDebugContext& ctx) const override {
      debug_lcr(parent, "RefBracket", expr, os, ctx);
   }

};

} // be::blt

#endif
