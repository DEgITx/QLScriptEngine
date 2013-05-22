#ifndef JSENGINE_H
#define JSENGINE_H

#include "qlscriptengine.h"
#include <QScriptEngine>
#include <QtScriptTools>

namespace QLScriptEngine
{

class JSEngine : public QLScriptEngine
{
public:
	JSEngine();
	virtual ~JSEngine();

	bool loadFile(const char* filename) override;
	QVariant invokeFunction(const char* object, const char* method, const QVariantList& arguments = QVariantList(), QLSCallback callback = nullptr) override;

	//QScriptEngine* scriptEngine();

	/*
	template<typename T>
	void invokeFunction(const QString& funcName, const T& strParam)
	{
		QScriptValue func = scriptEngine()->globalObject().property(funcName);
		func.call(QScriptValue(), QScriptValueList() << strParam);
	}

	void invokeFunction(const QString& funcName, const QScriptValueList& params)
	{
		QScriptValue func = scriptEngine()->globalObject().property(funcName);
		func.call(QScriptValue(), params);
	}
	*/

private:
	QString m_scriptFilename;
	QScriptEngine m_scriptEngine;
	QScriptValueList m_callParams;

	void exposeQObject(QObject* obj, const QString& scriptVarName);


//#if !defined(QT_NO_SCRIPTTOOLS)  && (defined(DEBUG) || defined(_DEBUG))
//	QMainWindow* m_debugWindow;
//	QScopedPointer<QScriptEngineDebugger> m_debugger;
//#endif
};

}

#endif // JSENGINE_H
