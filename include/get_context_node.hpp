#pragma once
#ifndef BE_BLT_GET_CONTEXT_NODE_HPP_
#define BE_BLT_GET_CONTEXT_NODE_HPP_

#include "node.hpp"
#include "token_data.hpp"

namespace be::blt {

struct GetContextNode : Node {
   U32 level;

   GetContextNode(U32 level) : level(level) { }

   virtual void operator()(std::ostream& os) const override {
      if (level == 0) {
         os << "__ctx__:get()";
      } else {
         os << "__ctx__:get(" << level << ")";
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
      debug_c("GetContext (" + std::to_string(level) + ")", os, ctx.c_prefix, ctx.last_line_empty);
   }

};

} // be::blt

#endif
