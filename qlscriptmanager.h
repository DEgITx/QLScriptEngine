#ifndef QLSCRIPTMANAGER_H
#define QLSCRIPTMANAGER_H

#include <QHash>
#include <QString>
#include <QDir>
#include "qlscriptengine.h"

#define ScriptManager() ::QLScriptEngine::QLScriptManager::Instance()

namespace QLScriptEngine
{

class QLScriptManager
{
public:
	QLScriptManager();
	~QLScriptManager();

	static QLScriptManager& Instance()
	{
		static QLScriptManager instance;
		return instance;
	}

	void initializeEngine(const QString& format);
	QHash<QString, QVariant> invoke(const char* function, const QVariantList& arguments = QVariantList(), QLScriptEngine::QLSCallback callback = nullptr);
	QHash<QString, QVariant> invoke(const char* object, const char* method, const QVariantList& arguments = QVariantList(), QLScriptEngine::QLSCallback callback = nullptr);
	void setScriptsDirectory(const QDir& directory);
	void loadScripts();
	void exportVariable(const char* name, const QVariant& value);
	void changeEngine(const QString& format, QLScriptEngine* engine);
private:
	QHash<QString, QLScriptEngine*> m_engines;
	QDir m_scriptsDirectory;
};

}

#endif // QLSCRIPTMANAGER_H
