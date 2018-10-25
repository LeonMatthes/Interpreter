#pragma once
#include <nan.h>
#include <programGraph/Program.h>
#include <programGraph/Value.h>
#include <vector>

class JSProgram : public Nan::ObjectWrap
{
public:
	JSProgram(GraphicalFunction::UPtr start, std::vector<GraphicalFunction::UPtr> functions);
	virtual ~JSProgram() = default;
	
	static NAN_MODULE_INIT(Init);
	static NAN_METHOD(New);
	static NAN_METHOD(run);

	static Nan::Persistent<v8::FunctionTemplate> constructor;

protected:
	Program m_program;
private:
};