#pragma once
#ifndef BE_BLT_SIMPLE_SEQUENCE_NODE_HPP_
#define BE_BLT_SIMPLE_SEQUENCE_NODE_HPP_

#include "sequence_node.hpp"

namespace be::blt {

struct SimpleSequenceNode : SequenceNode {
protected:
   virtual const char* name_() const override {
      return "Sequence";
   }
};

} // be::blt

#endif
