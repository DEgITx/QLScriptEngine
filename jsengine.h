#ifndef JSENGINE_H
#define JSENGINE_H

#include "qlscriptengine.h"
#include <QScriptEngine>
#if !defined(QT_NO_SCRIPTTOOLS)  && (defined(DEBUG) || defined(_DEBUG))
#include <QtScriptTools>
#endif

class Test_Script;

namespace QLScriptEngine
{

class JSEngine : public QLScriptEngine
{
	friend class ::Test_Script;
public:
	JSEngine();
	virtual ~JSEngine();

	bool loadFile(const char* filename) override;
	QVariant invokeFunction(const char* function, const QVariantList& arguments = QVariantList(), QLSCallback callback = nullptr) override;
	QVariant invokeFunction(const char* object, const char* method, const QVariantList& arguments = QVariantList(), QLSCallback callback = nullptr) override;
	void exportVariable(const char* name, const QVariant& value) override;

protected:
	virtual QScriptEngine* scriptEngine();
	virtual void initializeEngine(QScriptEngine* engine);

private:
	QScriptValueList m_callParams;
	QString m_scriptFilename;
	QScopedPointer<QScriptEngine> m_scriptEngine;

#if !defined(QT_NO_SCRIPTTOOLS)  && (defined(DEBUG) || defined(_DEBUG))
	QScopedPointer<QScriptEngineDebugger> m_debugger;
#endif
};

}

#endif // JSENGINE_H
