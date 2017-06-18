#pragma once
#ifndef BE_BLT_TOKEN_DATA_NODE_HPP_
#define BE_BLT_TOKEN_DATA_NODE_HPP_

#include "node.hpp"
#include "token_data.hpp"
#include <be/util/string_span.hpp>

namespace be::blt {

struct TokenDataNode {
   const TokenData* token;

   void operator()(std::ostream& os) const {
      os << token->text;
   }

   bool is_literal() const {
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

   bool is_static_constant() const {
      return is_literal();
   }

   bool is_nonnil_constant() const {
      return is_literal() && token->type != TokenType::kw_nil;
   }

   bool is_nullipotent() const {
      return is_literal() || token->type == TokenType::identifier;
   }

   void debug(std::ostream& os, NodeDebugContext& ctx) const {
      debug_c("TokenData " + S(get_name(token->type)) + " " + limit_length(single_quote_escape(token->text), 25), os, ctx.c_prefix, ctx.last_line_empty);
   }

};

} // be::blt

#endif
