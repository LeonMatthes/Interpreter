#pragma once
#include <nan.h>
#include <programGraph/GraphicalFunction.h>
#include <cstdint>
#include <map>

class JSProgramTranslator
{
public:
	JSProgramTranslator();
	virtual ~JSProgramTranslator() = default;

	class JSProgram* translateProgram(v8::Local<v8::Object> javascriptProgram);
	
	std::map<int64_t, GraphicalFunction::UPtr> translateFunctionDeclarations(v8::Local<v8::Value> functionMap);
	GraphicalFunction::UPtr translateFunctionDeclaration(v8::Local<v8::Value> jsFunction);

	std::vector<Datatype> translateDatatypeArray(v8::Local<v8::Value> jsArray);
	Datatype translateDatatype(v8::Local<v8::Value> jsDatatype);
	
	std::vector<v8::Local<v8::Value>> accessMapKeys(v8::Local<v8::Object> javascriptMap, const char* errorMessage);
protected:
	
	class GraphicalFunction::UPtr translateFunction(v8::Local<v8::Object> javascriptFunction);

private:
};