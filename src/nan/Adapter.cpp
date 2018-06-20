#include <nan.h>
#include <test.h>
#include <string>

using namespace v8;


NAN_METHOD(Method) {
	std::string fibonacci = std::to_string(fib(5));
	info.GetReturnValue().Set(Nan::New(fibonacci).ToLocalChecked());
}


NAN_MODULE_INIT(Init)
{
    Nan::Set(target, Nan::New("hello").ToLocalChecked(), 
        Nan::GetFunction(Nan::New<FunctionTemplate>(Method)).ToLocalChecked());
}

NODE_MODULE(InterpreterNan, Init)