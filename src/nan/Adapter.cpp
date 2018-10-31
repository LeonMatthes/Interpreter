#include <nan.h>
#include <string>
#include <JSProgram.h>
#include <iostream>
#include <JSProgramTranslator.h>
#include <TranslationError.h>

NAN_METHOD(Testing)
{
	TranslationError t("Test!", 0);
}

NAN_MODULE_INIT(Init)
{
	JSProgram::Init(target);

	Nan::Set(target, Nan::New("primitives").ToLocalChecked(),
		Nan::GetFunction(Nan::New<v8::FunctionTemplate>(JSProgramTranslator::Primitives)).ToLocalChecked());
}

NODE_MODULE(InterpreterNan, Init)