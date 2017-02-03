#pragma once
#ifndef BE_BLT_TEMPLATE_WRAPPER_NODE_HPP_
#define BE_BLT_TEMPLATE_WRAPPER_NODE_HPP_

#include "sequence_node.hpp"
#include <be/core/console_indent.hpp>

namespace be {
namespace blt {

struct TemplateWrapperNode : SequenceNode {

   void operator()(std::ostream& os) const {
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
      debug_c("Template", os, ctx.c_prefix, ctx.last_line_empty);

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
} // be

#endif
