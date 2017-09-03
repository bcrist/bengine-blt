#pragma once
#ifndef BE_BLT_BLOCK_NODE_HPP_
#define BE_BLT_BLOCK_NODE_HPP_

#include "sequence_node.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct BlockNode : SequenceNode {

   virtual void operator()(std::ostream& os) const override {
      os << nl << "do " << indent;
      SequenceNode::operator()(os);
      os << unindent << nl << "end ";
   }

protected:
   virtual const char* name_() const override {
      return "Block";
   }

};

} // be::blt

#endif
