#pragma once
#ifndef BE_BLT_NODE_HPP_
#define BE_BLT_NODE_HPP_

#include <be/core/console_indent.hpp>
#include <ostream>

namespace be::blt {

///////////////////////////////////////////////////////////////////////////////
struct NodeDebugContext {
   const S l_prefix, c_prefix, r_prefix;
   bool& last_line_empty;
};

///////////////////////////////////////////////////////////////////////////////
struct Node {
   virtual ~Node() = default;

   virtual bool is_literal() const = 0;
   virtual bool is_static_constant() const = 0;
   virtual bool is_nonnil_constant() const = 0;
   virtual bool is_nullipotent() const = 0;

   virtual void debug(std::ostream& os, NodeDebugContext& context) const = 0;
   virtual void operator()(std::ostream& os) const = 0;
};

///////////////////////////////////////////////////////////////////////////////
inline void debug_node(const std::unique_ptr<Node>& node, std::ostream& os, NodeDebugContext& ctx) {
   if (!node) {
      os << ctx.c_prefix << " nil" << nl;
      ctx.last_line_empty = false;
   } else {
      node->debug(os, ctx);
   }
}

///////////////////////////////////////////////////////////////////////////////
inline void debug_c(const S& str, std::ostream& os, const S& prefix, bool& e) {
   os << prefix << " " << str << nl;
   e = false;
}

///////////////////////////////////////////////////////////////////////////////
inline void debug_l(const std::unique_ptr<Node>& node, std::ostream& os, const S& prefix, bool& e) {
   NodeDebugContext ctx { prefix + "    ", prefix + " /--", prefix + " |  ", e };
   if (!e) {
      os << ctx.l_prefix << nl;
      e = true;
   }
   debug_node(node, os, ctx);
}

///////////////////////////////////////////////////////////////////////////////
inline void debug_l(const S& str, std::ostream& os, const S& prefix, bool& e) {
   if (!e) {
      os << prefix << "    " << nl;
   }
   os << prefix << " /-- " << str << nl;
   e = false;
}

///////////////////////////////////////////////////////////////////////////////
inline void debug_i(const std::unique_ptr<Node>& node, std::ostream& os, const S& prefix, bool& e) {
   NodeDebugContext ctx { prefix + " |  ", prefix + " +--", prefix + " |  ", e };
   debug_node(node, os, ctx);
}

///////////////////////////////////////////////////////////////////////////////
inline void debug_i(const S& str, std::ostream& os, const S& prefix, bool& e) {
   os << prefix << " +-- " << str << nl;
   e = false;
}

///////////////////////////////////////////////////////////////////////////////
inline void debug_r(const std::unique_ptr<Node>& node, std::ostream& os, const S& prefix, bool& e) {
   NodeDebugContext ctx { prefix + " |  ", prefix + " \\--", prefix + "    ", e };
   debug_node(node, os, ctx);
   if (!e) {
      os << ctx.r_prefix << nl;
      e = true;
   }
}

///////////////////////////////////////////////////////////////////////////////
inline void debug_r(const S& str, std::ostream& os, const S& prefix, bool& e) {
   os << prefix << " \\-- " << str << nl;
   os << prefix << "    " << nl;
   e = true;
}

///////////////////////////////////////////////////////////////////////////////
template <typename L, typename C, typename R>
void debug_lcr(const L& ln, const C& cn, const R& rn, std::ostream& os, const NodeDebugContext& ctx) {
   debug_l(ln, os, ctx.l_prefix, ctx.last_line_empty);
   debug_c(cn, os, ctx.c_prefix, ctx.last_line_empty);
   debug_r(rn, os, ctx.r_prefix, ctx.last_line_empty);
}

///////////////////////////////////////////////////////////////////////////////
template <typename L, typename I, typename C, typename R>
void debug_licr(const L& ln, const I& in, const C& cn, const R& rn, std::ostream& os, const NodeDebugContext& ctx) {
   debug_l(ln, os, ctx.l_prefix, ctx.last_line_empty);
   debug_i(in, os, ctx.l_prefix, ctx.last_line_empty);
   debug_c(cn, os, ctx.c_prefix, ctx.last_line_empty);
   debug_r(rn, os, ctx.r_prefix, ctx.last_line_empty);
}

///////////////////////////////////////////////////////////////////////////////
template <typename L, typename C, typename I, typename R>
void debug_lcir(const L& ln, const C& cn, const I& in, const R& rn, std::ostream& os, const NodeDebugContext& ctx) {
   debug_l(ln, os, ctx.l_prefix, ctx.last_line_empty);
   debug_c(cn, os, ctx.c_prefix, ctx.last_line_empty);
   debug_i(in, os, ctx.r_prefix, ctx.last_line_empty);
   debug_r(rn, os, ctx.r_prefix, ctx.last_line_empty);
}

///////////////////////////////////////////////////////////////////////////////
template <typename L, typename C>
void debug_lc(const L& ln, const C& cn, std::ostream& os, const NodeDebugContext& ctx) {
   debug_l(ln, os, ctx.l_prefix, ctx.last_line_empty);
   debug_c(cn, os, ctx.c_prefix, ctx.last_line_empty);
}

///////////////////////////////////////////////////////////////////////////////
template <typename C, typename R>
void debug_cr(const C& cn, const R& rn, std::ostream& os, const NodeDebugContext& ctx) {
   debug_c(cn, os, ctx.c_prefix, ctx.last_line_empty);
   debug_r(rn, os, ctx.r_prefix, ctx.last_line_empty);
}

///////////////////////////////////////////////////////////////////////////////
template <typename C, typename I, typename R>
void debug_cir(const C& cn, const I& in, const R& rn, std::ostream& os, const NodeDebugContext& ctx) {
   debug_c(cn, os, ctx.c_prefix, ctx.last_line_empty);
   debug_i(in, os, ctx.r_prefix, ctx.last_line_empty);
   debug_r(rn, os, ctx.r_prefix, ctx.last_line_empty);
}

} // be::blt

#endif
