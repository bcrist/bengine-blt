#pragma once
#ifndef BE_BLT_UNARY_OP_NODE_HPP_
#define BE_BLT_UNARY_OP_NODE_HPP_

#include "node.hpp"
#include "token_type.hpp"

namespace be::blt {

struct UnaryOpNode : Node {
   TokenType op;
   std::unique_ptr<Node> expr;

   UnaryOpNode(TokenType op, std::unique_ptr<Node> expr) : op(op), expr(std::move(expr)) { }

   virtual void operator()(std::ostream& os) const override {
      os << to_lua(op) << ' ';
      (*expr)(os);
   }

   virtual bool is_literal() const override {
      return false;
   }

   virtual bool is_static_constant() const override {
      return expr->is_static_constant();
   }

   virtual bool is_nonnil_constant() const override {
      // all unary ops either:
      //  - are defined for nil operands and never output nil (not)
      //  - throw when an operand is nil (negation, complement, len)
      return expr->is_nonnil_constant() || expr->is_static_constant();
   }

   virtual bool is_nullipotent() const override {
      return expr->is_nullipotent();
   }

   virtual void debug(std::ostream& os, NodeDebugContext& ctx) const override {
      debug_cr("UnaryOp " + S(get_name(op)), expr, os, ctx);
   }

};

} // be::blt

#endif
