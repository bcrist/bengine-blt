#pragma once
#ifndef BE_BLT_TERNARY_NODE_HPP_
#define BE_BLT_TERNARY_NODE_HPP_

#include "node.hpp"
#include "token_type.hpp"

namespace be::blt {

struct TernaryNode : Node {
   std::unique_ptr<Node> condition;
   std::unique_ptr<Node> true_expr;
   std::unique_ptr<Node> false_expr;

   TernaryNode(std::unique_ptr<Node> condition, std::unique_ptr<Node> true_expr, std::unique_ptr<Node> false_expr)
      : condition(std::move(condition)), true_expr(std::move(true_expr)), false_expr(std::move(false_expr)) { }

   virtual void operator()(std::ostream& os) const override {
      if (true_expr->is_nullipotent() && false_expr->is_nullipotent()) {
         // use functional-if
         os << "__blt__.fif_(" << indent;
         (*condition)(os);
         os << ", ";
         (*true_expr)(os);
         os << ", ";
         (*false_expr)(os);
         os << ")" << unindent;
      } else {
         // use anonymous closure
         os << "(function() if " << indent;
         (*condition)(os);
         os << " then return ";
         (*true_expr)(os);
         os << " else return ";
         (*false_expr)(os);
         os << " end end)()" << unindent;
      }
   }

   virtual bool is_literal() const override {
      return false;
   }

   virtual bool is_static_constant() const override {
      return condition->is_static_constant() && true_expr->is_static_constant() && false_expr->is_static_constant();
   }

   virtual bool is_nonnil_constant() const override {
      return (condition->is_nonnil_constant() || condition->is_static_constant()) && true_expr->is_nonnil_constant() && false_expr->is_nonnil_constant();
   }

   virtual bool is_nullipotent() const override {
      return condition->is_nullipotent() && true_expr->is_nullipotent() && false_expr->is_nullipotent();
   }

   virtual void debug(std::ostream& os, NodeDebugContext& ctx) const override {
      debug_lcir(condition, "Ternary", true_expr, false_expr, os, ctx);
   }

};

} // be::blt

#endif
