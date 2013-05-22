#ifndef QLSCRIPTENGINE_H
#define QLSCRIPTENGINE_H

#include <QVariant>

namespace QLScriptEngine
{
  
class QLScriptEngine
{
public:
	typedef void (*QLSCallback)(QVariant);

    QLScriptEngine() {};
    virtual ~QLScriptEngine() {};
	
    virtual QVariant invokeFunction(const char* object, const char* method, const QVariantList& arguments = QVariantList(), QLSCallback callback = nullptr) = 0;
    virtual bool loadFile(const char* filename) = 0;
};

}

#endif // QLSCRIPTENGINE_H
