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
#include <pthread.h>

namespace QLScriptEngine
{
  
class LuaEngine : public QLScriptEngine
{
public:
    LuaEngine() : L(lua_open()) {
		loadLuaLibs();
	};
    ~LuaEngine() {
		lua_close(L);
	};
	
	int callEvent(const std::string& table, const std::string& method, int args = 0, int ret = LUA_MULTRET, bool async = false);
	struct CallEventArgs {
		std::string table;
		std::string method;
		int args;
		int ret;
		bool async;
	} callArgs;
	void registerFunction(const char *name, lua_CFunction func);
	void loadFile(const char* filename);
	void loadFile(const std::string& filename) { loadFile(filename.c_str()); };
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
	lua_State *L;
	std::map<std::string, pthread_t> callThread;
	static void *call_thread(void *context);
	static int status;
	int loadFileStatus;
private:
	void loadLuaLibs(void);
	static void reportError(lua_State *L);
};

};

#endif //WITH_LUA

#endif // LUAMANAGER_H
