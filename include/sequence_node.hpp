#pragma once
#ifndef BE_BLT_SEQUENCE_NODE_HPP_
#define BE_BLT_SEQUENCE_NODE_HPP_

#include "node.hpp"
#include <vector>

namespace be::blt {

struct SequenceNode {
   std::vector<Node> seq;

   void operator()(std::ostream& os) const {
      for (auto& node : seq) {
         node(os);
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
      return false;
   }

   void debug(std::ostream& os, NodeDebugContext& ctx) const {
      debug_c("Sequence", os, ctx.c_prefix, ctx.last_line_empty);

      for (auto it = seq.begin(), end = seq.end(); it != end; ++it) {
         if (it + 1 != end) {
            debug_i(*it, os, ctx.r_prefix, ctx.last_line_empty);
         } else {
            debug_r(*it, os, ctx.r_prefix, ctx.last_line_empty);
         }
      }
   }

};

} // be::blt

#endif
