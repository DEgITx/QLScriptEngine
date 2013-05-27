#include "qlscriptengine.h"

namespace QLScriptEngine
{
QVariant QLScriptEngine::invokeFunction(const char* function, const QVariantList& arguments /*= QVariantList()*/, QLSCallback callback /*= nullptr*/)
{
	return invokeFunction(nullptr, function, arguments, callback);
}

}