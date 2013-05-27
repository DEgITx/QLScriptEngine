#ifdef WITH_LUA

#include "luaengine.h"
#include <cstring>

namespace QLScriptEngine
{

int LuaEngine::status;

LuaEngine::LuaEngine() : L(lua_open())
{
	luaL_openlibs(L);
}

LuaEngine::~LuaEngine()
{
	lua_close(L);
}

void LuaEngine::call_thread(const LuaEventContext& context)
{
	//Log(std::string("[LUA][CALL]") + th->callArgs.table + ":" + th->callArgs.method, Log::VERBOSE);
	lua_getglobal(context.L, context.table.c_str()); // args + 1
	lua_pushstring(context.L, context.method.c_str()); // args + 2
	if (lua_istable(context.L, -2)) // есть такая таблица
	{
		lua_gettable(context.L, -2); // args + 2
		lua_remove(context.L, -2); // args + 1
		lua_insert(context.L, 1); // args + 1 (смещаем на 1 вниз)
		LuaEngine::status = lua_pcall(context.L, context.args, context.ret, 0);
		reportError(context.L);
	}
	else
	{
		lua_settop(context.L, 0);
	}
}

QVariant LuaEngine::invokeFunction(const char* object, const char* method, const QVariantList& arguments /*= QVariantList()*/, QLSCallback callback /*= nullptr*/)
{
	int argc = arguments.count();
#ifdef Q_OS_WIN
	foreach(const QVariant & argv, arguments)
#else
	for (const QVariant & argv : arguments)
#endif
	{
		luaPush(argv);
	}

	invokeFunction(std::string(object), std::string(method), argc, 1, callback != nullptr);

	QVariant result;

	if (lua_isnumber(L, -1))
	{
		result = luaPopNumber();
	}
	else if (lua_isstring(L, -1))
	{
		result = QString(luaPopString().c_str());
	}
	else if (lua_isuserdata(L, -1))
	{
		result = (qulonglong)lua_touserdata(L, -1);
	}
	else if (lua_istable(L, -1))
	{
		QHash<QString, QVariant> table;
		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			table[lua_tostring(L, -2)] = lua_tostring(L, -1);
			lua_pop(L, 1);
		}
		lua_pop(L, 1); // remove table

		result = table;
	}

	return result;
}

int LuaEngine::invokeFunction(const std::string& table, const std::string& method, int args, int ret, bool async)
{
	LuaEventContext context = {L, table, method, args, ret};
#ifndef Q_OS_WIN32
	std::thread thread(call_thread, context);
	if (!async)
	{
		thread.join();
	}
#else
	call_thread_q.context = context;
	call_thread_q.start();
	if (!async)
	{
		call_thread_q.wait();
	}
#endif
	return 0;
}

void LuaEngine::registerFunction(const char* name, lua_CFunction func)
{
	//Log(std::string("[LUA][REGISTER]") + name, Log::DEBUG);
	lua_register(L, name, func);
}

bool LuaEngine::loadFile(const char* filename)
{
	//Log(std::string("[LUA][LOAD]") + filename, Log::DEBUG);
	return (loadFileStatus = luaL_dofile(L, filename)) == 0;
}

void LuaEngine::reload()
{
	// Full reload
	lua_close(L);
	L = lua_open();
	luaL_openlibs(L);
}

void LuaEngine::reportError(lua_State* L)
{
	if (LuaEngine::status != 0)
	{
		//Log(std::string("[LUA][ERROR]") + lua_tostring(L, -1), Log::WARNING);
		lua_pop(L, 1);
	}
}


void LuaEngine::luaPush(int i)
{
	lua_pushnumber(L, i);
}

void LuaEngine::luaPush(bool b)
{
	lua_pushboolean(L, b);
}

void LuaEngine::luaPush(const std::string& str)
{
	lua_pushlstring(L, str.data(), str.size());
}

void LuaEngine::luaPush(const char* str)
{
	lua_pushlstring(L, str, strlen(str));
}

void LuaEngine::luaPush(void* func)
{
	lua_pushlightuserdata(L, func);
}

void LuaEngine::luaPush(const std::map< std::string, std::string >& map)
{
	lua_newtable(L);
#ifdef Q_OS_WIN
	typedef std::pair<std::string, std::string> PairStrings;
	foreach(PairStrings row, map)
#else
	for (std::pair<std::string, std::string> row : map)
#endif
	{
		lua_pushstring(L, row.first.c_str());
		lua_pushstring(L, row.second.c_str());
		lua_settable(L, -3);
	}
}

void LuaEngine::luaPush(const QString& string)
{
	luaPush(string.toStdString());
}

void LuaEngine::luaPush(const QVariant& variant)
{
	switch (variant.type())
	{
	case QVariant::Bool:
	{
		luaPush(variant.toBool());
		break;
	}
	case QVariant::String:
	{
		luaPush(variant.toString());
		break;
	}
	case QVariant::Int:
	{
		luaPush(variant.toInt());
		break;
	}
	case QVariant::Map:
	{
		luaPush(variant.toMap());
		break;
	}
	case QVariant::Hash:
	{
		luaPush(variant.toHash());
		break;
	}
	default:
		break;
	}
}

void LuaEngine::luaPush(const QHash<QString, QVariant>& hash)
{
	lua_newtable(L);
	for (QHash<QString, QVariant>::const_iterator it = hash.constBegin(); it != hash.constEnd(); ++it)
	{
		luaPush(it.key());
		luaPush(it.value());
		lua_settable(L, -3);
	}
}

void LuaEngine::luaPush(const QMap<QString, QVariant>& hash)
{
	lua_newtable(L);
	for (QMap<QString, QVariant>::const_iterator it = hash.constBegin(); it != hash.constEnd(); ++it)
	{
		luaPush(it.key());
		luaPush(it.value());
		lua_settable(L, -3);
	}
}

double LuaEngine::luaPopNumber(void)
{
	double back;
	if (lua_isnumber(L, -1))
	{
		back = lua_tonumber(L, -1);
	}
	lua_pop(L, 1);
	return back;
}

std::string LuaEngine::luaPopString(void)
{
	std::string back;
	if (lua_isstring(L, -1))
	{
		back = lua_tostring(L, -1);
	}
	lua_pop(L, 1);
	return back;
}

std::map< std::string, std::string > LuaEngine::luaPopTable(void)
{
	std::map< std::string, std::string > table;
	if (lua_istable(L, -1))
	{
		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			table[lua_tostring(L, -2)] = lua_tostring(L, -1);
			lua_pop(L, 1);
		}
		lua_pop(L, 1); // remove table
	}
	return table;
}

void LuaEngine::exportVariable(const char* name, const QVariant& value)
{

}


#ifdef Q_OS_WIN32
void LuaEngine::_CallThread::run()
{
	call_thread(this->context);
}
#endif

}


#endif
