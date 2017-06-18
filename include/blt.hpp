#pragma once
#ifndef BE_BLT_BLT_HPP_
#define BE_BLT_BLT_HPP_

#include "blt_autolink.hpp"
#include <be/core/be.hpp>

namespace be::blt {

void compile_blt(gsl::cstring_span<> input, std::ostream& os);
S compile_blt(gsl::cstring_span<> input);

void debug_blt(gsl::cstring_span<> input, std::ostream& os);
S debug_blt(gsl::cstring_span<> input);

} // be::blt

#endif
