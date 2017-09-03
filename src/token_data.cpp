#include "pch.hpp"
#include "token_data.hpp"
#include "codegen.hpp"

namespace be::blt {

///////////////////////////////////////////////////////////////////////////////
S pretty_token(const TokenData& token) {
   S val;
   val.reserve(32);

   switch (token.type) {
      case TokenType::identifier:
      case TokenType::invalid_character:
         val = get_name(token.type);
         val.reserve(val.length() + 20);
         val.append(1, ' ');
         val.append(single_quote_escape(token.text));
         break;

      case TokenType::document:
      case TokenType::lua:
      {
         val = get_name(token.type);
         val.reserve(val.length() + 20);
         val.append(1, ' ');
         S data = limit_length(token.text);
         val.append(single_quote_escape(data));
         break;
      }

      default:
         val = get_name(token.type);
         break;
   }

   val.reserve(val.length() + 12);
   val.append(" @ ");
   val.append(std::to_string(token.location.line));
   val.append(" : ");
   val.append(std::to_string(token.location.column));

   return val;
}

} // be::blt
