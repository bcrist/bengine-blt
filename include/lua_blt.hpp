#pragma once
#ifndef BE_BLT_LUA_BLT_HPP_
#define BE_BLT_LUA_BLT_HPP_

#include <lua/lua.h>
#include <lua/lauxlib.h>

namespace be::belua {

///////////////////////////////////////////////////////////////////////////////
int open_blt(lua_State* L);

///////////////////////////////////////////////////////////////////////////////
int open_blt_compile(lua_State* L);

extern const luaL_Reg blt_module;
extern const luaL_Reg blt_compile_module;

} // be::belua

#endif
