#pragma once
#ifndef BE_BLT_REF_NODE_HPP_
#define BE_BLT_REF_NODE_HPP_

#include "node.hpp"
#include <be/util/string_span.hpp>

namespace be::blt {

struct RefNode : Node {
   gsl::cstring_span<> id;
   std::unique_ptr<Node> parent;

   RefNode(gsl::cstring_span<> id)
      : id(id) { }

   RefNode(gsl::cstring_span<> id, std::unique_ptr<Node> parent)
      : id(id), parent(std::move(parent)) { }

   virtual void operator()(std::ostream& os) const override {
      if (!parent) {
         os << "(__ctx__:ref('" << id << "', " << id << "))";
      } else {
         (*parent)(os);
         os << '.' << id;
      }
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
      return true;
   }

   virtual void debug(std::ostream& os, NodeDebugContext& ctx) const override {
      if (!parent) {
         debug_c("Ref " + single_quote_escape(id), os, ctx.c_prefix, ctx.last_line_empty);
      } else {
         debug_lc(parent, "Ref " + single_quote_escape(id), os, ctx);
      }
   }

};

} // be::blt

#endif
