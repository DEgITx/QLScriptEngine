#include "qlscriptmanager.h"
#include "luaengine.h"
#include "jsengine.h"
#include <QFileInfo>
#include <QFile>

namespace QLScriptEngine
{

QLScriptManager::QLScriptManager()
{

}

QLScriptManager::~QLScriptManager()
{
	foreach(QLScriptEngine* engine, m_engines)
	{
		delete engine;
	}
}

QHash<QString, QVariant> QLScriptManager::invoke( const char* object, const char* method, const QVariantList& arguments /*= QVariantList()*/, QLScriptEngine::QLSCallback callback /*= nullptr*/ )
{
	QHash<QString, QVariant> enginesResults;
	for(auto engine = m_engines.constBegin(); engine != m_engines.constEnd(); ++engine)
	{
		enginesResults[engine.key()] = engine.value()->invokeFunction(object, method, arguments, callback);
	}

	return enginesResults;
}

void QLScriptManager::initializeEngine( const QString& format )
{
	if(!m_engines.contains(format))
	{
		// TODO: move it to format map
		if(format == "lua")
		{
			m_engines[format] = new LuaEngine();
		}
		else if(format == "js")
		{
			m_engines[format] = new JSEngine();
		}
	}
}

void QLScriptManager::setScriptsDirectory(const QDir& directory)
{
	m_scriptsDirectory = directory;
}

void QLScriptManager::loadScripts()
{
	if(m_scriptsDirectory.exists())
	{
		foreach(QString file, m_scriptsDirectory.entryList())
		{
			QString extension = QFileInfo(file).suffix();
			initializeEngine(extension);
			if(m_engines.contains(extension))
			{
				m_engines[extension]->loadFile(m_scriptsDirectory.absoluteFilePath(file).toUtf8().constData());
			}
		}
	}
}



}
