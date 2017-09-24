#pragma once
#ifndef BE_BLT_TOKEN_DATA_NODE_HPP_
#define BE_BLT_TOKEN_DATA_NODE_HPP_

#include "node.hpp"
#include "token_data.hpp"

namespace be::blt {

struct TokenDataNode : Node {
   const TokenData* token;

   TokenDataNode(const TokenData* token) : token(token) { }

   virtual void operator()(std::ostream& os) const override {
      os << token->text;
   }

   virtual bool is_literal() const override {
      switch (token->type) {
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
      return is_literal() && token->type != TokenType::kw_nil;
   }

   virtual bool is_nullipotent() const override {
      return is_literal() || token->type == TokenType::identifier;
   }

   virtual void debug(std::ostream& os, NodeDebugContext& ctx) const override {
      debug_c("TokenData " + S(get_name(token->type)) + " " + limit_length(single_quote_escape(token->text), 25), os, ctx.c_prefix, ctx.last_line_empty);
   }

};

} // be::blt

#endif
