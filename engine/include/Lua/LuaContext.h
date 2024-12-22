#pragma once

struct lua_State;
class LuaContext {
public:
  LuaContext();

private:
  void RegisterMathLibrary();
private:
  lua_State* m_luaState;
};