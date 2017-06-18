#pragma once
#ifndef BE_BLT_CHOOSE_COMPARISON_NODE_HPP_
#define BE_BLT_CHOOSE_COMPARISON_NODE_HPP_

#include "node.hpp"

namespace be::blt {

struct ChooseComparisonNode {
   Node expr;

   void operator()(std::ostream& os) const {
      os << "__k__ == ( ";
      expr(os);
      os << " )";
   }

   bool is_literal() const {
      return false;
   }

   bool is_static_constant() const {
      return false;
   }

   bool is_nonnil_constant() const {
      return false;
   }

   bool is_nullipotent() const {
      return false;
   }

   void debug(std::ostream& os, NodeDebugContext& ctx) const {
      debug_cr("ChooseComparison", expr, os, ctx);
   }

};

} // be::blt

#endif
