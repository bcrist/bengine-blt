#pragma once
#ifndef BE_BLT_UNARY_OP_NODE_HPP_
#define BE_BLT_UNARY_OP_NODE_HPP_

#include "node.hpp"
#include "token_type.hpp"

namespace be::blt {

struct UnaryOpNode {
   TokenType op;
   Node expr;
   
   void operator()(std::ostream& os) const {
      os << to_lua(op) << ' ';
      expr(os);
   }

   bool is_literal() const {
      return false;
   }

   bool is_static_constant() const {
      return expr.is_static_constant();
   }

   bool is_nonnil_constant() const {
      // all unary ops either:
      //  - are defined for nil operands and never output nil (not)
      //  - throw when an operand is nil (negation, complement, len)
      return expr.is_nonnil_constant() || expr.is_static_constant();
   }

   bool is_nullipotent() const {
      return expr.is_nullipotent();
   }

   void debug(std::ostream& os, NodeDebugContext& ctx) const {
      debug_cr("UnaryOp " + S(get_name(op)), expr, os, ctx);
   }

};

} // be::blt

#endif
