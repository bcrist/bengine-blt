#pragma once
#ifndef BE_BLT_TOKEN_NODE_HPP_
#define BE_BLT_TOKEN_NODE_HPP_

#include "node.hpp"
#include "token_type.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct TokenNode : Node {
   TokenType token;

   TokenNode(TokenType token) : token(token) { }

   virtual void operator()(std::ostream& os) const override {
      os << to_lua(token);
   }

   virtual bool is_literal() const override {
      switch (token) {
         case TokenType::numeric_literal:
         case TokenType::string_literal:
         case TokenType::kw_nil:
         case TokenType::kw_false:
         case TokenType::kw_true:
            return true;
         default:
            return false;
      }
   }

   virtual bool is_static_constant() const override {
      return is_literal();
   }

   virtual bool is_nonnil_constant() const override {
      return is_literal() && token != TokenType::kw_nil;
   }

   virtual bool is_nullipotent() const override {
      return is_literal() || token == TokenType::identifier;
   }

   virtual void debug(std::ostream& os, NodeDebugContext& ctx) const override {
      debug_c("Token " + S(get_name(token)), os, ctx.c_prefix, ctx.last_line_empty);
   }

};

} // be::blt

#endif
