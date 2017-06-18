#pragma once
#ifndef BE_BLT_REF_NODE_HPP_
#define BE_BLT_REF_NODE_HPP_

#include "node.hpp"
#include <be/util/string_span.hpp>

namespace be::blt {

struct RefNode {
   gsl::cstring_span<> id;
   Node parent;

   void operator()(std::ostream& os) const {
      if (is_empty(parent)) {
         os << "(__ctx__:ref('" << id << "', " << id << "))";
      } else {
         parent(os);
         os << '.' << id;
      }
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
      return true;
   }

   void debug(std::ostream& os, NodeDebugContext& ctx) const {
      if (is_empty(parent)) {
         debug_c("Ref " + single_quote_escape(id), os, ctx.c_prefix, ctx.last_line_empty);
      } else {
         debug_lc(parent, "Ref " + single_quote_escape(id), os, ctx);
      }
   }

};

} // be::blt

#endif
