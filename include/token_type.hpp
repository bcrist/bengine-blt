#pragma once
#ifndef BE_BLT_TOKEN_TYPE_HPP_
#define BE_BLT_TOKEN_TYPE_HPP_

namespace be::blt {

///////////////////////////////////////////////////////////////////////////////
enum class TokenType {
   document,
   lua,

   numeric_literal,
   string_literal,

   kw_if,
   kw_else,
   kw_while,
   kw_for,
   kw_with,
   kw_each,
   kw_using,
   kw_choose,
   kw_break,
   kw_continue,
   kw_until,
   kw_and,
   kw_or,
   kw_not,
   kw_nil,
   kw_false,
   kw_true,

   identifier,

   brace_opener,
   brace_closer,

   paren_opener,
   paren_closer,

   bracket_opener,
   bracket_closer,

   semicolon,

   dot,
   dot2,
   comma,
   question,
   colon,
   plus,
   star,
   slash2,
   percent,
   chevron,
   right2,
   left2,
   eq2,
   tilde_eq,
   left_eq,
   right_eq,
   amp2,
   bar2,
   pound,
   dollars,
   eq,
   minus,
   slash,
   amp,
   bar,
   tilde,
   left,
   right,

   invalid_character,

   count_
};

///////////////////////////////////////////////////////////////////////////////
const char* get_name(TokenType type);

///////////////////////////////////////////////////////////////////////////////
const char* to_lua(TokenType type);

} // be::blt

#endif
