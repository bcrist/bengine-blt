#include "pch.hpp"
#include "blt.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <sstream>

namespace be::blt {

///////////////////////////////////////////////////////////////////////////////
void compile_blt(SV input, std::ostream& os) {
   Lexer lexer(input);
   lexer.lex_all();
   Parser parser(lexer.tokens());
   parser.generate(os);
}

///////////////////////////////////////////////////////////////////////////////
S compile_blt(SV input) {
   std::ostringstream oss;
   compile_blt(input, oss);
   return oss.str();
}

///////////////////////////////////////////////////////////////////////////////
void debug_blt(SV input, std::ostream& os) {
   Lexer lexer(input);
   lexer.lex_all();
   Parser parser(lexer.tokens());
   parser.debug(os);
}

///////////////////////////////////////////////////////////////////////////////
S debug_blt(SV input) {
   std::ostringstream oss;
   debug_blt(input, oss);
   return oss.str();
}

} // be::blt
