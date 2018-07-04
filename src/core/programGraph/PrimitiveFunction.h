#pragma once

#include <programGraph/Function.h>
#include <functional>
#include <vector>
#include <programGraph/Value.h>

class PrimitiveFunction : public Function
{
	//////////////////////////////////////////////////////////////////////////
	//						INSTANCE
	//////////////////////////////////////////////////////////////////////////
public:
	virtual ~PrimitiveFunction() = default;

	std::vector<Value> operator()(std::vector<Value> inputs) const;

	MAKE_VISITABLE;
protected:
	PrimitiveFunction(std::vector<Datatype> inputTypes, std::vector<Datatype> outputTypes, std::function<std::vector<Value>(std::vector<Value>)> executeFunction);
	
	std::function<std::vector<Value>(std::vector<Value>)> m_executeFunction;
	PrimitiveFunction() = delete;
private:

	//////////////////////////////////////////////////////////////////////////
	//							STATIC
	//////////////////////////////////////////////////////////////////////////
public:
	static PrimitiveFunction add;
	static PrimitiveFunction subtract;
};