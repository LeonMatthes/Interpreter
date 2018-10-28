#pragma once
#include <nan.h>
#include <programGraph/GraphicalFunction.h>
#include <programGraph/Block.h>
#include <cstdint>
#include <map>

class JSProgramTranslator
{
public:
	using Identifier = int32_t;

	JSProgramTranslator();
	virtual ~JSProgramTranslator() = default;

	class JSProgram* translateProgram(v8::Local<v8::Object> javascriptProgram);
	
protected:
	std::map<Identifier, GraphicalFunction::UPtr> m_functions;
	std::map<Identifier, v8::Local<v8::Value>> m_jsFunctionValues;

	std::map<Identifier, v8::Local<v8::Value>> translateMap(v8::Local<v8::Value> jsMapValue);

	std::vector<Datatype> translateDatatypeArray(v8::Local<v8::Value> jsArrayValue, std::string errorMessage, Identifier currentFunctionID);
	Datatype translateDatatype(v8::Local<v8::Value> jsDatatype, Identifier currentFunctionID);
	std::map<Identifier, GraphicalFunction::UPtr> translateFunctionDeclarations(std::map<Identifier, v8::Local<v8::Value>> jsFunctionsMap);
	GraphicalFunction::UPtr translateFunctionDeclaration(Identifier ID, v8::Local<v8::Value> jsFunction);

	void fillFunctionDefinitions(std::map<Identifier, v8::Local<v8::Value>>& jsDeclarations);
	void fillFunctionDefinition(Identifier ID, v8::Local<v8::Value> jsFunctionValue);
	void checkFunction(Identifier ID);
	
	std::vector<v8::Local<v8::Value>> accessMapKeys(v8::Local<v8::Object> javascriptMap, const char* errorMessage);
	
	std::map<Identifier, Block::Ptr> translateBlockDeclarations(std::map<Identifier, v8::Local<v8::Value>> jsBlockValues, Identifier currentFunctionID);

private:
	Block::Ptr translateBlockDeclaration(Identifier ID, v8::Local<v8::Value> jsBlock, Identifier currentFunctionID);
};