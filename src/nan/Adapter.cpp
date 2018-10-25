#include <nan.h>
#include <string>
#include <JSProgram.h>
#include <iostream>
#include <JSProgramTranslator.h>

NAN_METHOD(Testing)
{
	auto translator = JSProgramTranslator();
	auto keys = translator.accessMapKeys(info[0]->ToObject(), "Some error occured!");
	std::cout << keys.size() << std::endl;

	info.GetReturnValue().Set(true);
}

NAN_MODULE_INIT(Init)
{
	JSProgram::Init(target);
	Nan::Set(target, Nan::New("Testing").ToLocalChecked(), 
		Nan::GetFunction(Nan::New<v8::FunctionTemplate>(Testing)).ToLocalChecked());
}

NODE_MODULE(InterpreterNan, Init)