#pragma once
#ifndef BE_BLT_BINARY_OP_NODE_HPP_
#define BE_BLT_BINARY_OP_NODE_HPP_

#include "node.hpp"
#include "token_type.hpp"

namespace be::blt {

struct BinaryOpNode : Node {
   std::unique_ptr<Node> left;
   TokenType op;
   std::unique_ptr<Node> right;

   BinaryOpNode(std::unique_ptr<Node> left, TokenType op, std::unique_ptr<Node> right)
      : left(std::move(left)), op(op), right(std::move(right)) { }

   virtual void operator()(std::ostream& os) const override {
      (*left)(os);
      os << ' ' << to_lua(op) << ' ';
      (*right)(os);
   }

   virtual bool is_literal() const override {
      return false;
   }

   virtual bool is_static_constant() const override {
      return left->is_static_constant() && right->is_static_constant();
   }

   virtual bool is_nonnil_constant() const override {
      if (op == TokenType::amp2 || op == TokenType::kw_and ||
          op == TokenType::bar2 || op == TokenType::kw_or) {
         return left->is_nonnil_constant() && right->is_nonnil_constant();
      } else {
         // all other binary ops either:
         //  - are defined for nil operands and never output nil (comparisons)
         //  - throw when an operand is nil (arithmetic, bitwise, concatenation)
         return (left->is_nonnil_constant() || left->is_static_constant()) &&
            (right->is_nonnil_constant() || right->is_static_constant());
      }
   }

   virtual bool is_nullipotent() const override {
      return left->is_nullipotent() && right->is_nullipotent();
   }

   virtual void debug(std::ostream& os, NodeDebugContext& ctx) const override {
      debug_lcr(left, "BinaryOp " + S(get_name(op)), right, os, ctx);
   }

};

} // be::blt

#endif
