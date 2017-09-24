#include "lua_blt_debug.hpp"
#include <be/blt/blt.hpp>
#include <be/belua/lua_helpers.hpp>

namespace be::belua {

///////////////////////////////////////////////////////////////////////////////
const luaL_Reg blt_debug_module { "be.blt.debug", open_blt_debug };

namespace {

///////////////////////////////////////////////////////////////////////////////
int blt_debug(lua_State* L) {
   SV input = belua::to_string_view(L, 1);
   S output = blt::debug_blt(input);
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
