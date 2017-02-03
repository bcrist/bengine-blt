#pragma once
#ifndef BE_BLT_LUA_BLT_DEBUG_HPP_
#define BE_BLT_LUA_BLT_DEBUG_HPP_

#include "lua_blt_autolink.hpp"
#include "blt_autolink.hpp"
#include <lua/lua.h>
#include <lua/lauxlib.h>

namespace be {
namespace belua {

///////////////////////////////////////////////////////////////////////////////
int open_blt_debug(lua_State* L);

extern const luaL_Reg blt_debug_module;

} // be::belua
} // be

#endif
