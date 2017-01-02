#pragma once
#ifndef BE_BLT_BLT_HPP_
#define BE_BLT_BLT_HPP_

#include "blt_autolink.hpp"
#include <be/core/be.hpp>

#define BE_BLT_VERSION_MAJOR 0
#define BE_BLT_VERSION_MINOR 1
#define BE_BLT_VERSION_REV 0
#define BE_BLT_VERSION "BLT " BE_STRINGIFY(BE_BLT_VERSION_MAJOR) "." BE_STRINGIFY(BE_BLT_VERSION_MINOR) "." BE_STRINGIFY(BE_BLT_VERSION_REV)

namespace be {
namespace blt {

void compile_blt(gsl::cstring_span<> input, std::ostream& os);
S compile_blt(gsl::cstring_span<> input);

void debug_blt(gsl::cstring_span<> input, std::ostream& os);
S debug_blt(gsl::cstring_span<> input);

} // be::blt
} // be

#endif
