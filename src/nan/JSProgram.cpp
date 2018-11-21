#include <JSProgram.h>
#include <JSProgramTranslator.h>
#include <TranslationError.h>
#include <error/Error.h>
#include <visitors/TypeChecker.h>
#include <error/RuntimeError.h>
#include <error/Error.h>

JSProgram::JSProgram(GraphicalFunction::UPtr start, 
	std::vector<GraphicalFunction::UPtr> functions,
	const std::map<Function*, JSProgramTranslator::Identifier>& functionIdentifiers,
	const std::map<Block*, JSProgramTranslator::Identifier>& blockIdentifiers)
	: m_program(std::move(start), std::move(functions))
	, m_blockIdentifiers{blockIdentifiers}
	, m_functionIdentifiers{functionIdentifiers}
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
	auto jsProgram = (JSProgram*)(nullptr);
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

	auto results = std::vector<Value>();
	auto jsResults = v8::Array::New(info.GetIsolate(), results.size());
	try
	{
		auto parameters = self->translateParameters(info);

		results = self->m_program.run(parameters);
	
		for (size_t i = 0; i < results.size(); i++)
		{
			jsResults->Set(i, self->translateValue(results.at(i)));
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
		parameters.push_back(JSProgramTranslator().translateValue(info[i], Nan::Nothing<JSProgramTranslator::Identifier>(), Nan::Nothing<JSProgramTranslator::Identifier>()));
	}
	return parameters;
}
