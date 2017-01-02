#pragma once
#ifndef BE_BLT_TOKEN_NODE_HPP_
#define BE_BLT_TOKEN_NODE_HPP_

#include "node.hpp"
#include "token_type.hpp"
#include <be/core/console_indent.hpp>

namespace be {
namespace blt {

struct TokenNode {
   TokenType token;

   void operator()(std::ostream& os) const {
      os << to_lua(token);
   }

   bool is_literal() const {
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

   bool is_static_constant() const {
      return is_literal();
   }

   bool is_nonnil_constant() const {
      return is_literal() && token != TokenType::kw_nil;
   }

   bool is_nullipotent() const {
      return is_literal() || token == TokenType::identifier;
   }

   void debug(std::ostream& os, NodeDebugContext& ctx) const {
      debug_c("Token " + S(get_name(token)), os, ctx.c_prefix, ctx.last_line_empty);
   }

};

} // be::blt
} // be

#endif
