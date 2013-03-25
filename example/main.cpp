#include "qlscriptengine/luaengine.h"
#include <iostream>
#include <assert.h>

using namespace QLScriptEngine;

int main()
{
    LuaEngine lua;
    assert(lua.loadFile("example.lua"));

    lua.luaPush("hellow, world!");
    lua.callEvent("PichiCommands", "example", 1);

    return 0;
}

