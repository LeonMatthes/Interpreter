#pragma once
#include <JSProgramTranslator.h>
#include <error/Error.h>
#include <nan.h>
#include <programGraph/Program.h>
#include <programGraph/Value.h>
#include <vector>

class JSProgram : public Nan::ObjectWrap
{
public:
	JSProgram(GraphicalFunction::UPtr start, 
		std::vector<GraphicalFunction::UPtr> functions,
		std::map<Function*, JSProgramTranslator::Identifier>  functionIdentifiers,
		std::map<Block*, JSProgramTranslator::Identifier>  blockIdentifiers);
	JSProgram(GraphicalFunction::UPtr start, std::vector<GraphicalFunction::UPtr> functions);
	~JSProgram() override = default;
	
	static NAN_MODULE_INIT(Init);
	static NAN_METHOD(New);
	static NAN_METHOD(run);
	static NAN_METHOD(typeCheck);

	static Nan::Persistent<v8::FunctionTemplate> constructor;

	auto assertTypeCheckBeforeRun(v8::Isolate* isolate)->bool;
	auto typeCheck()->TypeCheckResult;
	auto translateTypeCheck(TypeCheckResult checkResult, v8::Isolate* isolate)->v8::Local<v8::Value>;

protected:
	Program m_program;
	std::map<Block*, JSProgramTranslator::Identifier> m_blockIdentifiers;
	std::map<Function*, JSProgramTranslator::Identifier> m_functionIdentifiers;
	bool m_typeChecked = false;


	void throwError(Error::Ptr error);

	auto translateBlocks(std::set<class Block*> blocks, v8::Isolate* isolate)->v8::Local<v8::Array>;
	auto translateValue(Value& value)->v8::Local<v8::Value>;
	auto translateParameters(Nan::NAN_METHOD_ARGS_TYPE info)->std::vector<Value>;
private:
};