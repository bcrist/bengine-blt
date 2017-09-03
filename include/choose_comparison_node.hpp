#pragma once
#ifndef BE_BLT_CHOOSE_COMPARISON_NODE_HPP_
#define BE_BLT_CHOOSE_COMPARISON_NODE_HPP_

#include "node.hpp"

namespace be::blt {

struct ChooseComparisonNode : Node {
   std::unique_ptr<Node> expr;

   ChooseComparisonNode(std::unique_ptr<Node> expr)
      : expr(std::move(expr)) { }

   virtual void operator()(std::ostream& os) const override {
      os << "__k__ == ( ";
      (*expr)(os);
      os << " )";
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
      debug_cr("ChooseComparison", expr, os, ctx);
   }

};

} // be::blt

#endif
