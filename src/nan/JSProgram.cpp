#include <JSProgram.h>
#include <JSProgramTranslator.h>
#include <TranslationError.h>
#include <error/Error.h>
#include <visitors/TypeChecker.h>

JSProgram::JSProgram(GraphicalFunction::UPtr start, std::vector<GraphicalFunction::UPtr> functions)
	: m_program(std::move(start), std::move(functions))
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
		results = self->m_program.run({});
	
		for (size_t i = 0; i < results.size(); i++)
		{
			jsResults->Set(i, self->translateValue(results.at(i)));
		}
	}
	catch (Error::Ptr e)
	{
		Nan::ThrowError(Nan::New(e->message()).ToLocalChecked());
		return;
	}
	catch (...)
	{
		Nan::ThrowError(Nan::New("Unknown C++ exception thrown!").ToLocalChecked());
		return;
	}

	info.GetReturnValue().Set(jsResults);
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
