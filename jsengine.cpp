#include "jsengine.h"

#include <QFile>
#include <QDebug>
#include <QScriptValueIterator>
#if !defined(QT_NO_SCRIPTTOOLS)  && (defined(DEBUG) || defined(_DEBUG))
#include <QMainWindow>
#endif

namespace QLScriptEngine
{

JSEngine::JSEngine()
{
}

JSEngine::~JSEngine()
{
}

bool JSEngine::loadFile(const char* filename)
{
	QFile file(filename);
	if (file.open(QFile::ReadOnly))
	{
		QTextStream in(&file);
		in.setCodec("UTF-8");
		QString script = in.readAll();
		QScriptSyntaxCheckResult syntaxResalt = JSEngine::scriptEngine()->checkSyntax(script);
		if (syntaxResalt.state() == QScriptSyntaxCheckResult::Valid)
		{
			QScriptProgram program(script);
			file.close();
			QScriptValue result = JSEngine::scriptEngine()->evaluate(program);

			if (result.isError())
			{
				qDebug() << "Evaluating error" << result.toString();
			}
			else
			{
				return true;
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

	return false;
}

QVariant JSEngine::invokeFunction(const char* object, const char* method, const QVariantList& arguments, QLSCallback callback)
{
	QScriptValue func;

	if (object != nullptr)
	{
		func = JSEngine::scriptEngine()->globalObject().property(object).property(method);
	}
	else
	{
		func = JSEngine::scriptEngine()->globalObject().property(method);
	}

	if (!func.isValid())
	{
		return QVariant();
	}

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
		{
			QObject* obj = qvariant_cast<QObject*>(arg);
			if (obj != nullptr)
			{
				m_callParams.append(scriptEngine()->newQObject(obj));
			}
		}
		break;
		}
	}
	QScriptValue result = func.call(QScriptValue(), m_callParams);
	m_callParams.clear();
	return result.toVariant();
}

QVariant JSEngine::invokeFunction(const char* function, const QVariantList& arguments /*= QVariantList()*/, QLSCallback callback /*= nullptr*/)
{
	return QLScriptEngine::invokeFunction(function, arguments, callback);
}

void JSEngine::exportVariable(const char* name, const QVariant& value)
{
	QObject* qobject = qvariant_cast<QObject*>(value);
	if (qobject != nullptr)
	{
		QScriptValue appProperty = JSEngine::scriptEngine()->newQObject(qobject);
		JSEngine::scriptEngine()->globalObject().setProperty(QString(name), appProperty);
		return;
	}
	else
	{
		QScriptValue appProperty = JSEngine::scriptEngine()->newVariant(value);
		JSEngine::scriptEngine()->globalObject().setProperty(QString(name), appProperty);
	}
}

QScriptEngine* JSEngine::scriptEngine()
{
	if (m_scriptEngine == nullptr)
	{
		m_scriptEngine.reset(new QScriptEngine());
#if !defined(QT_NO_SCRIPTTOOLS)  && (defined(DEBUG) || defined(_DEBUG))
		m_debugger.reset(new QScriptEngineDebugger());
		m_debugger->attachTo(m_scriptEngine.data());
		m_debugger->standardWindow();
#endif
		initializeEngine(m_scriptEngine.data());
	}

	return m_scriptEngine.data();
}

void JSEngine::initializeEngine(QScriptEngine* engine)
{

}


}
