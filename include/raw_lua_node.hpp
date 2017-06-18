#pragma once
#ifndef BE_BLT_RAW_LUA_NODE_HPP_
#define BE_BLT_RAW_LUA_NODE_HPP_

#include "node.hpp"
#include "token_data.hpp"
#include <be/core/console_indent.hpp>

namespace be::blt {

struct LuaNode {
   const TokenData* token;

   void operator()(std::ostream& os) const {
      assert(token->type == TokenType::lua);

      os << nl;
      //os << nl << "do " << indent << nl;

      enum state {
         write, cr, skipws
      } s = write;

      for (auto c : token->text) {
         switch (c) {
            case '\r':
               s = cr;
               os << nl;
               break;
            case '\n':
               if (s != cr) {
                  os << nl;
               }
               s = skipws;
               break;
            case '\t':
            case '\v':
            case '\f':
            case ' ':
               if (s == cr) {
                  s = skipws;
               }
               if (s != skipws) {
                  os << c;
               }
               break;
            default:
               s = write;
               os << c;
               break;
         }
      }

      os << nl;
      //os << unindent << nl << "end ";
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
      debug_c("Lua " + limit_length(single_quote_escape(token->text), 30), os, ctx.c_prefix, ctx.last_line_empty);
   }

};

} // be::blt

#endif
