#ifndef QLSCRIPTENGINE_H
#define QLSCRIPTENGINE_H

namespace QLScriptEngine
{
  
class QLScriptEngine
{
public:
    QLScriptEngine() {};
    virtual ~QLScriptEngine() {};
	
    virtual int callEvent(const char* table, const char* method, int args = 0, int returnValue = 0, bool asyncCall = false) = 0;
    virtual void loadFile(const char* filename) = 0;
};

}

#endif // QLSCRIPTENGINE_H
