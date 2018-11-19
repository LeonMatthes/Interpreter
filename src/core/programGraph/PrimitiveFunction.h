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
	std::string name() const;

	MAKE_VISITABLE;
protected:
	PrimitiveFunction(
		std::string name,
		std::vector<Datatype> inputTypes, 
		std::vector<Datatype> outputTypes, 
		std::function<std::vector<Value>(std::vector<Value>)> executeFunction);
	
	std::function<std::vector<Value>(std::vector<Value>)> m_executeFunction;
	std::string m_name;

	PrimitiveFunction() = delete;
private:

	//////////////////////////////////////////////////////////////////////////
	//							STATIC
	//////////////////////////////////////////////////////////////////////////
public:
	static PrimitiveFunction add;
	static PrimitiveFunction subtract;
	static PrimitiveFunction multiply;
	static PrimitiveFunction divide;

	static PrimitiveFunction smaller;

	static PrimitiveFunction logicalAnd;
	static PrimitiveFunction logicalOr;
	static PrimitiveFunction logicalNot;

	static PrimitiveFunction castBoolToDouble;
	static PrimitiveFunction castDoubleToBool;
};