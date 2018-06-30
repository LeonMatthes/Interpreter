#pragma once

#include <programGraph/Function.h>
#include <functional>
#include <vector>
#include <programGraph/Value.h>

class PrimitiveFunction : public Function
{
public:
	virtual ~PrimitiveFunction() = default;

	std::vector<Value> operator()(std::vector<Value> inputs) const;

	MAKE_VISITABLE;
protected:
	PrimitiveFunction(std::vector<Datatype> inputTypes, std::vector<Datatype> outputTypes, std::function<std::vector<Value>(std::vector<Value>)> executeFunction);
	
	std::function<std::vector<Value>(std::vector<Value>)> m_executeFunction;
	PrimitiveFunction() = delete;
private:

public:
	//////////////////////////////////////////////////////////////////////////
	//							STATIC
	//////////////////////////////////////////////////////////////////////////
	static PrimitiveFunction add;
	static PrimitiveFunction subtract;
};