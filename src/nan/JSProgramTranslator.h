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

	static NAN_METHOD(Datatypes);
	static NAN_METHOD(Primitives);
	static auto primitiveIdentifiers()->std::map<Identifier, PrimitiveFunction&>;
	static auto castingTable(Datatype type)->std::map<Datatype, Identifier>;
	
	static v8::Local<v8::Value> datatypeInformation(v8::Isolate* isolate, Datatype type);
	auto translateValue(v8::Local<v8::Value> jsValueValue, Nan::Maybe<Identifier> currentFunction, Nan::Maybe<Identifier> currentBlock)->Value;
protected:
	static auto datatypeArrayToJS(v8::Isolate* isolate, std::vector<Datatype> datatypes)->v8::Local<v8::Array>;

	auto translateMap(v8::Local<v8::Value> jsMapValue, std::string errorMessage)->std::map<Identifier, v8::Local<v8::Value>>;

	auto translateDatatypeArray(v8::Local<v8::Value> jsArrayValue, std::string errorMessage, Identifier currentFunctionID)->std::vector<Datatype>;
	auto translateDatatype(v8::Local<v8::Value> jsDatatype, Identifier currentFunctionID)->Datatype;
	auto translateFunctionDeclarations(std::map<Identifier, v8::Local<v8::Value>> jsFunctionsMap)->std::map<Identifier, GraphicalFunction::UPtr>;
	auto translateFunctionDeclaration(Identifier ID, v8::Local<v8::Value> jsFunction)->GraphicalFunction::UPtr;
	auto functionIDs()->std::map<Function*, Identifier>;

	void fillFunctionDefinitions(std::map<Identifier, v8::Local<v8::Value>>& jsDeclarations);
	void fillFunctionDefinition(Identifier ID, v8::Local<v8::Value> jsFunctionValue);
	void checkFunction(Identifier ID);
	
	auto accessMapKeys(v8::Local<v8::Object> javascriptMap, const char* errorMessage)->std::vector<v8::Local<v8::Value>>;
	
	auto translateBlockDeclarations(std::map<Identifier, v8::Local<v8::Value>> jsBlockValues, Identifier currentFunctionID)->std::map<Identifier, Block::Ptr>;
	auto translateBlockDeclaration(Nan::Maybe<Identifier> ID, v8::Local<v8::Value> jsBlockValue, Identifier currentFunctionID)->Block::Ptr;

	void translateBlockConnections(v8::Local<v8::Value> jsConnections, std::map<JSProgramTranslator::Identifier, Block::Ptr>& blocksMap, Identifier currentFunction);
	void translateBlockConnection(v8::Local<v8::Value> jsConnectionValue, std::map<Identifier, Block::Ptr>& blocksMap, Identifier currentFunction);

	auto translateIdentifier(v8::Local<v8::Value> numberValue, std::string errorMessage, Identifier currentFunction)->Identifier;

	auto translateFunctionBlock(Nan::Maybe<Identifier> ID, v8::Local<v8::Object> jsBlock, Identifier currentFunctionID)->Block::Ptr;
	void fillVariables(GraphicalFunction& graphical, Identifier ID, v8::Local<v8::Value> jsVariablesValue);
	void fillVariable(GraphicalFunction& graphical, Identifier ID, v8::Local<v8::Value> variableValue);

	std::map<Identifier, GraphicalFunction::UPtr> m_functions;
	std::map<Identifier, v8::Local<v8::Value>> m_jsFunctionValues;
	std::map<Block*, Identifier> m_blockIdentifiers;
private:
};