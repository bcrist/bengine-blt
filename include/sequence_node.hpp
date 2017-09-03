#pragma once
#ifndef BE_BLT_SEQUENCE_NODE_HPP_
#define BE_BLT_SEQUENCE_NODE_HPP_

#include "node.hpp"
#include <vector>

namespace be::blt {

struct SequenceNode : Node {
   std::vector<std::unique_ptr<Node>> seq;

   virtual void operator()(std::ostream& os) const override {
      for (auto& node : seq) {
         (*node)(os);
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
      return false;
   }

   virtual void debug(std::ostream& os, NodeDebugContext& ctx) const override {
      debug_c(name_(), os, ctx.c_prefix, ctx.last_line_empty);

      for (auto it = seq.begin(), end = seq.end(); it != end; ++it) {
         if (it + 1 != end) {
            debug_i(*it, os, ctx.r_prefix, ctx.last_line_empty);
         } else {
            debug_r(*it, os, ctx.r_prefix, ctx.last_line_empty);
         }
      }
   }

protected:
   SequenceNode() = default;
   ~SequenceNode() = default;

   virtual const char* name_() const = 0;
};

} // be::blt

#endif
