#pragma once
#ifndef BE_BLT_TERNARY_NODE_HPP_
#define BE_BLT_TERNARY_NODE_HPP_

#include "node.hpp"
#include "token_type.hpp"

namespace be::blt {

struct TernaryNode {
   Node condition;
   Node true_expr;
   Node false_expr;

   void operator()(std::ostream& os) const {
      if (true_expr.is_nullipotent() && false_expr.is_nullipotent()) {
         // use functional-if
         os << "__blt__.fif_(" << indent;
         condition(os);
         os << ", ";
         true_expr(os);
         os << ", ";
         false_expr(os);
         os << ")" << unindent;
      } else {
         // use anonymous closure
         os << "(function() if " << indent;
         condition(os);
         os << " then return ";
         true_expr(os);
         os << " else return ";
         false_expr(os);
         os << " end end)()" << unindent;
      }
   }

   bool is_literal() const {
      return false;
   }

   bool is_static_constant() const {
      return condition.is_static_constant() && true_expr.is_static_constant() && false_expr.is_static_constant();
   }

   bool is_nonnil_constant() const {
      return (condition.is_nonnil_constant() || condition.is_static_constant()) && true_expr.is_nonnil_constant() && false_expr.is_nonnil_constant();
   }

   bool is_nullipotent() const {
      return condition.is_nullipotent() && true_expr.is_nullipotent() && false_expr.is_nullipotent();
   }

   void debug(std::ostream& os, NodeDebugContext& ctx) const {
      debug_lcir(condition, "Ternary", true_expr, false_expr, os, ctx);
   }

};

} // be::blt

#endif
