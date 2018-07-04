#pragma once

#include <programGraph/Function.h>

class MockFunction : public Function
{
public:
	MockFunction() = default;

	MockFunction(std::vector<Datatype> in, std::vector<Datatype> out)
		: Function(in, out)
	{}

	virtual ~MockFunction() = default;

	MOCK_VISITABLE;
protected:
private:
};