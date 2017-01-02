#include "pch.hpp"
#include "blt.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <sstream>

namespace be {
namespace blt {

///////////////////////////////////////////////////////////////////////////////
void compile_blt(gsl::cstring_span<> input, std::ostream& os) {
   Lexer lexer(input);
   lexer.lex_all();
   Parser parser(lexer.tokens());
   parser.generate(os);
}

///////////////////////////////////////////////////////////////////////////////
S compile_blt(gsl::cstring_span<> input) {
   std::ostringstream oss;
   compile_blt(input, oss);
   return oss.str();
}

///////////////////////////////////////////////////////////////////////////////
void debug_blt(gsl::cstring_span<> input, std::ostream& os) {
   Lexer lexer(input);
   lexer.lex_all();
   Parser parser(lexer.tokens());
   parser.debug(os);
}

///////////////////////////////////////////////////////////////////////////////
S debug_blt(gsl::cstring_span<> input) {
   std::ostringstream oss;
   debug_blt(input, oss);
   return oss.str();
}

} // be::blt
} // be
