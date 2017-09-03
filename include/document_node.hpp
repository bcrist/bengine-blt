#pragma once
#ifndef BE_BLT_DOCUMENT_NODE_HPP_
#define BE_BLT_DOCUMENT_NODE_HPP_

#include "node.hpp"
#include "token_data.hpp"
#include "codegen.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct DocumentNode : Node {
   const TokenData* token;

   DocumentNode(const TokenData* t) : token(t) { }

   virtual void operator()(std::ostream& os) const override {
      assert(token->type == TokenType::document);
      os << nl << "__ctx__:write " << indent;

      auto idata = get_indent(os);
      I32 indent = idata.first * (idata.second ? (I32)strlen(idata.second) : 0);

      I32 wrap = std::max(20, 74 - indent);
      I32 initial_wrap = wrap - 3;

      single_quote_escape(token->text, os, wrap, initial_wrap);
      os << unindent;
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
      debug_c("Document " + limit_length(single_quote_escape(token->text), 30), os, ctx.c_prefix, ctx.last_line_empty);
   }

};

} // be::blt

#endif
