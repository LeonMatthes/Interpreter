#include <programGraph/PrimitiveFunction.h>

//////////////////////////////////////////////////////////////////////////
//					Math Primitives
//////////////////////////////////////////////////////////////////////////
PrimitiveFunction PrimitiveFunction::add(
	std::vector<Datatype>({ Datatype::DOUBLE, Datatype::DOUBLE }), 
	std::vector<Datatype>({ Datatype::DOUBLE }), 
	[](std::vector<Value> inputs)
	{
		double sum = inputs.at(0).getDouble() + inputs.at(1).getDouble();
		return std::vector<Value>({Value(sum)});
	});

PrimitiveFunction PrimitiveFunction::subtract(
	std::vector<Datatype>({ Datatype::DOUBLE, Datatype::DOUBLE }), 
	std::vector<Datatype>({Datatype::DOUBLE}),
	[](std::vector<Value> inputs) 
	{
		double result = inputs.at(0).getDouble() - inputs.at(1).getDouble();
		return std::vector<Value>({ Value(result) });
	});

PrimitiveFunction PrimitiveFunction::multiply(
	std::vector<Datatype>({ Datatype::DOUBLE, Datatype::DOUBLE }),
	std::vector<Datatype>({ Datatype::DOUBLE }),
	[](std::vector<Value> inputs)
	{
		double result = inputs.at(0).getDouble() * inputs.at(1).getDouble();
		return std::vector<Value>({ Value(result) });
	});

PrimitiveFunction PrimitiveFunction::divide(
	std::vector<Datatype>({ Datatype::DOUBLE, Datatype::DOUBLE }),
	std::vector<Datatype>({ Datatype::DOUBLE }),
	[](std::vector<Value> inputs)
	{
		double result = inputs.at(0).getDouble() / inputs.at(1).getDouble();
		return std::vector<Value>({ Value(result) });
	});

//////////////////////////////////////////////////////////////////////////
//				Comparison Primitives
//////////////////////////////////////////////////////////////////////////
PrimitiveFunction PrimitiveFunction::smaller(
	std::vector<Datatype>({ Datatype::DOUBLE, Datatype::DOUBLE }),
	std::vector<Datatype>({ Datatype::BOOLEAN }),
	[](std::vector<Value> inputs)
	{
		bool result = inputs.at(0).getDouble() < inputs.at(1).getDouble();
		return std::vector<Value>({ Value(result) });
	});

//////////////////////////////////////////////////////////////////////////
//				Boolean Primitives
//////////////////////////////////////////////////////////////////////////
PrimitiveFunction PrimitiveFunction::logicalAnd(
	std::vector<Datatype>({ Datatype::BOOLEAN, Datatype::BOOLEAN }),
	std::vector<Datatype>({ Datatype::BOOLEAN }),
	[](std::vector<Value> inputs)
	{
		bool result = inputs.at(0).getBoolean() && inputs.at(1).getBoolean();
		return std::vector<Value>({ Value(result) });
	});

PrimitiveFunction PrimitiveFunction::logicalOr(
	std::vector<Datatype>({ Datatype::BOOLEAN, Datatype::BOOLEAN }),
	std::vector<Datatype>({ Datatype::BOOLEAN }),
	[](std::vector<Value> inputs)
	{
		bool result = inputs.at(0).getBoolean() || inputs.at(1).getBoolean();
		return std::vector<Value>({ Value(result) });
	});

PrimitiveFunction PrimitiveFunction::logicalNot(
	std::vector<Datatype>({ Datatype::BOOLEAN }),
	std::vector<Datatype>({ Datatype::BOOLEAN }),
	[](std::vector<Value> inputs)
	{
		bool result = !inputs.front().getBoolean();
		return std::vector<Value>({ Value(result) });
	});

PrimitiveFunction::PrimitiveFunction(std::vector<Datatype> inputTypes, std::vector<Datatype> outputTypes, std::function<std::vector<Value>(std::vector<Value>)> executeFunction)
	: Function(inputTypes, outputTypes)
	, m_executeFunction{executeFunction}
{}


std::vector<Value> PrimitiveFunction::operator()(std::vector<Value> inputs) const
{
	return m_executeFunction(inputs);
}