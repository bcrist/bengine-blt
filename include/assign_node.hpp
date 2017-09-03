#pragma once
#ifndef BE_blt_ASSIGN_NODE_HPP_
#define BE_blt_ASSIGN_NODE_HPP_

#include "node.hpp"

namespace be::blt {

struct AssignNode : Node {
   std::unique_ptr<Node> ids;
   std::unique_ptr<Node> exprs;
   bool local;

   AssignNode(std::unique_ptr<Node> ids, std::unique_ptr<Node> exprs, bool local)
      : ids(std::move(ids)), exprs(std::move(exprs)), local(local) { }

   virtual void operator()(std::ostream& os) const override {
      os << nl;
      if (local) {
         os << "local ";
      }
      (*ids)(os);
      os << " = ";
      (*exprs)(os);
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
      debug_lcr(ids, (local ? "Assign (local)" : "Assign"), exprs, os, ctx);
   }

};

} // be::blt

#endif
