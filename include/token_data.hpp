#pragma once
#ifndef BE_BLT_TOKEN_DATA_HPP_
#define BE_BLT_TOKEN_DATA_HPP_

#include "token_type.hpp"
#include <be/core/be.hpp>
#include <gsl/string_span>

namespace be {
namespace blt {

///////////////////////////////////////////////////////////////////////////////
struct TokenLocation {
   U32 line;
   U32 column;
};

///////////////////////////////////////////////////////////////////////////////
struct TokenData {
   gsl::cstring_span<> text;
   TokenType type;
   TokenLocation location;
};

///////////////////////////////////////////////////////////////////////////////
S pretty_token(const TokenData& token);

} // be::blt
} // be

#endif
