#include <programGraph/PrimitiveFunction.h>


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

PrimitiveFunction::PrimitiveFunction(std::vector<Datatype> inputTypes, std::vector<Datatype> outputTypes, std::function<std::vector<Value>(std::vector<Value>)> executeFunction)
	: Function(inputTypes, outputTypes)
	, m_executeFunction{executeFunction}
{}


std::vector<Value> PrimitiveFunction::operator()(std::vector<Value> inputs) const
{
	return m_executeFunction(inputs);
}