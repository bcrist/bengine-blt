#pragma once
#ifndef BE_BLT_ID_LIST_NODE_HPP_
#define BE_BLT_ID_LIST_NODE_HPP_

#include "node.hpp"
#include <be/util/string_span.hpp>
#include <vector>

namespace be::blt {

struct IdListNode {
   std::vector<gsl::cstring_span<>> ids;

   void operator()(std::ostream& os) const {
      bool first = true;
      for (auto id : ids) {
         if (first) {
            first = false;
         } else {
            os << ", ";
         }
         os << id;
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
      debug_c("IdList", os, ctx.c_prefix, ctx.last_line_empty);

      for (auto it = ids.begin(), end = ids.end(); it != end; ++it) {
         if (it + 1 != end) {
            debug_i(S(it->begin(), it->end()), os, ctx.r_prefix, ctx.last_line_empty);
         } else {
            debug_r(S(it->begin(), it->end()), os, ctx.r_prefix, ctx.last_line_empty);
         }
      }
   }

};

} // be::blt

#endif
