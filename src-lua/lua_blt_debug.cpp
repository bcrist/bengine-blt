#include "lua_blt_debug.hpp"
#include <be/blt/blt.hpp>

namespace be {
namespace belua {

///////////////////////////////////////////////////////////////////////////////
const luaL_Reg blt_debug_module { "be.blt.debug", open_blt_debug };

namespace {

///////////////////////////////////////////////////////////////////////////////
int blt_debug(lua_State* L) {
   std::size_t len;
   const char* input = luaL_tolstring(L, 1, &len);
   S output = blt::debug_blt(gsl::cstring_span<>(input, len));
   lua_settop(L, 0);
   lua_pushlstring(L, output.c_str(), output.length());
   return 1;
}

} // be::lua::()

///////////////////////////////////////////////////////////////////////////////
int open_blt_debug(lua_State* L) {
   lua_pushcfunction(L, blt_debug);
   return 1;
}

} // be::belua
} // be
