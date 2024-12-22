#include "Lua/LuaContext.h"
#include "Math/float2.h"
#include <luajit-2.1/lua.hpp>
#include <LuaBridge/LuaBridge.h>

LuaContext::LuaContext() {
    m_luaState = luaL_newstate();
    luaL_openlibs(m_luaState);
}

void LuaContext::RegisterMathLibrary()
{
    luabridge::getGlobalNamespace(m_luaState)
    .beginNamespace("paw")
    .beginNamespace("math")
    .endNamespace()
    .endNamespace();
}