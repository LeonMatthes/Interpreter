#include <JSProgram.h>
#include <JSProgramTranslator.h>
#include <TranslationError.h>

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

	target->Set(Nan::New("JSProgram").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(JSProgram::New)
{
	if (!info.IsConstructCall())
	{
		return Nan::ThrowError("JSProgram() called without new keyword!");
	}

	if (info.Length() != 1)
	{
		return Nan::ThrowError("JSProgram constructor only expects one (1) argument");
	}

	if (!info[0]->IsObject())
	{
		return Nan::ThrowError("JSProgram constructor's first argument must be an object");
	}

	JSProgramTranslator translator;
	auto jsProgram = (JSProgram*)(nullptr);
	try
	{
		jsProgram = translator.translateProgram(info[0]->ToObject());
	}
	catch (TranslationError error)
	{
		return;
	}
	jsProgram->Wrap(info.Holder());

	info.GetReturnValue().Set(info.Holder());
}

NAN_METHOD(JSProgram::run)
{

}