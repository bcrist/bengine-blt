#include "pch.hpp"
#include "token_type.hpp"
#include <stdexcept>

namespace be {
namespace blt {

///////////////////////////////////////////////////////////////////////////////
const char* get_name(TokenType type) {
   using T = TokenType;
   switch (type) {
      case T::document:          return "document";
      case T::lua:               return "lua";
      case T::numeric_literal:   return "numeric_literal";
      case T::string_literal:    return "string_literal";
      case T::kw_if:             return "'if'";
      case T::kw_else:           return "'else'";
      case T::kw_while:          return "'while'";
      case T::kw_for:            return "'for'";
      case T::kw_with:           return "'with'";
      case T::kw_each:           return "'each'";
      case T::kw_using:          return "'using'";
      case T::kw_choose:         return "'choose'";
      case T::kw_break:          return "'break'";
      case T::kw_continue:       return "'continue'";
      case T::kw_until:          return "'until'";
      case T::kw_and:            return "'and'";
      case T::kw_or:             return "'or'";
      case T::kw_not:            return "'not'";
      case T::kw_nil:            return "'nil'";
      case T::kw_false:          return "'false'";
      case T::kw_true:           return "'true'";
      case T::identifier:        return "identifier";
      case T::brace_opener:      return "'{'";
      case T::brace_closer:      return "'}'";
      case T::paren_opener:      return "'('";
      case T::paren_closer:      return "')'";
      case T::bracket_opener:    return "'['";
      case T::bracket_closer:    return "']'";
      case T::semicolon:         return "';'";
      case T::dot:               return "'.'";
      case T::dot2:              return "'..'";
      case T::comma:             return "','";
      case T::question:          return "'?'";
      case T::colon:             return "':'";
      case T::plus:              return "'+'";
      case T::star:              return "'*'";
      case T::slash2:            return "'//'";
      case T::percent:           return "'%'";
      case T::chevron:           return "'^'";
      case T::right2:            return "'>>'";
      case T::left2:             return "'<<'";
      case T::eq2:               return "'=='";
      case T::tilde_eq:          return "'~='";
      case T::left_eq:           return "'>='";
      case T::right_eq:          return "'<='";
      case T::amp2:              return "'&&'";
      case T::bar2:              return "'||'";
      case T::pound:             return "'#'";
      case T::dollars:           return "'$'";
      case T::eq:                return "'='";
      case T::minus:             return "'-'";
      case T::slash:             return "'/'";
      case T::amp:               return "'&'";
      case T::bar:               return "'|'";
      case T::tilde:             return "'~'";
      case T::left:              return "'<'";
      case T::right:             return "'>'";
      case T::invalid_character: return "invalid_character";
      default:
         return "<unknown>";
   }
}

///////////////////////////////////////////////////////////////////////////////
const char* to_lua(TokenType type) {
   using T = TokenType;
   switch (type) {
      case T::kw_if:             return "if";
      case T::kw_else:           return "else";
      case T::kw_while:          return "while";
      case T::kw_for:            return "for";
      case T::kw_break:          return "break";
      case T::kw_continue:       return "goto continue_";
      case T::kw_until:          return "until";
      case T::kw_and:            return "and";
      case T::kw_or:             return "or";
      case T::kw_not:            return "not";
      case T::kw_nil:            return "nil";
      case T::kw_false:          return "false";
      case T::kw_true:           return "true";
      case T::brace_opener:      return "{";
      case T::brace_closer:      return "}";
      case T::paren_opener:      return "(";
      case T::paren_closer:      return ")";
      case T::bracket_opener:    return "[";
      case T::bracket_closer:    return "]";
      case T::semicolon:         return ";";
      case T::dot:               return ".";
      case T::dot2:              return "..";
      case T::comma:             return ",";
      case T::colon:             return ":";
      case T::plus:              return "+";
      case T::star:              return "*";
      case T::slash2:            return "//";
      case T::percent:           return "%";
      case T::chevron:           return "^";
      case T::right2:            return ">>";
      case T::left2:             return "<<";
      case T::eq2:               return "==";
      case T::tilde_eq:          return "~=";
      case T::left_eq:           return ">=";
      case T::right_eq:          return "<=";
      case T::amp2:              return "and";
      case T::bar2:              return "or";
      case T::pound:             return "#";
      case T::eq:                return "=";
      case T::minus:             return "-";
      case T::slash:             return "/";
      case T::amp:               return "&";
      case T::bar:               return "|";
      case T::tilde:             return "~";
      case T::left:              return "<";
      case T::right:             return ">";
      default:
         return "";
   }
}

} // be::blt
} // be
