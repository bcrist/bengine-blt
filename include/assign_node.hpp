#pragma once
#ifndef BE_blt_ASSIGN_NODE_HPP_
#define BE_blt_ASSIGN_NODE_HPP_

#include "node.hpp"

namespace be::blt {

struct AssignNode {
   Node ids;
   Node exprs;
   bool local;

   void operator()(std::ostream& os) const {
      os << nl;
      if (local) {
         os << "local ";
      }
      ids(os);
      os << " = ";
      exprs(os);
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
      debug_lcr(ids, (local ? "Assign (local)" : "Assign"), exprs, os, ctx);
   }

};

} // be::blt

#endif
