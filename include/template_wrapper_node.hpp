#pragma once
#ifndef BE_BLT_TEMPLATE_WRAPPER_NODE_HPP_
#define BE_BLT_TEMPLATE_WRAPPER_NODE_HPP_

#include "sequence_node.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct TemplateWrapperNode : SequenceNode {

   virtual void operator()(std::ostream& os) const override {
      os << "local __was_strict__ = __STRICT"
         << nl << "__STRICT = false"
         << nl << "local __blt__ = require('be.blt')"
         << nl << "local __ctx__ = __blt__.create_ctx_(...)"
         << nl << "local pgsub = __blt__.pgsub or pgsub"
         << nl << "local explode = __blt__.explode or explode"
         << nl << "local pad = __blt__.pad or pad"
         << nl << "local rpad = __blt__.rpad or rpad"
         << nl << "local lpad = __blt__.lpad or lpad"
         << nl << "local nl = __blt__.nl or nl"
         << nl;
      SequenceNode::operator()(os);
      os << nl
         << nl << "__STRICT = __was_strict__"
         << nl << "return __ctx__:coalesce()"
         << nl;
   }

protected:
   virtual const char* name_() const override {
      return "Template";
   }

};

} // be::blt

#endif
