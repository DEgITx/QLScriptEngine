#ifndef LUAENGINE_H
#define LUAENGINE_H

#ifdef WITH_LUA

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "qlscriptengine.h"
#include <string>
#include <map>
#include <thread>

namespace QLScriptEngine
{
  
class LuaEngine : public QLScriptEngine
{
public:
	struct LuaEventContext
	{
		lua_State *L;
		std::string table;
		std::string method;
		int args;
		int ret;
	};

    LuaEngine();
    ~LuaEngine();
	
    int callEvent(const char* table, const char* method, int args = 0, int returnValue = 0, bool asyncCall = false);
    int callEvent(const std::string& table, const std::string& method, int args = 0, int returnValue = 0, bool asyncCall = false);
	void registerFunction(const char *name, lua_CFunction func);
    bool loadFile(const char* filename);
    bool loadFile(const std::string& filename) { loadFile(filename.c_str()); };
	void reload();
	
	int getStatus(){ return status; };
	int getFileStatus(){ return loadFileStatus; };
	
	void luaPush(int i);
	void luaPush(bool b);
	void luaPush(const std::string& str);
	void luaPush(const std::map<std::string, std::string>& map);
	void luaPush(const char* str);
	void luaPush(void* func);
	template< typename T > void luaPush(T* p){ luaPush((void*)p); };
	std::string luaPopString(void);
	double luaPopNumber(void);
	template< typename T> T* luaPopData(void) {
		T* back;
		if(lua_isuserdata(L, -1))
			back = lua_touserdata(L, -1);
		lua_pop(L, 1);
		return back;
	};
	std::map<std::string, std::string> luaPopTable(void);
protected:
	static void call_thread(const LuaEventContext& context);
	static int status;
	int loadFileStatus;
private:
	lua_State *L;
	static void reportError(lua_State *L);
};

}

#endif // WITH_LUA

#endif // LUAENGINE_H
