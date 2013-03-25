#include "qlscriptengine/luaengine.h"
#include <iostream>

using namespace QLScriptEngine;

int main()
{
    LuaEngine lua;
    lua.loadFile("example.lua");
    return 0;
}

