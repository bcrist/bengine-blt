// lexer.cpp - AUTO-GENERATED - DO NOT MODIFY!
// compile with `ragel -L -G1 -o src/lexer.cpp meta/lexer.rl`
#include "pch.hpp"
#include "lexer.hpp"
#include <be/core/exceptions.hpp>

#define BE_BLT_TOKEN_ESTIMATE_RATIO 0.2

namespace be {
namespace blt {
namespace {
%%{
   machine blt_lexer;
   variable p p_;
   variable pe pe_;
   variable ts ts_;
   variable te te_;
   write data;
   
   newline =                  '\r' '\n'? | '\n' ;
   comment =                  '--' [^\r\n]* ;
   whitespace =               [\t\v\f ]+ ;
   whitespace_or_comment =    whitespace | comment ;

   initial_identifier_char =  alpha | '_' ;
   identifier_char =          initial_identifier_char | digit ;
   identifier =               initial_identifier_char identifier_char* ;

   decimal_escape =           '\\' digit{1,3} ;
   hex_escape =               '\\x' xdigit{2} ;
   unicode_escape =           '\\u{' xdigit+ '}';
   z_escape =                 '\\z' ( whitespace | newline )* ;
   char_escape =              '\\' any ;
   escape =                   decimal_escape | hex_escape | unicode_escape | z_escape | char_escape ;
   dq_string =                '"' ( escape | [^\\"] )* '"' ;
   sq_string =                '\'' ( escape | [^\\'] )* '\'' ;
   string_literal =           dq_string | sq_string ;

   hex_exponent =             'p'i ( '+' | '-' )? digit+ ;
   hex_literal =              '0x'i ( xdigit+ ( '.' xdigit* )? | '.' xdigit+ ) hex_exponent? ;
   decimal_exponent =         'e'i ( '+' | '-' )? digit+ ;
   decimal_literal =          ( digit+ ( '.' digit* )? | '.' digit+ ) decimal_exponent? ;
   numeric_literal =          hex_literal | decimal_literal;

   main := |*

      whitespace_or_comment => { };
      newline => {
         ls_ = te_;
         ++line_;
      };

      numeric_literal => { t_(TokenType::numeric_literal); };
      string_literal  => { t_(TokenType::string_literal); check_newlines_(); };

      'if'        => { t_(TokenType::kw_if); };
      'else'      => { t_(TokenType::kw_else); };
      'while'     => { t_(TokenType::kw_while); };
      'for'       => { t_(TokenType::kw_for); };
      'with'      => { t_(TokenType::kw_with); };
      'each'      => { t_(TokenType::kw_each); };
      'using'     => { t_(TokenType::kw_using); };
      'choose'    => { t_(TokenType::kw_choose); };
      'break'     => { t_(TokenType::kw_break); };
      'continue'  => { t_(TokenType::kw_continue); };
      'until'     => { t_(TokenType::kw_until); };
      'and'       => { t_(TokenType::kw_and); };
      'or'        => { t_(TokenType::kw_or); };
      'not'       => { t_(TokenType::kw_not); };
      'nil'       => { t_(TokenType::kw_nil); };
      'false'     => { t_(TokenType::kw_false); };
      'true'      => { t_(TokenType::kw_true); };

      ';'   => { t_(TokenType::semicolon); };
      '{'   => { t_(TokenType::brace_opener); };
      '}'   => { t_(TokenType::brace_closer); };
      '('   => { t_(TokenType::paren_opener); };
      ')'   => { t_(TokenType::paren_closer); };
      '['   => { t_(TokenType::bracket_opener); };
      ']'   => { t_(TokenType::bracket_closer); };      
      '.'   => { t_(TokenType::dot); };
      '..'  => { t_(TokenType::dot2); };
      ','   => { t_(TokenType::comma); };
      '='   => { t_(TokenType::eq); };
      '?'   => { t_(TokenType::question); };
      ':'   => { t_(TokenType::colon); };
      '+'   => { t_(TokenType::plus); };
      '-'   => { t_(TokenType::minus); };
      '*'   => { t_(TokenType::star); };
      '/'   => { t_(TokenType::slash); };
      '//'  => { t_(TokenType::slash2); };
      '%'   => { t_(TokenType::percent); };
      '^'   => { t_(TokenType::chevron); };
      '&'   => { t_(TokenType::amp); };
      '|'   => { t_(TokenType::bar); };
      '~'   => { t_(TokenType::tilde); };
      '>>'  => { t_(TokenType::right2); };
      '<<'  => { t_(TokenType::left2); };
      '=='  => { t_(TokenType::eq2); };
      '~='  => { t_(TokenType::tilde_eq); };
      '<'   => { t_(TokenType::left); };
      '>'   => { t_(TokenType::right); };
      '<='  => { t_(TokenType::left_eq); };
      '>='  => { t_(TokenType::right_eq); };
      '&&'  => { t_(TokenType::amp2); };
      '||'  => { t_(TokenType::bar2); };
      '#'   => { t_(TokenType::pound); };
      '$'   => { t_(TokenType::dollars); };

      identifier => { t_(TokenType::identifier); };

      any => { t_(TokenType::invalid_character); };
   *|;

}%%
} // be::blt::()

///////////////////////////////////////////////////////////////////////////////
void Lexer::lex_backtick_(gsl::cstring_span<> contents) {
   p_ = contents.data();
   pe_ = p_ + contents.length();
   const char* eof = pe_;
   int cs;
   int act;
   
   %% write init;
   %% write exec;

   if (cs == blt_lexer_error) {
      throw RecoverableException<void>("Lexer error!");
   }
}

///////////////////////////////////////////////////////////////////////////////
Lexer::Lexer(gsl::cstring_span<> input)
   : input_(input)
{ }

///////////////////////////////////////////////////////////////////////////////
void Lexer::lex_all() {
   tokens_.clear();
   tokens_.reserve(std::max((std::size_t)32, std::min((std::size_t)input_.length(),
      (std::size_t)(input_.length() * BE_BLT_TOKEN_ESTIMATE_RATIO))));

   ps_ = input_.data();
   const char* i = ps_;
   const char* end = ps_ + input_.length();
   ls_ = ts_ = ps_;
   const char* last_ls = ls_;
   line_ = 1;
   U32 last_line = line_;

   doc:
      if (i == end) {
         te_ = i;
         if (ts_ != te_) {
            t_(TokenType::document);
         }
         return;
      }
      if (*i == '\n') {
         ++last_line;
         last_ls = i + 1;
      } else if (*i == '`') {
         te_ = i;
         if (ts_ != te_) {
            t_(TokenType::document);
         }
         ts_ = ++i;
         line_ = last_line;
         ls_ = last_ls;
         goto backtick;
      }
      ++i;
      goto doc;

   backtick:
      if (i == end) {
         te_ = i;
         if (ts_ != te_) {
            std::ptrdiff_t offset = ts_ - ps_;
            std::ptrdiff_t length = te_ - ts_;
            lex_backtick_(input_.subspan(offset, length));
         }
         return;
      }
      if (*i == '!') {
         te_ = i;
         if (ts_ != te_) {
            std::ptrdiff_t offset = ts_ - ps_;
            std::ptrdiff_t length = te_ - ts_;
            lex_backtick_(input_.subspan(offset, length));
         }
         ts_ = ++i;
         last_line = line_;
         last_ls = ls_;
         goto rawlua;
      } else if (*i == '`') {
         te_ = i;
         if (ts_ == te_) {
            --ts_;
            if (*ts_ == '`') {
               t_(TokenType::document);
            }
         } else {
            std::ptrdiff_t offset = ts_ - ps_;
            std::ptrdiff_t length = te_ - ts_;
            lex_backtick_(input_.subspan(offset, length));
         }
         ts_ = ++i;
         last_line = line_;
         last_ls = ls_;
         goto doc;
      }
      ++i;
      goto backtick;

   rawlua:
      if (i == end) {
         te_ = i;
         if (ts_ != te_) {
            t_(TokenType::lua);
         }
         return;
      }
      if (*i == '\n') {
         ++last_line;
         last_ls = i + 1;
      } else if (*i == '!') {
         te_ = i;
         if (ts_ != te_) {
            t_(TokenType::lua);
         }
         ts_ = ++i;
         line_ = last_line;
         ls_ = last_ls;
         goto backtick;
      } else if (*i == '`') {
         te_ = i;
         if (ts_ != te_) {
            t_(TokenType::lua);
         }
         ts_ = ++i;
         line_ = last_line;
         ls_ = last_ls;
         goto doc;
      }
      ++i;
      goto rawlua;
}

///////////////////////////////////////////////////////////////////////////////
const std::vector<TokenData>& Lexer::tokens() {
   return tokens_;
}

///////////////////////////////////////////////////////////////////////////////
void Lexer::t_(TokenType type) {
   std::ptrdiff_t offset = ts_ - ps_;
   std::ptrdiff_t length = te_ - ts_;
   U32 line = line_;
   U32 column = (U32)(ts_ - ls_ + 1);
   tokens_.push_back({ input_.subspan(offset, length), type, { line, column } });
}

///////////////////////////////////////////////////////////////////////////////
void Lexer::t_(TokenType type, U32 line, U32 column, gsl::cstring_span<> text) {
   tokens_.push_back({ text, type, { line, column } });
}

///////////////////////////////////////////////////////////////////////////////
void Lexer::check_newlines_() {
   bool skip_lf = false;
   for (auto& c : tokens_.back().text) {
      if (c == '\r') {
	     ls_ = &c + 1;
		 ++line_;
		 skip_lf = true;
	  } else if (c == '\n') {
	     ls_ = &c + 1;
		 if (skip_lf) {
		    skip_lf = false;
		 } else {
		    ++line_;
		 }
	  } else {
	     skip_lf = false;
	  }
   }
}

} // be::blt
} // be
