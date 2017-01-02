#pragma once
#ifndef BE_BLT_GET_CONTEXT_NODE_HPP_
#define BE_BLT_GET_CONTEXT_NODE_HPP_

#include "node.hpp"
#include "token_data.hpp"

namespace be {
namespace blt {

struct GetContextNode {
   U32 level;

   void operator()(std::ostream& os) const {
      if (level == 0) {
         os << "__ctx__:get()";
      } else {
         os << "__ctx__:get(" << level << ")";
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
      debug_c("GetContext (" + std::to_string(level) + ")", os, ctx.c_prefix, ctx.last_line_empty);
   }

};

} // be::blt
} // be

#endif
