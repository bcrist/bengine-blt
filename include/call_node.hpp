#pragma once
#ifndef BE_BLT_CALL_NODE_HPP_
#define BE_BLT_CALL_NODE_HPP_

#include "token_data_node.hpp"
#include <be/core/console_indent.hpp>
#include <be/util/string_span.hpp>

namespace be {
namespace blt {

struct CallNode {
   Node ref;
   Node expr_list;
   gsl::cstring_span<> method;

   void operator()(std::ostream& os) const {
      ref(os);
      if (!method.empty()) {
         os << ":" << method;
      }
      os << '(' << indent;
      if (!is_empty(expr_list)) {
         expr_list(os);
      }
      os << ')' << unindent;
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
      debug_l(ref, os, ctx.l_prefix, ctx.last_line_empty);

      if (!method.empty()) {
         debug_i(S(":") + S(method.begin(), method.end()), os, ctx.l_prefix, ctx.last_line_empty);
      }

      debug_c("Call", os, ctx.c_prefix, ctx.last_line_empty);
      debug_r(expr_list, os, ctx.r_prefix, ctx.last_line_empty);
   }

};

} // be::blt
} // be

#endif
