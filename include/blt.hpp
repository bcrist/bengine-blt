#pragma once
#ifndef BE_BLT_BLT_HPP_
#define BE_BLT_BLT_HPP_

#include <be/core/be.hpp>

namespace be::blt {

void compile_blt(SV input, std::ostream& os);
S compile_blt(SV input);

void debug_blt(SV input, std::ostream& os);
S debug_blt(SV input);

} // be::blt

#endif
