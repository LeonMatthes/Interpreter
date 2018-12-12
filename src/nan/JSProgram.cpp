#include <JSProgram.h>
#include <JSProgramTranslator.h>
#include <TranslationError.h>
#include <error/Error.h>
#include <visitors/TypeChecker.h>
#include <error/RuntimeError.h>

#include <utility>
#include <error/Error.h>
#include <Utilities.h>

JSProgram::JSProgram(GraphicalFunction::UPtr start, 
	std::vector<GraphicalFunction::UPtr> functions,
	std::map<Function*, JSProgramTranslator::Identifier>  functionIdentifiers,
	std::map<Block*, JSProgramTranslator::Identifier>  blockIdentifiers)
	: m_program(std::move(start), std::move(functions))
	, m_blockIdentifiers{std::move(blockIdentifiers)}
	, m_functionIdentifiers{std::move(functionIdentifiers)}
{

}

JSProgram::JSProgram(GraphicalFunction::UPtr start, std::vector<GraphicalFunction::UPtr> functions)
	: JSProgram(std::move(start), std::move(functions), {}, {})
{

}

Nan::Persistent<v8::FunctionTemplate> JSProgram::constructor;

NAN_MODULE_INIT(JSProgram::Init)
{
	v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(JSProgram::New);
	constructor.Reset(ctor);

	ctor->InstanceTemplate()->SetInternalFieldCount(1);
	ctor->SetClassName(Nan::New("JSProgram").ToLocalChecked());

	Nan::SetPrototypeMethod(ctor, "run", run);
	Nan::SetPrototypeMethod(ctor, "typeCheck", typeCheck);

	target->Set(Nan::New("Program").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(JSProgram::New)
{
	if (!info.IsConstructCall())
	{
		return Nan::ThrowError("Program constructor called without new keyword!");
	}

	if (info.Length() != 1)
	{
		return Nan::ThrowError("Program constructor expects one (1) argument");
	}

	if (!info[0]->IsObject())
	{
		return Nan::ThrowError("Program constructor's first argument must be an object");
	}

	JSProgramTranslator translator;
	auto jsProgram = static_cast<JSProgram*>(nullptr);
	try
	{
		jsProgram = translator.translateProgram(info[0]->ToObject());
		if (jsProgram == nullptr)
		{
			throw TranslationError("Unknown Error while translating program!");
		}
	}
	catch (TranslationError error)
	{
		return;
	}
	catch (std::exception& error)
	{
		TranslationError translationError(error.what());
		return;
	}
	catch (...)
	{
		TranslationError error("Unknown c++ exception thrown while translating program!");
		return;
	}

	jsProgram->Wrap(info.Holder());

	info.GetReturnValue().Set(info.Holder());
}

NAN_METHOD(JSProgram::run)
{
	auto* self = Nan::ObjectWrap::Unwrap<JSProgram>(info.This());

	if (!self->assertTypeCheckBeforeRun(info.GetIsolate()))
	{
		return;
	}

	auto results = std::vector<Value>();
	auto jsResults = v8::Array::New(info.GetIsolate(), checked_cast<int>(results.size()));
	try
	{
		auto parameters = self->translateParameters(info);

		results = self->m_program.run(parameters);
	
		for (size_t i = 0; i < results.size(); i++)
		{
			jsResults->Set(checked_cast<uint32_t>(i), self->translateValue(results.at(i)));
		}
	}
	catch (Error::Ptr e)
	{
		self->throwError(e);
		return;
	}
	catch (...)
	{
		Nan::ThrowError(Nan::New("Unknown C++ exception thrown!").ToLocalChecked());
		return;
	}

	info.GetReturnValue().Set(jsResults);
}

NAN_METHOD(JSProgram::typeCheck)
{
	auto* self = Nan::ObjectWrap::Unwrap<JSProgram>(info.This());

	auto checkResult = self->typeCheck();

	info.GetReturnValue().Set(self->translateTypeCheck(checkResult, info.GetIsolate()));
}

void JSProgram::throwError(Error::Ptr error)
{
	auto runtimeError = std::dynamic_pointer_cast<RuntimeError>(error);
	if (!runtimeError)
	{
		Nan::ThrowError(Nan::New(error->message()).ToLocalChecked());
		return;
	}

	auto jsError = Nan::Error(Nan::New(runtimeError->message()).ToLocalChecked());
	auto functionIDPair = m_functionIdentifiers.find(runtimeError->function());
	if (functionIDPair != m_functionIdentifiers.end())
	{
		auto functionID = functionIDPair->second;
		Nan::Set(jsError->ToObject(), Nan::New("functionID").ToLocalChecked(), Nan::New(functionID));
	}
	auto blockIDPair = m_blockIdentifiers.find(runtimeError->block());
	if (blockIDPair != m_blockIdentifiers.end())
	{
		auto blockID = blockIDPair->second;
		Nan::Set(jsError->ToObject(), Nan::New("blockID").ToLocalChecked(), Nan::New(blockID));
	}
	Nan::ThrowError(jsError);
}

auto JSProgram::translateTypeCheck(TypeCheckResult checkResult, v8::Isolate* isolate) ->v8::Local<v8::Value>
{
	auto offenderCount = std::count_if(checkResult.offenders.begin(), checkResult.offenders.end(), [this](std::pair<Function*, std::set<Block*>> offender) {
		return m_functionIdentifiers.find(offender.first) != m_functionIdentifiers.end();
	});

	auto jsResult = v8::Object::New(isolate);
	Nan::Set(jsResult, Nan::New("succeeded").ToLocalChecked(), Nan::New(checkResult.succeeded));

	auto jsOffenders = v8::Array::New(isolate, checked_cast<int>(offenderCount));
	auto index = 0;
	for (const auto& offender : checkResult.offenders)
	{
		if (m_functionIdentifiers.find(offender.first) != m_functionIdentifiers.end())
		{
			auto jsOffender = v8::Object::New(isolate);

			Nan::Set(jsOffender, Nan::New("functionID").ToLocalChecked(), Nan::New(m_functionIdentifiers.at(offender.first)));
			Nan::Set(jsOffender, Nan::New("blockIDs").ToLocalChecked(), translateBlocks(offender.second, isolate));

			jsOffenders->Set(index++, jsOffender);
		}
	}

	Nan::Set(jsResult, Nan::New("offenders").ToLocalChecked(), jsOffenders);
	return jsResult;
}

auto JSProgram::translateBlocks(std::set<class Block*> blocks, v8::Isolate* isolate) ->v8::Local<v8::Array>
{
	auto blockCount = std::count_if(blocks.begin(), blocks.end(), [this](Block* block) {
		return m_blockIdentifiers.find(block) != m_blockIdentifiers.end();
	});
	auto jsBlocks = v8::Array::New(isolate, checked_cast<int>(blockCount));

	auto index = 0;
	for (const auto& block : blocks)
	{
		if (m_blockIdentifiers.find(block) != m_blockIdentifiers.end())
		{
			jsBlocks->Set(index++, Nan::New(m_blockIdentifiers.at(block)));
		}
	}
	return jsBlocks;
}

auto JSProgram::assertTypeCheckBeforeRun(v8::Isolate* isolate) ->bool
{
	if (m_typeChecked)
	{
		return m_typeChecked;
	}

	auto checkResult = typeCheck();
	if (!checkResult)
	{
		auto jsError = Nan::Error(Nan::New("Program did not pass type check!").ToLocalChecked());
		Nan::Set(jsError->ToObject(), Nan::New("checkResult").ToLocalChecked(), translateTypeCheck(checkResult, isolate));
		Nan::ThrowError(jsError);
	}

	return checkResult.succeeded;
}

auto JSProgram::typeCheck() ->TypeCheckResult
{
	auto result = TypeCheckResult();
	auto checker = TypeChecker();
	for (const auto& function : m_program.functions())
	{
		result.merge(function->accept(checker));
	}
	result.merge(m_program.startFunction()->accept(checker));

	m_typeChecked = result.succeeded;
	return result;
}

v8::Local<v8::Value> JSProgram::translateValue(Value& value)
{
	switch (value.type())
	{
	default:
		throw TranslationError("Tried to translate value of unsupported type!");
		break;
	case Datatype::DOUBLE:
		return Nan::New(value.getDouble());
		break;
	case Datatype::BOOLEAN:
		return Nan::New(value.getBoolean());
		break;
	}
}

std::vector<Value> JSProgram::translateParameters(Nan::NAN_METHOD_ARGS_TYPE info)
{
	auto parameters = std::vector<Value>();
	for (size_t i = 0; i < info.Length(); i++)
	{
		auto value = JSProgramTranslator().translateValue(info[checked_cast<int>(i)], Nan::Nothing<JSProgramTranslator::Identifier>(), Nan::Nothing<JSProgramTranslator::Identifier>());
		parameters.emplace_back(value);
	}
	return parameters;
}
