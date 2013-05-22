#include "jsengine.h"

#include <QFile>
#include <QDebug>
#include <QScriptValueIterator>

namespace QLScriptEngine
{

JSEngine::JSEngine()
{
}

JSEngine::~JSEngine()
{
}

/*
QScriptEngine* JSEngine::scriptEngine()
{
	if (m_scriptEngine.isNull())
	{
		m_scriptEngine.reset(new QScriptEngine());
#if !defined(QT_NO_SCRIPTTOOLS)  && (defined(DEBUG) || defined(_DEBUG))
		m_debugger.reset(new QScriptEngineDebugger());
		m_debugger->attachTo(m_scriptEngine.data());
		m_debugWindow = m_debugger->standardWindow();
#endif
		loadScriptFile(m_scriptFilename);
		ScriptExtension* extension = new ScriptExtension(m_scriptEngine.data());
		exposeQObject(extension, "ext");
	}

	return m_scriptEngine.data();
}
*/

bool JSEngine::loadFile( const char* filename )
{
	//Q_ASSERT(m_scriptEngine);
	QFile file(filename);
	if (file.open(QFile::ReadOnly))
	{
		QTextStream in(&file);
		in.setCodec("UTF-8");
		QString script = in.readAll();
		QScriptSyntaxCheckResult syntaxResalt = m_scriptEngine.checkSyntax(script);
		if (syntaxResalt.state() == QScriptSyntaxCheckResult::Valid)
		{
			QScriptProgram program(script);
			file.close();
			QScriptValue result = m_scriptEngine.evaluate(program);

			if (result.isError())
			{
				qDebug() << "Evaluating error" << result.toString();
			}
		}
		else     // SyntaxError
		{
			qDebug() << "Syntax error:" << syntaxResalt.errorMessage() << " line:" << syntaxResalt.errorLineNumber();
		}
	}
	else
	{
		qDebug() << __FUNCTION__ << "Can not open script file";
	}
	return true;
}

QVariant JSEngine::invokeFunction(const char* object, const char* method, const QVariantList& arguments, QLSCallback callback)
{
	foreach(QVariant arg, arguments)
	{
		switch (arg.type())
		{
		case QVariant::String:
		{
			m_callParams.append(arg.toString());
			break;
		}
		case QVariant::Int:
		{
				m_callParams.append(arg.toInt());
				break;
		}
		case QVariant::UInt:
			{
				m_callParams.append(arg.toUInt());
				break;
			}
		case QVariant::Bool:
			{
				m_callParams.append(arg.toBool());
				break;
			}
		default:
			break;
		}
	}
	QScriptValue func = m_scriptEngine.globalObject().property(object).property(method);
	if(!func.isValid())
	{
		return QVariant();
	}
	QScriptValue result = func.call(QScriptValue(), m_callParams);
	m_callParams.clear();
	return result.toVariant();
}

void JSEngine::exportVariable( const char* name, const QVariant& value )
{
	QObject* qobject = qvariant_cast<QObject*>(value);
	if(qobject != nullptr)
	{
		QScriptValue appProperty = m_scriptEngine.newQObject(qobject);
		m_scriptEngine.globalObject().setProperty(QString(name), appProperty);
		return;
	}
}


}
