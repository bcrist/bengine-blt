#pragma once
#ifndef BE_BLT_LEXER_HPP_
#define BE_BLT_LEXER_HPP_

#include "token_data.hpp"
#include <vector>

namespace be::blt {

///////////////////////////////////////////////////////////////////////////////
class Lexer final : Immovable {
public:
   Lexer(SV input);

   void lex_all();
   const std::vector<TokenData>& tokens();

private:
   void lex_backtick_(SV contents);
   void t_(TokenType type);
   void t_(TokenType type, U32 line, U32 column, SV text);
   void check_newlines_();

   SV input_;
   std::vector<TokenData> tokens_;
   const char* ps_;
   const char* p_;
   const char* pe_;
   const char* ts_;
   const char* te_;
   const char* ls_;
   U32 line_;
};

} // be::blt

#endif
