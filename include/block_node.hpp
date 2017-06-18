#pragma once
#ifndef BE_BLT_BLOCK_NODE_HPP_
#define BE_BLT_BLOCK_NODE_HPP_

#include "sequence_node.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct BlockNode {
   SequenceNode contents;

   void operator()(std::ostream& os) const {
      os << nl << "do " << indent;
      contents(os);
      os << unindent << nl << "end ";
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
      debug_cr("Block", contents, os, ctx);
   }

};

} // be::blt

#endif
