#pragma once
#ifndef BE_BLT_BINARY_OP_NODE_HPP_
#define BE_BLT_BINARY_OP_NODE_HPP_

#include "node.hpp"
#include "token_type.hpp"

namespace be::blt {

struct BinaryOpNode {
   Node left;
   TokenType op;
   Node right;

   void operator()(std::ostream& os) const {
      left(os);
      os << ' ' << to_lua(op) << ' ';
      right(os);
   }

   bool is_literal() const {
      return false;
   }

   bool is_static_constant() const {
      return left.is_static_constant() && right.is_static_constant();
   }

   bool is_nonnil_constant() const {
      if (op == TokenType::amp2 || op == TokenType::kw_and ||
          op == TokenType::bar2 || op == TokenType::kw_or) {
         return left.is_nonnil_constant() && right.is_nonnil_constant();
      } else {
         // all other binary ops either:
         //  - are defined for nil operands and never output nil (comparisons)
         //  - throw when an operand is nil (arithmetic, bitwise, concatenation)
         return (left.is_nonnil_constant() || left.is_static_constant()) &&
            (right.is_nonnil_constant() || right.is_static_constant());
      }
   }

   bool is_nullipotent() const {
      return left.is_nullipotent() && right.is_nullipotent();
   }

   void debug(std::ostream& os, NodeDebugContext& ctx) const {
      debug_lcr(left, "BinaryOp " + S(get_name(op)), right, os, ctx);
   }

};

} // be::blt

#endif
